//==============================================================================
// Copyright (C) John-Philip Taylor
// jpt13653903@gmail.com
//
// This file is part of ALCHA
//
// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
//==============================================================================

#include "Engine.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

ENGINE::ENGINE(){
  error = false;

  // TODO: Maybe pull constants into a class of their own?
  Constants["e" ] = 2.718281828459045235360;
  Constants["π" ] = 3.141592653589793238463;
  Constants["pi"] = 3.141592653589793238463;
  Constants["i" ].Add(0, 1);
  Constants["j" ].Add(0, 1);

  time_t RawTime;
  struct tm* Time;

  time(&RawTime);
  Time = localtime(&RawTime);

  Constants["__YEAR__"   ] = Time->tm_year+1900;
  Constants["__MONTH__"  ] = Time->tm_mon+1;
  Constants["__DAY__"    ] = Time->tm_mday;
  Constants["__HOUR__"   ] = Time->tm_hour;
  Constants["__MINUTE__" ] = Time->tm_min;
  Constants["__SECOND__" ] = Time->tm_sec;

  Constants["__WEEKDAY__"] = ((Time->tm_wday+6)%7)+1;
  Constants["__YEARDAY__"] = Time->tm_yday+1;
}
//------------------------------------------------------------------------------

ENGINE::~ENGINE(){
  while(!AstStack.empty()){
    delete AstStack.top();
    AstStack.pop();
  }
}
//------------------------------------------------------------------------------

void ENGINE::Error(AST::BASE* Ast, const char* Message){
  if(error) return;
  error = true;
  printf(
    ANSI_FG_BRIGHT_BLACK "Line "
    ANSI_FG_CYAN         "%05d "
    ANSI_FG_BRIGHT_BLACK "of "
    ANSI_FG_YELLOW       "%s\n"
    ANSI_FG_BRIGHT_RED   "  Error: "
    ANSI_RESET,
    Ast->Line,
    Ast->Filename.c_str()
  );
  if(Message) printf("%s\n", Message);
}
//------------------------------------------------------------------------------

void ENGINE::Warning(AST::BASE* Ast, const char* Message){
  printf(
    ANSI_FG_BRIGHT_BLACK "Line "
    ANSI_FG_CYAN         "%05d "
    ANSI_FG_BRIGHT_BLACK "of "
    ANSI_FG_YELLOW       "%s\n"
    ANSI_FG_MAGENTA      "  Warning: "
    ANSI_RESET,
    Ast->Line,
    Ast->Filename.c_str()
  );
  if(Message) printf("%s\n", Message);
}
//------------------------------------------------------------------------------

