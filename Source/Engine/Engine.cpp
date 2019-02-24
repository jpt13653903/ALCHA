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
//------------------------------------------------------------------------------

ENGINE::ENGINE(){
}
//------------------------------------------------------------------------------

ENGINE::~ENGINE(){
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

OBJECTS::EXPRESSION* ENGINE::Evaluate(AST::EXPRESSION* Node){
  if(!Node) return 0;

  OBJECTS::EXPRESSION* Result = 0;

  switch(Node->ExpressionType){
    case AST::EXPRESSION::String:
      Result = new OBJECTS::EXPRESSION(OBJECTS::EXPRESSION::String);
      Result->StrValue = *(Node->StrValue);
      break;

    case AST::EXPRESSION::Literal:
      Result = new OBJECTS::EXPRESSION(OBJECTS::EXPRESSION::Literal);
      Result->Value = *(Node->Value);
      break;

    case AST::EXPRESSION::Array:{
      Result = new OBJECTS::EXPRESSION(OBJECTS::EXPRESSION::Array);
      auto Element = (AST::EXPRESSION*)Node->Right;
      while(Element){
        Result->Elements.push_back(Evaluate(Element));
        Element = (AST::EXPRESSION*)Element->Next;
      }
      break;
    }

    case AST::EXPRESSION::Identifier:{
      auto Object = OBJECTS::Current->Symbols.find(Node->Name);
      if(Object != OBJECTS::Current->Symbols.end()){
        Result = new OBJECTS::EXPRESSION(OBJECTS::EXPRESSION::Object);
        Result->ObjectRef = Object->second;
      }else{
        error("Scope look-up not yet implemented");
        // TODO Remember to first look in the stack, then try the namespace
        //      tree from the current leaf upwards toward the root
      }
      break;
    }

    case AST::EXPRESSION::VectorConcatenate:{
      Result = new OBJECTS::EXPRESSION(OBJECTS::EXPRESSION::VectorConcatenate);
      Result->Raw = true;
      auto Element = (AST::EXPRESSION*)Node->Right;
      while(Element){
        Result->Elements.push_back(Evaluate(Element));
        Element = (AST::EXPRESSION*)Element->Next;
      }
      break;
    }

    case AST::EXPRESSION::ArrayConcatenate:{
      Result = new OBJECTS::EXPRESSION(OBJECTS::EXPRESSION::ArrayConcatenate);
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

    case AST::EXPRESSION::AccessMember:
      error("AccessMember not yet implemented");
      break;

    case AST::EXPRESSION::AccessMemberSafe:
      error("AccessMemberSafe not yet implemented");
      break;

    case AST::EXPRESSION::AccessAttribute:
      error("AccessAttribute not yet implemented");
      break;

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
      Result = new OBJECTS::EXPRESSION(OBJECTS::EXPRESSION::Negate);
      if(Node->Right && Node->Right->Type == AST::BASE::TYPE::Expression)
        Result->Right = Evaluate((AST::EXPRESSION*)Node->Right);
      if(Result->Right && Result->Right->ExpressionType == OBJECTS::EXPRESSION::Literal){
        Result->ExpressionType = OBJECTS::EXPRESSION::Literal;
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
      Result = new OBJECTS::EXPRESSION(OBJECTS::EXPRESSION::Multiply);
      if(Node->Left ) Result->Left  = Evaluate(Node->Left );
      if(Node->Right && Node->Right->Type == AST::BASE::TYPE::Expression){
        Result->Right = Evaluate((AST::EXPRESSION*)Node->Right);
      }
      if(
        Result->Left  &&
        Result->Right &&
        Result->Left ->ExpressionType == OBJECTS::EXPRESSION::Literal &&
        Result->Right->ExpressionType == OBJECTS::EXPRESSION::Literal
      ){
        // TODO: Question -- does "Raw" propagate up the tree?
        Result->ExpressionType = OBJECTS::EXPRESSION::Literal;
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

bool ENGINE::ApplyAttributes(OBJECTS::BASE* Object, AST::ASSIGNMENT* AttributeList){
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

    OBJECTS::EXPRESSION* Value = Evaluate(AttributeList->Right);
    if(!Value){
      Error(AttributeList, "Invalid attribute expression");
      return false;
    }

    switch(Value->ExpressionType){
      case OBJECTS::EXPRESSION::String:
      case OBJECTS::EXPRESSION::Literal:
        break;

      case OBJECTS::EXPRESSION::Array:
        // TODO Make sure that the array only contains strings or literals
        break;

      default:
        Error(AttributeList, "Attribute values must be strings, literals or arrays");
        delete Value;
        return false;
    }
    Object->Attributes[AttributeList->Left->Name] = Value;

    AttributeList = (AST::ASSIGNMENT*)AttributeList->Next;
  }
  return true;
}
//------------------------------------------------------------------------------

bool ENGINE::ApplyParameters(OBJECTS::SYNTHESISABLE* Object, AST::BASE* Parameter){
  int  Position          = 0; // Negative => named parameters
  bool ExplicitFullScale = false;

  while(Parameter){
    switch(Parameter->Type){
      case AST::BASE::TYPE::Expression:{
        if(Position < 0) return false; // Mixing named and positional parameters

        OBJECTS::EXPRESSION* Param = Evaluate((AST::EXPRESSION*)Parameter);
        if(!Param){
          Error(Parameter, "Invalid parameter expression");
          return false;
        }

        switch(Param->ExpressionType){
          case OBJECTS::EXPRESSION::Literal:
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

bool ENGINE::Definition(AST::DEFINITION* Ast){
  auto Identifier = Ast->Identifiers;

  while(Identifier){
    auto Object = OBJECTS::Current->Symbols.find(Identifier->Identifier);
    if(Object != OBJECTS::Current->Symbols.end()){
      Error(Ast);
      printf("Object \"%s\" already defined in the current namespace\n",
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
        auto Pin = new OBJECTS::PIN(Identifier->Identifier.c_str());
        Pin->Direction = Ast->Direction;
        if(!ApplyParameters(Pin, Ast->Parameters)) Error(Ast, "Invalid parameters");
        if(!ApplyAttributes(Pin, Ast->Attributes)) Error(Ast, "Invalid attributes");
        if(Identifier->Initialiser) error("Not yet implemented");

        OBJECTS::Current->Symbols[Pin->Name] = Pin;
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
  OBJECTS::EXPRESSION* Left  = Evaluate(Ast->Left );
  OBJECTS::EXPRESSION* Right = Evaluate(Ast->Right);
  OBJECTS::EXPRESSION* Temp;

  if(!Left){
    error("Null assignment target");
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
      Left->ExpressionType != OBJECTS::EXPRESSION::Object &&
      Left->ExpressionType != OBJECTS::EXPRESSION::Attribute
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

  if(Left->ExpressionType == OBJECTS::EXPRESSION::Attribute){
    error("Not yet implemented");

  }else{ // Object target
    auto Object = Left->ObjectRef;

    switch(Object->Type){
      case OBJECTS::BASE::TYPE::Pin:{
        auto Pin = (OBJECTS::PIN*)Object;
        if(Pin->Driver) delete Pin->Driver;
        Pin->Driver = Right;
        break;
      }

      default:
        error("Unimplemented target type: %d", Object->Type);
        delete Left;
        delete Right;
        return true;
    }
  }

  return true;
}
//------------------------------------------------------------------------------

bool ENGINE::Run(){
  debug("Running the engine...");

  AST::BASE* Ast = AST::Root;

  error = false;

  while(Ast){
    switch(Ast->Type){
      case AST::BASE::TYPE::Fence:
        error("Fence not yet implemented");
        break;

      case AST::BASE::TYPE::Import:
        error("Import not yet implemented");
        break;

      case AST::BASE::TYPE::Group:
        error("Group not yet implemented");
        break;

      case AST::BASE::TYPE::Alias:
        error("Alias not yet implemented");
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
        error("Unknown AST type: %d", Ast->Type);
        break;
    }
    Ast = Ast->Next;
  }
  return true;
}
//------------------------------------------------------------------------------

