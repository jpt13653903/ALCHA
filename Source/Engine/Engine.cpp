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
using namespace OBJECTS;
//------------------------------------------------------------------------------

ENGINE::ENGINE(){
  error = false;
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
      Result = new EXPRESSION(EXPRESSION::String);
      Result->StrValue = *(Node->StrValue);
      break;

    case AST::EXPRESSION::Literal:
      Result = new EXPRESSION(EXPRESSION::Literal);
      Result->Value = *(Node->Value);
      break;

    case AST::EXPRESSION::Array:{
      Result = new EXPRESSION(EXPRESSION::Array);
      auto Element = (AST::EXPRESSION*)Node->Right;
      while(Element){
        Result->Elements.push_back(Evaluate(Element));
        Element = (AST::EXPRESSION*)Element->Next;
      }
      break;
    }

    case AST::EXPRESSION::Identifier:{
      auto NamespaceIterator = Stack.begin();
      while(!Result && NamespaceIterator != Stack.end()){
        MODULE* Module = *NamespaceIterator;
        while(!Result && Module){
          auto Object = Module->Symbols.find(Node->Name);
          if(Object != Module->Symbols.end()){
            if(Object->second &&
               Object->second->Type == BASE::TYPE::Alias){
              auto Alias = (ALIAS*)Object->second;
              Stack.push_front(Alias->Module);
                Result = Evaluate(Alias->Expression);
              Stack.pop_front();
            }else{
              Result = new EXPRESSION(EXPRESSION::Object);
              Result->ObjectRef = Object->second;
            }
          }
          Module = Module->Module;
        }
        NamespaceIterator++;
      }
      break;
    }

    case AST::EXPRESSION::VectorConcatenate:{
      Result = new EXPRESSION(EXPRESSION::VectorConcatenate);
      Result->Raw = true;
      auto Element = (AST::EXPRESSION*)Node->Right;
      while(Element){
        Result->Elements.push_back(Evaluate(Element));
        Element = (AST::EXPRESSION*)Element->Next;
      }
      break;
    }

    case AST::EXPRESSION::ArrayConcatenate:{
      Result = new EXPRESSION(EXPRESSION::ArrayConcatenate);
      Result->Raw = true;
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
        if(Left->ObjectRef && (
           Left->ObjectRef->Type == BASE::TYPE::Module)){
          auto Object = (MODULE*)Left->ObjectRef;
          auto Found  = Object->Symbols.find(Right->Name);
          if(Found == Object->Symbols.end()){
            Error(Node);
            printf("Object %s not found in namespace %s\n",
                   Right->Name.c_str(), Object->Name.c_str());
            delete Left;
            return 0;
          }
          if(Found->second &&
             Found->second->Type == BASE::TYPE::Alias){
            auto Alias = (ALIAS*)Found->second;
            Stack.push_front(Alias->Module);
              Result = Evaluate(Alias->Expression);
            Stack.pop_front();
          }else{
            Result = new EXPRESSION(EXPRESSION::Object);
            Result->ObjectRef = Found->second;
          }
          delete Left;
        }
      }else{
        // TODO Could be a slice expression, which is not supported yet
        error("Unimplemented attribute access expression");
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

      Result = new EXPRESSION(EXPRESSION::Attribute);

      if(Node->Left) Left = Evaluate(Node->Left);

      if(Left){
        if(Left->ExpressionType == EXPRESSION::Object){
          Result->ObjectRef = Left->ObjectRef;
          delete Left;
        }else if(Left->ExpressionType == EXPRESSION::Attribute){
          Error(Node, "Hierarchical attributes not supported");
          delete Left;
          return 0;
        }else{
          // TODO Could be a slice expression, which is not supported yet
          error("Unimplemented attribute access expression");
          delete Left;
          return 0;
        }
      }else{ // Attribute of the current namespace
        Result->ObjectRef = Stack.front();
      }
      if(Node->Right && Node->Right->Type == AST::BASE::TYPE::Expression){
        auto Right = (AST::EXPRESSION*)Node->Right;
        if(Right->ExpressionType == AST::EXPRESSION::Identifier){
          Result->Name = Right->Name;
        }else{
          // TODO Could be a slice expression, which is not supported yet
          error("Unimplemented attribute access expression");
        }
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
      error("Factorial not yet implemented");
      break;

    case AST::EXPRESSION::Negate:
      Result = new EXPRESSION(EXPRESSION::Negate);
      if(Node->Right && Node->Right->Type == AST::BASE::TYPE::Expression)
        Result->Right = Evaluate((AST::EXPRESSION*)Node->Right);
      if(Result->Right && Result->Right->ExpressionType == EXPRESSION::Literal){
        Result->ExpressionType = EXPRESSION::Literal;
        Result->Value = Result->Right->Value;
        Result->Value.Mul(-1);
        delete Result->Right; Result->Right = 0;
      }
      break;

    case AST::EXPRESSION::Bit_NOT:
      error("Bit_NOT not yet implemented");
      break;

    case AST::EXPRESSION::Raw:
      error("Raw not yet implemented");
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
      error("Cast not yet implemented");
      break;

    case AST::EXPRESSION::Replicate:
      error("Replicate not yet implemented");
      break;

    case AST::EXPRESSION::Multiply:
      Result = new EXPRESSION(EXPRESSION::Multiply);
      if(Node->Left) Result->Left = Evaluate(Node->Left);
      if(Node->Right && Node->Right->Type == AST::BASE::TYPE::Expression){
        Result->Right = Evaluate((AST::EXPRESSION*)Node->Right);
      }
      if(
        Result->Left  &&
        Result->Right &&
        Result->Left ->ExpressionType == EXPRESSION::Literal &&
        Result->Right->ExpressionType == EXPRESSION::Literal
      ){
        // TODO: Question -- does "Raw" propagate up the tree?
        Result->ExpressionType = EXPRESSION::Literal;
        Result->Value = Result->Left->Value;
        Result->Value.Mul(Result->Right->Value);
        delete Result->Left ; Result->Left  = 0;
        delete Result->Right; Result->Right = 0;
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
      error("Conditional not yet implemented");
      break;

    default:
      error("Unknown expression type: %d", Node->ExpressionType);
      break;
  }
  return Result;
}
//------------------------------------------------------------------------------

bool ENGINE::ApplyAttributes(
  BASE*       Object,
  std::string&         Name,
  EXPRESSION* Value,
  AST::BASE*           Ast
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
    Object->Signed     = true;
  }
  return true;
}
//------------------------------------------------------------------------------

bool ENGINE::Import(AST::IMPORT* Ast){
  string Filename;
  bool   OwnNamespace = !Ast->Namespace.empty();

  if(OwnNamespace){
    auto Found = Stack.front()->Symbols.find(Ast->Namespace);
    if(Found != Stack.front()->Symbols.end()){
      Error(Ast);
      printf("Symbol \"%s\" already exists in the current namespace\n",
             Ast->Namespace.c_str());
      return false;
    }
    auto Module = new MODULE(Ast->Namespace.c_str());
    Stack.front()->Symbols[Ast->Namespace] = Module;
    Stack.push_front(Module);
  }

  string& Path = Ast->Filename;
  int n;
  for(n = Path.length()-1; n >= 0; n--){
    if(Path[n] == '/') break;
  }
  if(n >= 0) Filename = Path.substr(0, n+1);
  Filename += Ast->File;
  Filename += ".alc";
  debug("Filename = %s", Filename.c_str());

  bool Result = Run(Filename.c_str());

  if(OwnNamespace){
    Stack.pop_front();
  }
  return Result;
}
//------------------------------------------------------------------------------

bool ENGINE::Alias(AST::ALIAS* Ast){
  auto Symbol = Stack.front()->Symbols.find(Ast->Identifier);
  if(Symbol != Stack.front()->Symbols.end()){
    Error(Ast);
    printf("Symbol \"%s\" already defined in the current namespace\n",
           Ast->Identifier.c_str());
    return false;
  }

  auto Object = new ALIAS(Ast->Identifier.c_str(), Ast->Expression);

  Stack.front()->Symbols[Object->Name] = Object;

  return true;
}
//------------------------------------------------------------------------------

bool ENGINE::Definition(AST::DEFINITION* Ast){
  auto Identifier = Ast->Identifiers;

  while(Identifier){
    auto Symbol = Stack.front()->Symbols.find(Identifier->Identifier);
    if(Symbol != Stack.front()->Symbols.end()){
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
        auto Pin = new PIN(Identifier->Identifier.c_str());
        Pin->Direction = Ast->Direction;
        if(!ApplyParameters(Pin, Ast->Parameters)) Error(Ast, "Invalid parameters");
        if(!ApplyAttributes(Pin, Ast->Attributes)) Error(Ast, "Invalid attributes");
        if(Identifier->Initialiser) error("Not yet implemented");

        Stack.front()->Symbols[Pin->Name] = Pin;
        break;
      }

      case AST::DEFINITION::Net:
        error("Not yet implemented");
        break;

      case AST::DEFINITION::Byte:
        error("Not yet implemented");
        break;

      case AST::DEFINITION::Char:
        error("Not yet implemented");
        break;

      case AST::DEFINITION::Num:
        error("Not yet implemented");
        break;

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

bool ENGINE::Assignment(AST::ASSIGNMENT* Ast){
  EXPRESSION* Left  = Evaluate(Ast->Left );
  EXPRESSION* Right = Evaluate(Ast->Right);
  EXPRESSION* Temp;

  if(!Left){
    Error(Ast, "Target object not defined");
    if(Left ) delete Left;
    if(Right) delete Right;
    return false;
  }
  if(!Right){
    error("Null assignment expression");
    if(Left ) delete Left;
    if(Right) delete Right;
    return false;
  }

  // The evaluation simplifies the tree as far as possible
  if(
    !Left->ObjectRef ||
    (
      Left->ExpressionType != EXPRESSION::Object &&
      Left->ExpressionType != EXPRESSION::Attribute
    )
  ){
    Error(Ast, "Illegal assignment target");
    delete Left;
    delete Right;
    return false;
  }

  switch(Ast->AssignmentType){
    case AST::ASSIGNMENT::Assign:
      // Use Right without modification
      break;

    case AST::ASSIGNMENT::Raw_Assign:
      Right->Raw = true;
      break;

    case AST::ASSIGNMENT::Append_Assign:
      error("Append_Assign not yet implemented");
      // TODO All these are similar:
      //   1. Get a reference to the target expression (new function)
      //   2. Make a new node with the appropriate action
      //   3. Temp->Left  = The current target expression
      //   4. Temp->Right = Right
      //   5. Right = Temp;
      //   6. Make the current target expression null so that it's not deleted
      break;

    case AST::ASSIGNMENT::Add_Assign:
      error("Add_Assign not yet implemented");
      break;

    case AST::ASSIGNMENT::Subtract_Assign:
      error("Subtract_Assign not yet implemented");
      break;

    case AST::ASSIGNMENT::Multiply_Assign:
      error("Multiply_Assign not yet implemented");
      break;

    case AST::ASSIGNMENT::Divide_Assign:
      error("Divide_Assign not yet implemented");
      break;

    case AST::ASSIGNMENT::Modulus_Assign:
      error("Modulus_Assign not yet implemented");
      break;

    case AST::ASSIGNMENT::Exponential_Assign:
      error("Exponential_Assign not yet implemented");
      break;

    case AST::ASSIGNMENT::AND_Assign:
      error("AND_Assign not yet implemented");
      break;

    case AST::ASSIGNMENT::OR_Assign:
      error("OR_Assign not yet implemented");
      break;

    case AST::ASSIGNMENT::XOR_Assign:
      error("XOR_Assign not yet implemented");
      break;

    case AST::ASSIGNMENT::Shift_Left_Assign:
      error("Shift_Left_Assign not yet implemented");
      break;

    case AST::ASSIGNMENT::Shift_Right_Assign:
      error("Shift_Right_Assign not yet implemented");
      break;

    default:
      error("Unknown assignment type: %d", Ast->AssignmentType);
      delete Left;
      delete Right;
      return false;
  }

  if(Left->ExpressionType == EXPRESSION::Attribute){
    if(Left->ObjectRef){
      ApplyAttributes(Left->ObjectRef, Left->Name, Right, Ast);
      // The function above deletes Right if required
    }else{
      error("Null object reference");
      delete Right;
    }
    delete Left;
    return true;

  }else{ // Object target
    auto Object = Left->ObjectRef;

    switch(Object->Type){
      case BASE::TYPE::Pin:{
        auto Pin = (PIN*)Object;
        if(Pin->Driver) delete Pin->Driver;
        Pin->Driver = Right;
        break;
      }

      default:
        error("Unimplemented target type: %d", (int)Object->Type);
        delete Left;
        delete Right;
        return false;
    }
  }

  return true;
}
//------------------------------------------------------------------------------

bool ENGINE::Run(const char* Filename){
  if(Stack.empty()) Stack.push_front(&Global);

  AST::BASE* Ast = Parser.Run(Filename);
  if(Ast) AstStack.push(Ast);
  else    return false;

  while(Ast){
    switch(Ast->Type){
      case AST::BASE::TYPE::Fence:
        error("Fence not yet implemented");
        break;

      case AST::BASE::TYPE::Import:
        if(!Import((AST::IMPORT*)Ast)) return false;
        break;

      case AST::BASE::TYPE::Group:
        error("Group not yet implemented");
        break;

      case AST::BASE::TYPE::Alias:
        if(!Alias((AST::ALIAS*)Ast)) return false;
        break;

      case AST::BASE::TYPE::TargetDefinition:
        error("TargetDefinition not yet implemented");
        break;

      case AST::BASE::TYPE::ClassDefinition:
        error("ClassDefinition not yet implemented");
        break;

      case AST::BASE::TYPE::EnumDefinition:
        error("EnumDefinition not yet implemented");
        break;

      case AST::BASE::TYPE::Definition:
        if(!Definition((AST::DEFINITION*)Ast)) return false;
        break;

      case AST::BASE::TYPE::Parameter:
        error("Parameter not yet implemented");
        break;

      case AST::BASE::TYPE::Expression:
        error("Expression not yet implemented");
        break;

      case AST::BASE::TYPE::Assignment:
        if(!Assignment((AST::ASSIGNMENT*)Ast)) return false;
        break;

      case AST::BASE::TYPE::NamespacePush:
        error("NamespacePush not yet implemented");
        break;

      case AST::BASE::TYPE::IfStatement:
        error("IfStatement not yet implemented");
        break;

      case AST::BASE::TYPE::ForLoop:
        error("ForLoop not yet implemented");
        break;

      case AST::BASE::TYPE::LoopLoop:
        error("LoopLoop not yet implemented");
        break;

      case AST::BASE::TYPE::WhileLoop:
        error("WhileLoop not yet implemented");
        break;

      case AST::BASE::TYPE::Switch:
        error("Switch not yet implemented");
        break;

      case AST::BASE::TYPE::Jump:
        error("Jump not yet implemented");
        break;

      case AST::BASE::TYPE::RTL:
        error("RTL not yet implemented");
        break;

      case AST::BASE::TYPE::FSM:
        error("FSM not yet implemented");
        break;

      case AST::BASE::TYPE::HDL:
        error("HDL not yet implemented");
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