EXPRESSION* ENGINE::Evaluate(AST::EXPRESSION* Node){
  if(!Node) return 0;

  EXPRESSION* Result = 0;

  switch(Node->ExpressionType){
    case AST::EXPRESSION::String:
      Result = new EXPRESSION(Node->Line, Node->Filename, EXPRESSION::String);
      Result->StrValue = *(Node->StrValue);
      break;

    case AST::EXPRESSION::Literal:
      Result = new EXPRESSION(Node->Line, Node->Filename, EXPRESSION::Literal);
      Result->Value = *(Node->Value);
      break;

    case AST::EXPRESSION::Array:{
      Result = new EXPRESSION(Node->Line, Node->Filename, EXPRESSION::Array);
      auto Element = (AST::EXPRESSION*)Node->Right;
      while(Element){
        Result->Elements.push_back(Evaluate(Element));
        Element = (AST::EXPRESSION*)Element->Next;
      }
      break;
    }

    case AST::EXPRESSION::Identifier:{
      auto NamespaceIterator = NamespaceStack.begin();
      while(!Result && NamespaceIterator != NamespaceStack.end()){
        NAMESPACE* Namespace = *NamespaceIterator;
        while(!Result && Namespace){
          auto Object = Namespace->Symbols.find(Node->Name);
          if(Object != Namespace->Symbols.end()){
            if(Object->second){
              switch(Object->second->Type){
                case BASE::TYPE::Alias:{
                  auto Alias = (ALIAS*)Object->second;
                  NamespaceStack.push_front(Alias->Namespace);
                    Result = Evaluate(Alias->Expression);
                  NamespaceStack.pop_front();
                  break;
                }
                case BASE::TYPE::Pin:
                case BASE::TYPE::Net:{
                  auto Synthesisable = (SYNTHESISABLE*)Object->second;
                  Result = new EXPRESSION(Node->Line, Node->Filename, EXPRESSION::Object);
                  Result->ObjectRef = Synthesisable;
                  Result->Signed    = Synthesisable->Signed;
                  Result->Width     = Synthesisable->Width;
                  Result->FullScale = Synthesisable->FullScale;
                  Synthesisable->Used = true;
                  break;
                }
                default:{
                  Result = new EXPRESSION(Node->Line, Node->Filename, EXPRESSION::Object);
                  Result->ObjectRef = Object->second;
                  break;
                }
              }
            }
          }
          Namespace = Namespace->Namespace;
        }
        NamespaceIterator++;
      }
      if(!Result){
        NUMBER Constant;
        if(GetConstant(Node->Name.c_str(), &Constant)){
          Result = new EXPRESSION(Node->Line, Node->Filename, EXPRESSION::Literal);
          Result->Value = Constant;
        }else{
          Error(Node);
          printf("Identifier \"%s\" not defined\n", Node->Name.c_str());
        }
      }
      break;
    }

    case AST::EXPRESSION::VectorConcatenate:{
      Result = new EXPRESSION(Node->Line, Node->Filename, EXPRESSION::VectorConcatenate);
      auto Element = (AST::EXPRESSION*)Node->Right;
      while(Element){
        auto EvaluatedElement = Evaluate(Element);
        if(!EvaluatedElement){
          delete Result;
          return 0;
        }
        Result->Elements.push_back(EvaluatedElement);
        if(EvaluatedElement->Width){
          Result->Width += EvaluatedElement->Width;
        }else{
          Error(Element, "Vector element has undefined width");
          delete Result;
          return 0;
        }
        Element = (AST::EXPRESSION*)Element->Next;
      }
      Result->Signed    = false;
      Result->FullScale = 1;
      Result->FullScale.BinScale(Result->Width);
      break;
    }

    case AST::EXPRESSION::ArrayConcatenate:{
      Result = new EXPRESSION(Node->Line, Node->Filename, EXPRESSION::ArrayConcatenate);
      auto Element = (AST::EXPRESSION*)Node->Right;
      while(Element){
        Result->Elements.push_back(Evaluate(Element));
        Element = (AST::EXPRESSION*)Element->Next;
      }
      break;
    }

    case AST::EXPRESSION::FunctionCall:
      error("FunctionCall not yet implemented");
      break;

    case AST::EXPRESSION::Slice:
      error("Slice not yet implemented");
      break;

    case AST::EXPRESSION::AccessMember:{
      EXPRESSION* Left = 0;
      if(Node->Left) Left = Evaluate(Node->Left);

      if(!Left || !Node->Right || Node->Right->Type != AST::BASE::TYPE::Expression){
        error("Invalid member access expression");
        delete Left;
        return 0;
      }
      auto Right = (AST::EXPRESSION*)Node->Right;

      if(Left->ExpressionType == EXPRESSION::Object){
        if(Left->ObjectRef){
          switch(Left->ObjectRef->Type){
            case BASE::TYPE::Pin:{
              auto Pin = (PIN*)Left->ObjectRef;
              // TODO Implement explicit "driver", "enabled" and "pin" access
              error("Not yet implemented");
              delete Left;
              return 0;
            }
            case BASE::TYPE::Module:
            case BASE::TYPE::Group:{
              auto Namespace = (NAMESPACE*)Left->ObjectRef;
              auto Found  = Namespace->Symbols.find(Right->Name);
              if(Found == Namespace->Symbols.end()){
                Error(Node);
                printf("Object %s not found in namespace %s\n",
                       Right->Name.c_str(), Namespace->Name.c_str());
                delete Left;
                return 0;
              }
              if(Found->second){
                if(Found->second->Type == BASE::TYPE::Alias){
                  auto Alias = (ALIAS*)Found->second;
                  NamespaceStack.push_front(Alias->Namespace);
                    Result = Evaluate(Alias->Expression);
                  NamespaceStack.pop_front();
                }else{
                  Result = new EXPRESSION(Node->Line, Node->Filename, EXPRESSION::Object);
                  Result->ObjectRef = Found->second;
                  if(Result->ObjectRef->Type == BASE::TYPE::Pin ||
                     Result->ObjectRef->Type == BASE::TYPE::Net ){
                    auto Object = (SYNTHESISABLE*)Result->ObjectRef;
                    Object->Used      = true;
                    Result->Signed    = Object->Signed;
                    Result->Width     = Object->Width;
                    Result->FullScale = Object->FullScale;
                  }
                }
              }
              delete Left;
              break;
            }
            default:{
              Error(Node, "Invalid member access");
              delete Left;
              return 0;
            }
          }
        }else{
          error("Unexpected null expression");
          delete Left;
          return 0;
        }
      }else{
        // TODO Could be a slice expression, which is not supported yet
        error("Unimplemented member access expression");
        delete Left;
        return 0;
      }
      break;
    }

    case AST::EXPRESSION::AccessMemberSafe:
      error("AccessMemberSafe not yet implemented");
      break;

    case AST::EXPRESSION::AccessAttribute:{
      EXPRESSION* Left = 0;
      if(Node->Left) Left = Evaluate(Node->Left);

      if(!Left || !Node->Right || Node->Right->Type != AST::BASE::TYPE::Expression){
        error("Invalid attribute access expression");
        delete Left;
        return 0;
      }
      auto Right = (AST::EXPRESSION*)Node->Right;

      if(Left->ExpressionType == EXPRESSION::Object){
        auto Object = Left->ObjectRef;
        auto Found  = Object->GetAttrib(Right->Name);
        if(!Found){
          Error(Node);
          printf("Attribute %s not found in object %s\n",
                 Right->Name.c_str(), Object->Name.c_str());
          delete Left;
          return 0;
        }
        Result = new EXPRESSION(Found);
        Result->Line     = Node->Line;
        Result->Filename = Node->Filename;
        delete Left;
      }else{
        // TODO Could be a slice expression, which is not supported yet
        error("Unimplemented attribute access expression");
        delete Left;
        return 0;
      }
      break;
    }

    case AST::EXPRESSION::Range:
      error("Range not yet implemented");
      break;

    case AST::EXPRESSION::Increment:
      error("Increment not yet implemented");
      break;

    case AST::EXPRESSION::Decrement:
      error("Decrement not yet implemented");
      break;

    case AST::EXPRESSION::Factorial:
      // TODO: Assign the expression to Left
      error("Factorial not yet implemented");
      break;

    case AST::EXPRESSION::Negate:
      Result = new EXPRESSION(Node->Line, Node->Filename, EXPRESSION::Negate);
      if(Node->Right && Node->Right->Type == AST::BASE::TYPE::Expression){
        Result->Right = Evaluate((AST::EXPRESSION*)Node->Right);
      }
      if(!Result->Right){
        delete Result;
        return 0;
      }
      Result->Signed    = Result->Right->Signed;
      Result->Width     = Result->Right->Width;
      Result->FullScale = Result->Right->FullScale;
      break;

    case AST::EXPRESSION::Bit_NOT:
      Result = new EXPRESSION(Node->Line, Node->Filename, EXPRESSION::Bit_NOT);
      if(Node->Right && Node->Right->Type == AST::BASE::TYPE::Expression){
        Result->Right = Evaluate((AST::EXPRESSION*)Node->Right);
      }
      if(!Result->Right){
        delete Result;
        return 0;
      }
      Result->Signed    = Result->Right->Signed;
      Result->Width     = Result->Right->Width;
      Result->FullScale = Result->Right->FullScale;
      break;

    case AST::EXPRESSION::Raw:
      error("Raw not yet implemented");
      // TODO: Implement as a cast to (N, 2^N) for unsigned
      //       and (N+1, 2^(N+1)) for signed
      break;

    case AST::EXPRESSION::AND_Reduce:
      error("AND_Reduce not yet implemented");
      break;

    case AST::EXPRESSION::NAND_Reduce:
      error("NAND_Reduce not yet implemented");
      break;

    case AST::EXPRESSION::OR_Reduce:
      error("OR_Reduce not yet implemented");
      break;

    case AST::EXPRESSION::NOR_Reduce:
      error("NOR_Reduce not yet implemented");
      break;

    case AST::EXPRESSION::XOR_Reduce:
      error("XOR_Reduce not yet implemented");
      break;

    case AST::EXPRESSION::XNOR_Reduce:
      error("XNOR_Reduce not yet implemented");
      break;

    case AST::EXPRESSION::Logical_NOT:
      error("Logical_NOT not yet implemented");
      break;

    case AST::EXPRESSION::Cast:
      // TODO: Root has the target type; Root->Left has the original expression
      //       Root->Right is optional and carry the class name (when applicable)
      error("Cast not yet implemented");
      break;

    case AST::EXPRESSION::Replicate:
      error("Replicate not yet implemented");
      break;

    case AST::EXPRESSION::Multiply:
      Result = new EXPRESSION(Node->Line, Node->Filename, EXPRESSION::Multiply);
      if(Node->Left) Result->Left = Evaluate(Node->Left);
      if(Node->Right && Node->Right->Type == AST::BASE::TYPE::Expression){
        Result->Right = Evaluate((AST::EXPRESSION*)Node->Right);
      }
      break;

    case AST::EXPRESSION::Divide:
      error("Divide not yet implemented");
      break;

    case AST::EXPRESSION::Modulus:
      error("Modulus not yet implemented");
      break;

    case AST::EXPRESSION::Exponential:
      error("Exponential not yet implemented");
      break;

    case AST::EXPRESSION::Add:
      error("Add not yet implemented");
      break;

    case AST::EXPRESSION::Subtract:
      error("Subtract not yet implemented");
      break;

    case AST::EXPRESSION::Shift_Left:
      error("Shift_Left not yet implemented");
      break;

    case AST::EXPRESSION::Shift_Right:
      error("Shift_Right not yet implemented");
      break;

    case AST::EXPRESSION::Less:
      error("Less not yet implemented");
      break;

    case AST::EXPRESSION::Greater:
      error("Greater not yet implemented");
      break;

    case AST::EXPRESSION::Less_Equal:
      error("Less_Equal not yet implemented");
      break;

    case AST::EXPRESSION::Greater_Equal:
      error("Greater_Equal not yet implemented");
      break;

    case AST::EXPRESSION::Equal:
      error("Equal not yet implemented");
      break;

    case AST::EXPRESSION::Not_Equal:
      error("Not_Equal not yet implemented");
      break;

    case AST::EXPRESSION::Bit_AND:
      error("Bit_AND not yet implemented");
      break;

    case AST::EXPRESSION::Bit_NAND:
      error("Bit_NAND not yet implemented");
      break;

    case AST::EXPRESSION::Bit_OR:
      error("Bit_OR not yet implemented");
      break;

    case AST::EXPRESSION::Bit_NOR:
      error("Bit_NOR not yet implemented");
      break;

    case AST::EXPRESSION::Bit_XOR:
      error("Bit_XOR not yet implemented");
      break;

    case AST::EXPRESSION::Bit_XNOR:
      error("Bit_XNOR not yet implemented");
      break;

    case AST::EXPRESSION::Logical_AND:
      error("Logical_AND not yet implemented");
      break;

    case AST::EXPRESSION::Logical_OR:
      error("Logical_OR not yet implemented");
      break;

    case AST::EXPRESSION::Conditional:
      // TODO: There should be a third component...  Left and Right of Right?
      //       Or break it down to an if-statement?
      error("Conditional not yet implemented");
      break;

    default:
      error("Unknown expression type: %d", Node->ExpressionType);
      break;
  }
  Simplify(Result);
  return Result;
}
//------------------------------------------------------------------------------

bool ENGINE::GetConstant(const string& Name, NUMBER* Constant){
  auto Result = Constants.find(Name);
  if(Result == Constants.end()) return false;

  *Constant = Result->second;

  return true;
}
//------------------------------------------------------------------------------

bool ENGINE::ApplyAttributes(
  BASE*        Object,
  std::string& Name,
  EXPRESSION*  Value,
  AST::BASE*   Ast
){
  if(!Value){
    Error(Ast, "Invalid attribute expression");
    return false;
  }

  switch(Value->ExpressionType){
    case EXPRESSION::String:
    case EXPRESSION::Literal:
      break;

    case EXPRESSION::Array:
      // TODO Make sure that the array only contains strings or literals
      break;

    default:
      Error(Ast, "Attribute values must be strings, literals or arrays");
      delete Value;
      return false;
  }
  Object->Attributes[Name] = Value;
  return true;
}
//------------------------------------------------------------------------------

bool ENGINE::ApplyAttributes(BASE* Object, AST::ASSIGNMENT* AttributeList){
  while(AttributeList){
    if(!AttributeList->Left){
      error("Null attribute name");
      return false;
    }
    if(!AttributeList->Right){
      error("Null attribute value");
      return false;
    }
    if(AttributeList->Left->ExpressionType != AST::EXPRESSION::Identifier){
      error("Attribute LHS not an identifier");
      return false;
    }
    auto a = Object->Attributes.find(AttributeList->Left->Name);
    if(a != Object->Attributes.end()){
      Warning(AttributeList);
      printf("Overwriting attribute %s\n", AttributeList->Left->Name.c_str());
      if(a->second) delete a->second;
    }

    if(!ApplyAttributes(
      Object,
      AttributeList->Left->Name,
      Evaluate(AttributeList->Right),
      AttributeList
    )) return false;

    AttributeList = (AST::ASSIGNMENT*)AttributeList->Next;
  }
  return true;
}
//------------------------------------------------------------------------------

bool ENGINE::ApplyParameters(SYNTHESISABLE* Object, AST::BASE* Parameter){
  int  Position          = 0; // Negative => named parameters
  bool ExplicitFullScale = false;

  while(Parameter){
    switch(Parameter->Type){
      case AST::BASE::TYPE::Expression:{
        if(Position < 0) return false; // Mixing named and positional parameters

        EXPRESSION* Param = Evaluate((AST::EXPRESSION*)Parameter);
        if(!Param){
          Error(Parameter, "Invalid parameter expression");
          return false;
        }

        switch(Param->ExpressionType){
          case EXPRESSION::Literal:
            switch(Position){
              case 0:
                if(!Param->Value.IsInt()) return false;
                Object->Width = round(Param->Value.GetReal());
                break;

              case 1:
                ExplicitFullScale = true;
                Object->FullScale = Param->Value;
                break;

              default: // Too many parameters
                delete Param;
                return false;
            }
            break;

          default:
            Error(Parameter, "Parameters must be pure scripting expressions");
            delete Param;
            return false;
        }
        delete Param;
        break;
      }
      //------------------------------------------------------------------------

      case AST::BASE::TYPE::Assignment:{
        // auto Param = (AST::EXPRESSION*)Parameter;
        Position = -1;
        error("Not yet implemented");
        break;
      }
      //------------------------------------------------------------------------

      default:
        return false;
    }
    Parameter = Parameter->Next;
    if(Position >= 0) Position++;
  }
  if(Object->Width < 0){
    Object->Width *= -1;
    Object->Signed = true;
  }
  if(!ExplicitFullScale){
    Object->FullScale = pow(2.0, Object->Width);
  }
  if(Object->FullScale.IsReal() && (Object->FullScale.GetReal() < 0)){
    Object->FullScale.Mul(-1);
    Object->Signed = true;
  }
  return true;
}
//------------------------------------------------------------------------------

void ENGINE::SimplifyFilename(string& Filename){
  int  LastSlash;
  int  n, c;
  bool Changes;

  do{
    Changes   = false;
    LastSlash = -1;

    for(n = 0; Filename[n]; n++){
      if(Filename[n] == '/'
        #ifdef WINVER
          || Filename[n] == '\\'
        #endif
      ){
        if(
          Filename[n+1] == '.' &&
          Filename[n+2] == '.' && (
            Filename[n+3] == '/'
            #ifdef WINVER
              || Filename[n+3] == '\\'
            #endif
          ) && (
            n > LastSlash+3 ||
            Filename[LastSlash+1] != '.' ||
            Filename[LastSlash+2] != '.'
          )
        ){
          n += 4;
          for(c = LastSlash+1; Filename[n]; c++, n++) Filename[c] = Filename[n];
          Filename[c] = 0;
          Changes = true;
          break;
        }else{
          LastSlash = n;
        }
      }
    }
  }while(Changes);

  Filename.resize(n);
}
//------------------------------------------------------------------------------

bool ENGINE::Import(AST::IMPORT* Ast){
  string Filename;
  bool   OwnNamespace = !Ast->Namespace.empty();

  if(OwnNamespace){
    auto Found = NamespaceStack.front()->Symbols.find(Ast->Namespace);
    if(Found != NamespaceStack.front()->Symbols.end()){
      Error(Ast);
      printf("Symbol \"%s\" already exists in the current namespace\n",
             Ast->Namespace.c_str());
      return false;
    }
    auto Module = new MODULE(Ast->Line, Ast->Filename, Ast->Namespace.c_str());
    NamespaceStack.front()->Symbols[Ast->Namespace] = Module;
    NamespaceStack.push_front(Module);
  }

  string& Path = Ast->Filename;
  int n;
  for(n = Path.length()-1; n >= 0; n--){
    if(Path[n] == '/') break;
  }
  if(n >= 0) Filename = Path.substr(0, n+1);
  Filename += Ast->File;
  Filename += ".alc";
  SimplifyFilename(Filename);
  Debug.print("\nFilename = %s\n", Filename);

  bool Result = Run(Filename.c_str());

  if(OwnNamespace){
    NamespaceStack.pop_front();
  }
  return Result;
}
//------------------------------------------------------------------------------

bool ENGINE::Group(AST::GROUP* Ast){
  if(Ast->Identifier.empty()){
    error("Anonymous groups not supported yet");
    return false;
  }

  auto Found = NamespaceStack.front()->Symbols.find(Ast->Identifier);
  if(Found != NamespaceStack.front()->Symbols.end()){
    Error(Ast);
    printf("Symbol \"%s\" already exists in the current namespace\n",
           Ast->Identifier.c_str());
    return false;
  }
  auto Object = new NETLIST::GROUP(Ast->Line, Ast->Filename, Ast->Identifier.c_str());
  ApplyAttributes(Object, Ast->Attributes);
  NamespaceStack.front()->Symbols[Ast->Identifier] = Object;
  NamespaceStack.push_front(Object);

  bool Result = Run(Ast->Body);

  NamespaceStack.pop_front();
  return Result;
}
//------------------------------------------------------------------------------

bool ENGINE::Alias(AST::ALIAS* Ast){
  auto Symbol = NamespaceStack.front()->Symbols.find(Ast->Identifier);
  if(Symbol != NamespaceStack.front()->Symbols.end()){
    Error(Ast);
    printf("Symbol \"%s\" already defined in the current namespace\n",
           Ast->Identifier.c_str());
    return false;
  }

  auto Object = new ALIAS(Ast->Line, Ast->Filename, Ast->Identifier.c_str(), Ast->Expression);

  NamespaceStack.front()->Symbols[Object->Name] = Object;

  return true;
}
//------------------------------------------------------------------------------

bool ENGINE::Definition(AST::DEFINITION* Ast){
  auto Identifier = Ast->Identifiers;

  while(Identifier){
    auto Symbol = NamespaceStack.front()->Symbols.find(Identifier->Identifier);
    if(Symbol != NamespaceStack.front()->Symbols.end()){
      Error(Ast);
      printf("Symbol \"%s\" already defined in the current namespace\n",
             Identifier->Identifier.c_str());
      return false;
    }

    if(Identifier->Function){
      error("Not yet implemented");
      Identifier = Identifier->Next;
      continue;
    }
    if(Identifier->Parameters) error("Not yet implemented");

    switch(Ast->DefinitionType){
      case AST::DEFINITION::Void:
        error("Not yet implemented");
        break;

      case AST::DEFINITION::Auto:
        error("Not yet implemented");
        break;

      case AST::DEFINITION::Pin:{
        auto Pin = new PIN(Ast->Line, Ast->Filename, Identifier->Identifier.c_str());
        Pin->Direction = Ast->Direction;
        if(!ApplyParameters(Pin, Ast->Parameters)) Error(Ast, "Invalid parameters");
        if(!ApplyAttributes(Pin, Ast->Attributes)) Error(Ast, "Invalid attributes");
        NamespaceStack.front()->Symbols[Pin->Name] = Pin;
        if(Identifier->Initialiser){
          if(!Assignment(Identifier->Initialiser)) return false;
        }
        break;
      }

      case AST::DEFINITION::Net:{
        auto Net = new NET(Ast->Line, Ast->Filename, Identifier->Identifier.c_str());
        Net->Direction = Ast->Direction;
        if(!ApplyParameters(Net, Ast->Parameters)) Error(Ast, "Invalid parameters");
        if(!ApplyAttributes(Net, Ast->Attributes)) Error(Ast, "Invalid attributes");
        NamespaceStack.front()->Symbols[Net->Name] = Net;
        if(Identifier->Initialiser){
          if(!Assignment(Identifier->Initialiser)) return false;
        }
        break;
      }

      case AST::DEFINITION::Byte:{
        auto Byte = new NETLIST::BYTE(Ast->Line, Ast->Filename, Identifier->Identifier.c_str());
        if(!ApplyAttributes(Byte, Ast->Attributes)) Error(Ast, "Invalid attributes");
        NamespaceStack.front()->Symbols[Byte->Name] = Byte;
        if(Identifier->Initialiser){
          if(!Assignment(Identifier->Initialiser)) return false;
        }
        break;
      }

      case AST::DEFINITION::Char:{
        auto Char = new CHARACTER(Ast->Line, Ast->Filename, Identifier->Identifier.c_str());
        if(!ApplyAttributes(Char, Ast->Attributes)) Error(Ast, "Invalid attributes");
        NamespaceStack.front()->Symbols[Char->Name] = Char;
        if(Identifier->Initialiser){
          if(!Assignment(Identifier->Initialiser)) return false;
        }
        break;
      }

      case AST::DEFINITION::Num:{
        auto Number = new NUM(Ast->Line, Ast->Filename, Identifier->Identifier.c_str());
        if(!ApplyAttributes(Number, Ast->Attributes)) Error(Ast, "Invalid attributes");
        NamespaceStack.front()->Symbols[Number->Name] = Number;
        if(Identifier->Initialiser){
          if(!Assignment(Identifier->Initialiser)) return false;
        }
        break;
      }

      case AST::DEFINITION::Func:
        error("Not yet implemented");
        break;

      case AST::DEFINITION::ClassInstance:
        error("Not yet implemented");
        break;

      default:
        error("Unknown definition type: %d", Ast->DefinitionType);
        return false;
    }
    Identifier = Identifier->Next;
  }
  return true;
}
//------------------------------------------------------------------------------

bool ENGINE::GetLHS_Object(BASE* Object, target_list& List, AST::BASE* Ast){
  TARGET_LIST ListNode;

  bool Result = false;

  if(Object){
    switch(Object->Type){
      case BASE::TYPE::Pin:{
        auto Pin = (PIN*)Object;
        if(Pin->Direction == AST::DEFINITION::Input){
          Error(Ast, "Cannot assign to an input pin");
          return false;
        }
        Pin->Used = true;
        ListNode.Object     =  Pin;
        ListNode.Expression = &Pin->Driver;
        List.push_back(ListNode);
        Result = true;
        break;
      }
      case BASE::TYPE::Net:{
        auto Net = (NET*)Object;
        ListNode.Object     =  Net;
        ListNode.Expression = &Net->Value;
        List.push_back(ListNode);
        Result = true;
        break;
      }
      case BASE::TYPE::Number:
      case BASE::TYPE::Byte:
      case BASE::TYPE::Character:{
        ListNode.Object = Object;
        ListNode.Expression = 0;
        List.push_back(ListNode);
        Result = true;
        break;
      }
      case BASE::TYPE::Alias:{
        auto Alias = (ALIAS*)Object;
        NamespaceStack.push_front(Alias->Namespace);
          Result = GetLHS(Alias->Expression, List);
        NamespaceStack.pop_front();
        break;
      }
      case BASE::TYPE::Array:{
        error("Array assignment not yet implemented");
        break;
      }
      case BASE::TYPE::Module:
      case BASE::TYPE::Group:{
        ListNode.Object     = Object;
        ListNode.Expression = 0;
        List.push_back(ListNode);
        Result = true;
        break;
      }
      default:
        error("Unknown object type %d", (int)Object->Type);
        break;
    }
  }
  return Result;
}
//------------------------------------------------------------------------------

bool ENGINE::GetLHS(AST::EXPRESSION* Node, target_list& List){
  if(!Node) return 0;

  bool        Result = false;
  TARGET_LIST ListNode;

  switch(Node->ExpressionType){
    case AST::EXPRESSION::Array:{
      error("Array not yet implemented");
      // Idea: Simply call GetLHS recursively for each array element
      //
      // auto Element = (AST::EXPRESSION*)Node->Right;
      // while(Element){
      //   Result->Elements.push_back(Evaluate(Element));
      //   Element = (AST::EXPRESSION*)Element->Next;
      // }
      break;
    }

    case AST::EXPRESSION::Identifier:{
      auto NamespaceIterator = NamespaceStack.begin();
      while(!Result && NamespaceIterator != NamespaceStack.end()){
        NAMESPACE* Namespace = *NamespaceIterator;
        while(!Result && Namespace){
          auto Object = Namespace->Symbols.find(Node->Name);
          if(Object != Namespace->Symbols.end()){
            Result = GetLHS_Object(Object->second, List, Node);
          }
          Namespace = Namespace->Namespace;
        }
        NamespaceIterator++;
      }
      break;
    }

    case AST::EXPRESSION::VectorConcatenate:{
      error("VectorConcatenate not yet implemented");
      // Result = new EXPRESSION(Ast->Line, Ast->Filename, EXPRESSION::VectorConcatenate);
      // auto Element = (AST::EXPRESSION*)Node->Right;
      // while(Element){
      //   Result->Elements.push_back(Evaluate(Element));
      //   Element = (AST::EXPRESSION*)Element->Next;
      // }
      break;
    }

    case AST::EXPRESSION::ArrayConcatenate:{
      error("ArrayConcatenate not yet implemented");
      // Result = new EXPRESSION(Ast->Line, Ast->Filename, EXPRESSION::ArrayConcatenate);
      // auto Element = (AST::EXPRESSION*)Node->Right;
      // while(Element){
      //   Result->Elements.push_back(Evaluate(Element));
      //   Element = (AST::EXPRESSION*)Element->Next;
      // }
      break;
    }

    case AST::EXPRESSION::Slice:
      error("Slice not yet implemented");
      break;

    case AST::EXPRESSION::AccessMember:{
      if(!Node->Left || !Node->Right || Node->Right->Type != AST::BASE::TYPE::Expression){
        error("Invalid member access expression");
        return 0;
      }

      target_list LeftList;
      if(!GetLHS(Node->Left, LeftList)) return false;
      if(LeftList.empty()){
        error("Empty target list");
        return false;
      }
      if(LeftList.size() > 1){
        error("Multiple assignment targets not yet supported");
        return false;
      }
      auto Left  = LeftList.front();
      auto Right = (AST::EXPRESSION*)Node->Right;

      if(Left.isAttribute){
        Error(Node, "Attributes cannot be structures");
        return false;
      }

      switch(Left.Object->Type){
        case BASE::TYPE::Pin:{
          auto Pin = (PIN*)Left.Object;
          ListNode.Object = Pin;
          if(Right->ExpressionType != AST::EXPRESSION::Identifier){
            Error(Node, "Invalid pin member");
            return false;
          }
          if(Pin->Direction == AST::DEFINITION::Input){
            Error(Node, "Cannot assign to an input pin.");
            return false;
          }
          if(Right->Name == "driver"){
            ListNode.Expression = &Pin->Driver;
          }else if(Right->Name == "enabled"){
            ListNode.Expression = &Pin->Enabled;
          }else{
            Error(Node, "Valid pin members are \"driver\" and \"enabled\" only");
            return false;
          }
          List.push_back(ListNode);
          Result = true;
          break;
        }
        case BASE::TYPE::Module:
        case BASE::TYPE::Group:{
          auto Namespace = (NAMESPACE*)Left.Object;
          auto Object    = Namespace->Symbols.find(Right->Name);
          if(Object == Namespace->Symbols.end()){
            Error(Node);
            printf("Object %s not found in namespace %s\n",
                   Right->Name.c_str(), Namespace->Name.c_str());
            return false;
          }
          Result = GetLHS_Object(Object->second, List, Node);
          break;
        }
        case BASE::TYPE::Array:{
          error("Array not yet implemented");
          return false;
        }
        default:
          Error(Node);
          printf("Invalid type for member access: %d\n", (int)Left.Object->Type);
          return false;
      }
      break;
    }

    case AST::EXPRESSION::AccessMemberSafe:
      error("AccessMemberSafe not yet implemented");
      break;

    case AST::EXPRESSION::AccessAttribute:{
      if(Node->Left){
        target_list LeftList;
        if(!GetLHS(Node->Left, LeftList)) return false;
        if(LeftList.empty()){
          error("Empty target list");
          return false;
        }
        if(LeftList.size() > 1){
          error("Multiple assignment targets not yet supported");
          return false;
        }
        ListNode = LeftList.front();
      }else{ // An attribute of the current namespace
        ListNode.Object = NamespaceStack.front();
      }
      if(ListNode.isAttribute){
        Error(Node, "Attributes are not hierarchical");
        return false;
      }

      if(Node->Right && Node->Right->Type == AST::BASE::TYPE::Expression){
        auto Right = (AST::EXPRESSION*)Node->Right;
        if(Right->ExpressionType == AST::EXPRESSION::Identifier){
          // The process of adding an entry initialises the pointer to null.
          // The default constructor of the pointer type is called.
          ListNode.Expression = &ListNode.Object->Attributes[Right->Name];
          ListNode.isAttribute = true;
          List.push_back(ListNode);
          Result = true;
        }else{
          // TODO Could be a slice expression, which is not supported yet
          error("Unimplemented attribute access expression");
        }
      }
      break;
    }

    default:
      Error(Node, "Invalid LHS expression");
      break;
  }
  return Result;
}
//------------------------------------------------------------------------------

void ENGINE::Simplify(EXPRESSION*& Root){
  if(!Root) return;

  EXPRESSION* Temp;

  Simplify(Root->Left );
  Simplify(Root->Right);

  switch(Root->ExpressionType){
    // Pass these directly
    case EXPRESSION::String:
    case EXPRESSION::Literal:
    case EXPRESSION::Array:
      break;

    case EXPRESSION::Object:
      if(Root->ObjectRef){
        switch(Root->ObjectRef->Type){
          case BASE::TYPE::Byte:{
            auto Byte = (NETLIST::BYTE*)Root->ObjectRef;
            Root->ExpressionType = EXPRESSION::Literal;
            Root->Value = Byte->Value;
          }
          case BASE::TYPE::Character:{
            auto Char = (CHARACTER*)Root->ObjectRef;
            Root->ExpressionType = EXPRESSION::Literal;
            Root->Value = Char->Value;
          }
          case BASE::TYPE::Number:{
            auto Num = (NUM*)Root->ObjectRef;
            Root->ExpressionType = EXPRESSION::Literal;
            Root->Value = Num->Value;
          }
          default:
            break;
        }
      }
      break;

    case EXPRESSION::VectorConcatenate:
      if(Root->Elements.empty()){
        error("Unexpected null pointer");
        break;
      }
      // TODO: Check members for literals
      break;

    case EXPRESSION::ArrayConcatenate:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::FunctionCall:
      if(!Root->Left){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Slice:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      // TODO Returns a new array (or scalar)
      error("Not yet implemented");
      break;

    case EXPRESSION::Factorial:
      if(!Root->Left){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Negate:
      if(!Root->Right){
        error("Unexpected null pointer");
        break;
      }
      if(Root->Right->ExpressionType == EXPRESSION::Literal){
        Temp = Root->Right;
        Root->Right = 0;
        delete Root;
        Root = Temp;
        Root->Value.Mul(-1);
      }
      break;

    case EXPRESSION::Bit_NOT:
      if(!Root->Right){
        error("Unexpected null pointer");
        break;
      }
      if(Root->Right->ExpressionType == EXPRESSION::Literal){
        error("Not yet implemented");
        // TODO: Literals require a length, so it must have been cast to a
        //       specific length for this to be valid
      }
      break;

    case EXPRESSION::AND_Reduce:
      if(!Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::NAND_Reduce:
      if(!Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::OR_Reduce:
      if(!Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::NOR_Reduce:
      if(!Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::XOR_Reduce:
      if(!Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::XNOR_Reduce:
      if(!Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Logical_NOT:
      if(!Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Cast:
      // TODO: Root has the target type; Root->Left has the original expression
      //       Root->Right is optional and carry the class name (when applicable)
      if(!Root->Left){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Replicate:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Multiply:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      if(Root->Left->ExpressionType == EXPRESSION::Literal){
        NUMBER Num = Root->Left->Value;

        if(Num.IsReal() && Num.GetReal() == 0.0){
          Temp = Root->Left; // Keep the literal and make it zero
          Root->Left = 0;
          delete Root;
          Root = Temp;
          Root->Value = 0;
        }else{
          Temp = Root->Right; // Keep the expression and modify the full-scale
          Root->Right = 0;
          delete Root;
          Root = Temp;
          if(Root->ExpressionType == EXPRESSION::Literal){
            Root->Value.Mul(Num);
          }else{
            if(Root->Width == 0) error("Unexpected 0 width");
            Root->FullScale.Mul(Num);
          }
        }
      }else if(Root->Right->ExpressionType == EXPRESSION::Literal){
        NUMBER Num = Root->Right->Value;

        if(Num.IsReal() && Num.GetReal() == 0.0){
          Temp = Root->Right; // Keep the literal and make it zero
          Root->Right = 0;
          delete Root;
          Root = Temp;
          Root->Value = 0;
        }else{
          Temp = Root->Left; // Keep the expression and modify the full-scale
          Root->Left = 0;
          delete Root;
          Root = Temp;
          if(Root->ExpressionType == EXPRESSION::Literal){
            Root->Value.Mul(Num);
          }else{
            if(Root->Width == 0) error("Unexpected 0 width");
            Root->FullScale.Mul(Num);
          }
        }
      }
      // TODO When multiplying an expression with a literal, simply scale the
      //      full-scale of the expression
      break;

    case EXPRESSION::Divide:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Modulus:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Exponential:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Add:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      if(
        Root->Left ->ExpressionType == EXPRESSION::Literal &&
        Root->Right->ExpressionType == EXPRESSION::Literal
      ){
        Root->ExpressionType = EXPRESSION::Literal;
        Root->Value = Root->Left->Value;
        Root->Value.Add(Root->Right->Value);
        delete Root->Left ; Root->Left  = 0;
        delete Root->Right; Root->Right = 0;
      }
      // TODO When adding an expression to a literal, follow the rules
      //      in the SIPS article
      break;

    case EXPRESSION::Subtract:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      if(
        Root->Left ->ExpressionType == EXPRESSION::Literal &&
        Root->Right->ExpressionType == EXPRESSION::Literal
      ){
        Root->ExpressionType = EXPRESSION::Literal;
        Root->Value = Root->Left->Value;
        Root->Value.Sub(Root->Right->Value);
        delete Root->Left ; Root->Left  = 0;
        delete Root->Right; Root->Right = 0;
      }
      // TODO When subtracting an expression from a literal (or vice versa),
      //      follow the rules in the SIPS article
      break;

    case EXPRESSION::Shift_Left:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Shift_Right:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Less:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Greater:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Less_Equal:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Greater_Equal:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Equal:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Not_Equal:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Bit_AND:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Bit_NAND:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Bit_OR:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Bit_NOR:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Bit_XOR:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Bit_XNOR:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Logical_AND:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Logical_OR:
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    case EXPRESSION::Conditional:
      // TODO: There should be a third component...  Left and Right of Right?
      //       Or break it down to an if-statement?
      if(!Root->Left || !Root->Right){
        error("Unexpected null pointer");
        break;
      }
      error("Not yet implemented");
      break;

    default:
      error("Unexpected expression type %d", Root->ExpressionType);
      break;
  }
}
//------------------------------------------------------------------------------

bool ENGINE::Assignment(AST::ASSIGNMENT* Ast){
  target_list Left;
  EXPRESSION* Right;
  EXPRESSION* Temp;

  if(!GetLHS(Ast->Left, Left)) return false;
  if(Left.empty()){
    Error(Ast, "Target object list is empty");
    return false;
  }

  if(Left.size() > 1){
    error("Multiple assignment targets not supported yet");
    return false;
  }

  Right = Evaluate(Ast->Right);
  if(!Right){
    // This is ok -- generally caused by a syntax or semantic error, but should
    // halt further compilation anyway
    return false;
  }

  BASE* Object = Left.front().Object;
  if(!Object){
    error("Unexpected null reference");
    return false;
  }

  EXPRESSION*  ScriptTarget = 0;
  EXPRESSION** Target = Left.front().Expression;
  if(!Target){
    switch(Object->Type){
      case BASE::TYPE::Byte:
      case BASE::TYPE::Character:
      case BASE::TYPE::Number:
        ScriptTarget = new EXPRESSION(Ast->Line, Ast->Filename, EXPRESSION::Object);
        ScriptTarget->ObjectRef = Object;
        Target = &ScriptTarget;
        break;

      default:
        error("Unexpected null reference");
        return false;
    }
  }

  bool RawAssign = true;

  switch(Ast->AssignmentType){
    case AST::ASSIGNMENT::Assign:
      RawAssign = false;
      if(*Target) delete *Target;
      *Target = Right;
      break;

    case AST::ASSIGNMENT::Raw_Assign:
      RawAssign = true;
      if(*Target) delete *Target;
      *Target = Right;
      break;

    case AST::ASSIGNMENT::Append_Assign:
      RawAssign = false;
      if(*Target){
        Temp = new EXPRESSION(Ast->Line, Ast->Filename, EXPRESSION::ArrayConcatenate);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error(Ast, "Append-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case AST::ASSIGNMENT::Add_Assign:
      RawAssign = false;
      if(*Target){
        Temp = new EXPRESSION(Ast->Line, Ast->Filename, EXPRESSION::Add);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error(Ast, "Add-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case AST::ASSIGNMENT::Subtract_Assign:
      RawAssign = false;
      if(*Target){
        Temp = new EXPRESSION(Ast->Line, Ast->Filename, EXPRESSION::Subtract);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error(Ast, "Subtract-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case AST::ASSIGNMENT::Multiply_Assign:
      RawAssign = false;
      if(*Target){
        Temp = new EXPRESSION(Ast->Line, Ast->Filename, EXPRESSION::Multiply);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error(Ast, "Multiply-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case AST::ASSIGNMENT::Divide_Assign:
      RawAssign = false;
      if(*Target){
        Temp = new EXPRESSION(Ast->Line, Ast->Filename, EXPRESSION::Divide);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error(Ast, "Divide-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case AST::ASSIGNMENT::Modulus_Assign:
      RawAssign = false;
      if(*Target){
        Temp = new EXPRESSION(Ast->Line, Ast->Filename, EXPRESSION::Modulus);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error(Ast, "Modulus-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case AST::ASSIGNMENT::Exponential_Assign:
      RawAssign = false;
      if(*Target){
        Temp = new EXPRESSION(Ast->Line, Ast->Filename, EXPRESSION::Exponential);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error(Ast, "Exponential-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case AST::ASSIGNMENT::AND_Assign:
      RawAssign = true;
      if(*Target){
        Temp = new EXPRESSION(Ast->Line, Ast->Filename, EXPRESSION::Bit_AND);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error(Ast, "AND-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case AST::ASSIGNMENT::OR_Assign:
      RawAssign = true;
      if(*Target){
        Temp = new EXPRESSION(Ast->Line, Ast->Filename, EXPRESSION::Bit_OR);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error(Ast, "OR-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case AST::ASSIGNMENT::XOR_Assign:
      RawAssign = true;
      if(*Target){
        Temp = new EXPRESSION(Ast->Line, Ast->Filename, EXPRESSION::Bit_XOR);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error(Ast, "XOR-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case AST::ASSIGNMENT::Shift_Left_Assign:
      RawAssign = true;
      if(*Target){
        Temp = new EXPRESSION(Ast->Line, Ast->Filename, EXPRESSION::Shift_Left);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error(Ast, "Shift-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case AST::ASSIGNMENT::Shift_Right_Assign:
      RawAssign = true;
      if(*Target){
        Temp = new EXPRESSION(Ast->Line, Ast->Filename, EXPRESSION::Shift_Right);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error(Ast, "Shift-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    default:
      error("Unknown assignment type: %d", Ast->AssignmentType);
      delete Right;
      if(ScriptTarget) delete ScriptTarget;
      return false;
  }
  Simplify(*Target);

  if(*Target){
    if(RawAssign){
      if(Object->Type == BASE::TYPE::Pin || Object->Type == BASE::TYPE::Net){
        SYNTHESISABLE* Synth = (SYNTHESISABLE*)Object;
        (*Target)->Signed    = Synth->Signed;
        (*Target)->Width     = Synth->Width;
        (*Target)->FullScale = Synth->FullScale;
      }
    }else{
      if(Object->Type == BASE::TYPE::Pin || Object->Type == BASE::TYPE::Net){
        SYNTHESISABLE* Synth = (SYNTHESISABLE*)Object;
        if((*Target)->ExpressionType == EXPRESSION::Literal){
          (*Target)->Value.Div     (Synth->FullScale);
          (*Target)->Value.BinScale(Synth->Width);
          (*Target)->Signed    = Synth->Signed;
          (*Target)->Width     = Synth->Width;
          (*Target)->FullScale = Synth->FullScale;
        }else{
          if((*Target)->Signed    != Synth->Signed ||
             (*Target)->Width     != Synth->Width  ||
             (*Target)->FullScale != Synth->FullScale){
            // Inject a cast expression and let the back-end sort it out
            Temp = new EXPRESSION(Ast->Line, Ast->Filename, EXPRESSION::Cast);
            Temp->Left      = *Target;
            Temp->Signed    = Synth->Signed;
            Temp->Width     = Synth->Width;
            Temp->FullScale = Synth->FullScale;
            *Target = Temp;
          }
        }
      }
    }
  }

  if(ScriptTarget){
    switch(Object->Type){
      case BASE::TYPE::Byte:{
        auto Byte = (NETLIST::BYTE*)Object;
        if(ScriptTarget->ExpressionType == EXPRESSION::Literal){
          Byte->Value = ScriptTarget->Value.GetReal();
        }else{
          // TODO Could be an array, for instance
          // TODO Check that the RHS is non-synthesisable
          error("Not yet implemented");
        }
        break;
      }
      case BASE::TYPE::Character:{
        auto Char = (CHARACTER*)Object;
        if(ScriptTarget->ExpressionType == EXPRESSION::Literal){
          Char->Value = ScriptTarget->Value.GetReal();
        }else{
          // TODO Could be an array, for instance
          // TODO Check that the RHS is non-synthesisable
          error("Not yet implemented");
        }
        break;
      }
      case BASE::TYPE::Number:{
        auto Num = (NUM*)Object;
        if(ScriptTarget->ExpressionType == EXPRESSION::Literal){
          Num->Value = ScriptTarget->Value;
        }else{
          // TODO Could be an array, for instance
          // TODO Check that the RHS is non-synthesisable
          error("Not yet implemented");
          delete ScriptTarget;
          return false;
        }
        break;
      }
      default:
        error("Unexpected default case");
        break;
    }
    delete ScriptTarget;
  }
  return true;
}
//------------------------------------------------------------------------------

bool ENGINE::Run(AST::BASE* Ast){
  while(Ast){
    switch(Ast->Type){
      case AST::BASE::TYPE::Fence:
        error("Fence not yet implemented"); return false;
        break;

      case AST::BASE::TYPE::Import:
        if(!Import((AST::IMPORT*)Ast)) return false;
        break;

      case AST::BASE::TYPE::Group:
        if(!Group((AST::GROUP*)Ast)) return false;
        break;

      case AST::BASE::TYPE::Alias:
        if(!Alias((AST::ALIAS*)Ast)) return false;
        break;

      case AST::BASE::TYPE::TargetDefinition:
        error("TargetDefinition not yet implemented"); return false;
        break;

      case AST::BASE::TYPE::ClassDefinition:
        error("ClassDefinition not yet implemented"); return false;
        break;

      case AST::BASE::TYPE::EnumDefinition:
        error("EnumDefinition not yet implemented"); return false;
        break;

      case AST::BASE::TYPE::Definition:
        if(!Definition((AST::DEFINITION*)Ast)) return false;
        break;

      case AST::BASE::TYPE::Parameter:
        error("Parameter not yet implemented"); return false;
        break;

      case AST::BASE::TYPE::Expression:
        error("Expression not yet implemented"); return false;
        break;

      case AST::BASE::TYPE::Assignment:
        if(!Assignment((AST::ASSIGNMENT*)Ast)) return false;
        break;

      case AST::BASE::TYPE::NamespacePush:
        error("NamespacePush not yet implemented"); return false;
        break;

      case AST::BASE::TYPE::IfStatement:
        error("IfStatement not yet implemented"); return false;
        break;

      case AST::BASE::TYPE::ForLoop:
        error("ForLoop not yet implemented"); return false;
        break;

      case AST::BASE::TYPE::LoopLoop:
        error("LoopLoop not yet implemented"); return false;
        break;

      case AST::BASE::TYPE::WhileLoop:
        error("WhileLoop not yet implemented"); return false;
        break;

      case AST::BASE::TYPE::Switch:
        error("Switch not yet implemented"); return false;
        break;

      case AST::BASE::TYPE::Jump:
        error("Jump not yet implemented"); return false;
        break;

      case AST::BASE::TYPE::RTL:
        error("RTL not yet implemented"); return false;
        break;

      case AST::BASE::TYPE::FSM:
        error("FSM not yet implemented"); return false;
        break;

      case AST::BASE::TYPE::HDL:
        error("HDL not yet implemented"); return false;
        break;

      default:
        error("Unknown AST type: %d", (int)Ast->Type);
        break;
    }
    Ast = Ast->Next;
  }
  return true;
}
//------------------------------------------------------------------------------

bool ENGINE::Run(const char* Filename){
  if(NamespaceStack.empty()) NamespaceStack.push_front(&Global);

  AST::BASE* Ast = Parser.Run(Filename);
  if(Ast) AstStack.push(Ast);
  else    return false;

  return Run(Ast);
}
//------------------------------------------------------------------------------

