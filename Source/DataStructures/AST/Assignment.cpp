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

#include "Assignment.h"
#include "Number.h"

#include "Netlist/Alias.h"
#include "Netlist/Byte.h"
#include "Netlist/Character.h"
#include "Netlist/Pin.h"
#include "Netlist/Net.h"
#include "Netlist/Num.h"
#include "Netlist/Module.h"

#include "Expression/Object.h"
#include "Expression/Literal.h"
#include "Expression/ArrayConcatenate.h"
#include "Expression/Add.h"
#include "Expression/Subtract.h"
#include "Expression/Multiply.h"
#include "Expression/Divide.h"
#include "Expression/Modulus.h"
#include "Expression/Exponential.h"
#include "Expression/Bit_AND.h"
#include "Expression/Bit_OR.h"
#include "Expression/Bit_XOR.h"
#include "Expression/Shift_Left.h"
#include "Expression/Shift_Right.h"
#include "Expression/Identifier.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

ASSIGNMENT::ASSIGNMENT(
  int             Line,
  std::string&    Filename,
  ASSIGNMENT_TYPE AssignmentType
): ASSIGNMENT(Line, Filename.c_str(), AssignmentType){}
//------------------------------------------------------------------------------

ASSIGNMENT::ASSIGNMENT(
  int             Line,
  const char*     Filename,
  ASSIGNMENT_TYPE AssignmentType
): BASE(Line, Filename, TYPE::Assignment){
  this->AssignmentType = AssignmentType;

  Fence = false;
  Left  = Right = 0;
}
//------------------------------------------------------------------------------

ASSIGNMENT::~ASSIGNMENT(){
  if(Left ) delete Left;
  if(Right) delete Right;
}
//------------------------------------------------------------------------------

BASE* ASSIGNMENT::Copy(bool CopyNext){
  ASSIGNMENT* Copy = new ASSIGNMENT(Line, Filename.c_str(), AssignmentType);

  Copy->Fence = Fence;

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool ASSIGNMENT::GetLHS_Object(NETLIST::BASE* Object, target_list& List, BASE* Ast){
  TARGET_LIST ListNode;

  bool Result = false;

  if(Object){
    switch(Object->Type){
      case NETLIST::BASE::TYPE::Pin:{
        auto Pin = (NETLIST::PIN*)Object;
        if(Pin->Direction == DEFINITION::DIRECTION::Input){
          Ast->Error("Cannot assign to an input pin");
          return false;
        }
        Pin->Used = true;
        ListNode.Object     =  Pin;
        ListNode.Expression = &Pin->Driver;
        List.push_back(ListNode);
        Result = true;
        break;
      }
      case NETLIST::BASE::TYPE::Net:{
        auto Net = (NETLIST::NET*)Object;
        ListNode.Object     =  Net;
        ListNode.Expression = &Net->Value;
        List.push_back(ListNode);
        Result = true;
        break;
      }
      case NETLIST::BASE::TYPE::Number:
      case NETLIST::BASE::TYPE::Byte:
      case NETLIST::BASE::TYPE::Character:{
        ListNode.Object = Object;
        ListNode.Expression = 0;
        List.push_back(ListNode);
        Result = true;
        break;
      }
      case NETLIST::BASE::TYPE::Alias:{
        auto Alias = (NETLIST::ALIAS*)Object;
        NETLIST::NamespaceStack.push_front(Alias->Namespace);
          Result = GetLHS(Alias->Expression, List);
        NETLIST::NamespaceStack.pop_front();
        break;
      }
      case NETLIST::BASE::TYPE::Array:{
        error("Array assignment not yet implemented");
        break;
      }
      case NETLIST::BASE::TYPE::Module:
      case NETLIST::BASE::TYPE::Group:{
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

bool ASSIGNMENT::GetLHS(EXPRESSION* Node, target_list& List){
  if(!Node) return 0;

  bool        Result = false;
  TARGET_LIST ListNode;

  switch(Node->Type){
    case TYPE::Array:{
      error("Array not yet implemented");
      // Idea: Simply call GetLHS recursively for each array element
      //
      // auto Element = (EXPRESSION*)Node->Right;
      // while(Element){
      //   Result->Elements.push_back(Evaluate(Element));
      //   Element = (EXPRESSION*)Element->Next;
      // }
      break;
    }

    case TYPE::Identifier:{
      auto Identifier = (IDENTIFIER*)Node;
      auto NamespaceIterator = NETLIST::NamespaceStack.begin();
      while(!Result && NamespaceIterator != NETLIST::NamespaceStack.end()){
        NETLIST::NAMESPACE* Namespace = *NamespaceIterator;
        while(!Result && Namespace){
          auto Object = Namespace->Symbols.find(Identifier->Name);
          if(Object != Namespace->Symbols.end()){
            Result = GetLHS_Object(Object->second, List, Identifier);
          }
          Namespace = Namespace->Namespace;
        }
        NamespaceIterator++;
      }
      break;
    }

    case TYPE::VectorConcatenate:{
      error("VectorConcatenate not yet implemented");
      // Result = (EXPRESSION*)Node->Copy(true);
      break;
    }

    case TYPE::ArrayConcatenate:{
      error("ArrayConcatenate not yet implemented");
      // Result = (EXPRESSION*)Node->Copy(true);
      break;
    }

    case TYPE::Slice:
      error("Slice not yet implemented");
      break;

    case TYPE::AccessMember:{
      if(!Node->Left || !Node->Right || Node->Right->Type <= TYPE::Expression){
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
      auto Right = (EXPRESSION*)Node->Right;

      if(Left.isAttribute){
        Node->Error("Attributes cannot be structures");
        return false;
      }

      switch(Left.Object->Type){
        case NETLIST::BASE::TYPE::Pin:{
          auto Pin = (NETLIST::PIN*)Left.Object;
          ListNode.Object = Pin;
          if(Right->Type != TYPE::Identifier){
            Node->Error("Invalid pin member");
            return false;
          }
          if(Pin->Direction == DEFINITION::DIRECTION::Input){
            Node->Error("Cannot assign to an input pin.");
            return false;
          }
          if(((IDENTIFIER*)Right)->Name == "driver"){
            ListNode.Expression = &Pin->Driver;
          }else if(((IDENTIFIER*)Right)->Name == "enabled"){
            ListNode.Expression = &Pin->Enabled;
          }else{
            Node->Error("Valid pin members are \"driver\" and \"enabled\" only");
            return false;
          }
          List.push_back(ListNode);
          Result = true;
          break;
        }
        case NETLIST::BASE::TYPE::Module:
        case NETLIST::BASE::TYPE::Group:{
          assert(Right->Type == TYPE::Identifier, return false);
          auto Namespace = (NETLIST::NAMESPACE*)Left.Object;
          auto Object    = Namespace->Symbols.find(((IDENTIFIER*)Right)->Name);
          if(Object == Namespace->Symbols.end()){
            Node->Error();
            printf("Object %s not found in namespace %s\n",
                   ((IDENTIFIER*)Right)->Name.c_str(), Namespace->Name.c_str());
            return false;
          }
          Result = GetLHS_Object(Object->second, List, Node);
          break;
        }
        case NETLIST::BASE::TYPE::Array:{
          error("Array not yet implemented");
          return false;
        }
        default:
          Node->Error();
          printf("Invalid type for member access: %d\n", (int)Left.Object->Type);
          return false;
      }
      break;
    }

    case TYPE::AccessMemberSafe:
      error("AccessMemberSafe not yet implemented");
      break;

    case TYPE::AccessAttribute:{
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
        ListNode.Object = NETLIST::NamespaceStack.front();
      }
      if(ListNode.isAttribute){
        Node->Error("Attributes are not hierarchical");
        return false;
      }

      if(Node->Right && Node->Right->Type > TYPE::Expression){
        auto Right = (EXPRESSION*)Node->Right;
        if(Right->Type == TYPE::Identifier){
          // The process of adding an entry initialises the pointer to null.
          // The default constructor of the pointer type is called.
          ListNode.Expression = &ListNode.Object->Attributes[((IDENTIFIER*)Right)->Name];
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
      Node->Error("Invalid LHS expression");
      break;
  }
  return Result;
}
//------------------------------------------------------------------------------

bool ASSIGNMENT::RunScripting(){
  target_list Left;
  EXPRESSION* Right;
  EXPRESSION* Temp;

  if(!GetLHS(this->Left, Left)) return false;
  if(Left.empty()){
    Error("Target object list is empty");
    return false;
  }

  if(Left.size() > 1){
    error("Multiple assignment targets not supported yet");
    return false;
  }

  Right = this->Right->Evaluate();
  if(!Right){
    // This is ok -- generally caused by a syntax or semantic error, but should
    // halt further compilation anyway
    return false;
  }

  NETLIST::BASE* Object = Left.front().Object;
  if(!Object){
    error("Unexpected null reference");
    return false;
  }

  EXPRESSION*  ScriptTarget = 0;
  EXPRESSION** Target = Left.front().Expression;
  if(!Target){
    switch(Object->Type){
      case NETLIST::BASE::TYPE::Byte:
      case NETLIST::BASE::TYPE::Character:
      case NETLIST::BASE::TYPE::Number:{
        ScriptTarget = new OBJECT(Line, Filename);
        ((OBJECT*)ScriptTarget)->ObjectRef = Object;
        Target = &ScriptTarget;
        break;
      }

      default:
        error("Unexpected null reference");
        return false;
    }
  }

  bool RawAssign = true;

  switch(AssignmentType){
    case ASSIGNMENT_TYPE::Assign:
      RawAssign = false;
      if(*Target) delete *Target;
      *Target = Right;
      break;

    case ASSIGNMENT_TYPE::Raw_Assign:
      RawAssign = true;
      if(*Target) delete *Target;
      *Target = Right;
      break;

    case ASSIGNMENT_TYPE::Append_Assign:
      RawAssign = false;
      if(*Target){
        Temp = new ARRAYCONCATENATE(Line, Filename);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error("Append-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case ASSIGNMENT_TYPE::Add_Assign:
      RawAssign = false;
      if(*Target){
        Temp = new ADD(Line, Filename);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error("Add-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case ASSIGNMENT_TYPE::Subtract_Assign:
      RawAssign = false;
      if(*Target){
        Temp = new SUBTRACT(Line, Filename);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error("Subtract-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case ASSIGNMENT_TYPE::Multiply_Assign:
      RawAssign = false;
      if(*Target){
        Temp = new MULTIPLY(Line, Filename);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target = Temp;
        break;
      }else{
        Error("Multiply-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case ASSIGNMENT_TYPE::Divide_Assign:
      RawAssign = false;
      if(*Target){
        Temp = new DIVIDE(Line, Filename);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error("Divide-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case ASSIGNMENT_TYPE::Modulus_Assign:
      RawAssign = false;
      if(*Target){
        Temp = new MODULUS(Line, Filename);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error("Modulus-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case ASSIGNMENT_TYPE::Exponential_Assign:
      RawAssign = false;
      if(*Target){
        Temp = new EXPONENTIAL(Line, Filename);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error("Exponential-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case ASSIGNMENT_TYPE::AND_Assign:
      RawAssign = true;
      if(*Target){
        Temp = new BIT_AND(Line, Filename);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error("AND-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case ASSIGNMENT_TYPE::OR_Assign:
      RawAssign = true;
      if(*Target){
        Temp = new BIT_OR(Line, Filename);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error("OR-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case ASSIGNMENT_TYPE::XOR_Assign:
      RawAssign = true;
      if(*Target){
        Temp = new BIT_XOR(Line, Filename);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error("XOR-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case ASSIGNMENT_TYPE::Shift_Left_Assign:
      RawAssign = true;
      if(*Target){
        Temp = new SHIFT_LEFT(Line, Filename);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error("Shift-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    case ASSIGNMENT_TYPE::Shift_Right_Assign:
      RawAssign = true;
      if(*Target){
        Temp = new SHIFT_RIGHT(Line, Filename);
        Temp->Left  = *Target;
        Temp->Right = Right;
        *Target     = Temp;
        break;
      }else{
        Error("Shift-Assign with null target");
        delete Right;
        if(ScriptTarget) delete ScriptTarget;
        return false;
      }

    default:
      error("Unknown assignment type: %d", (int)AssignmentType);
      delete Right;
      if(ScriptTarget) delete ScriptTarget;
      return false;
  }
  *Target = (*Target)->Simplify(false);

  if(*Target){
    if(!RawAssign){
      if(Object->Type == NETLIST::BASE::TYPE::Pin ||
         Object->Type == NETLIST::BASE::TYPE::Net
      ){
        NETLIST::SYNTHESISABLE* Synth = (NETLIST::SYNTHESISABLE*)Object;
        if((*Target)->Type == TYPE::Literal){
          auto Literal = (LITERAL*)(*Target);
          Literal->Value.Div     (Synth->FullScale);
          Literal->Value.BinScale(Synth->Width);
        }
      }
    }
  }

  if(ScriptTarget){
    switch(Object->Type){
      case NETLIST::BASE::TYPE::Byte:{
        auto Byte = (NETLIST::BYTE*)Object;
        if(ScriptTarget->Type == TYPE::Literal){
          Byte->Value = ((LITERAL*)ScriptTarget)->Value.GetReal();
        }else{
          // TODO Could be an array, for instance
          // TODO Check that the RHS is non-synthesisable
          error("Not yet implemented");
        }
        break;
      }
      case NETLIST::BASE::TYPE::Character:{
        auto Char = (NETLIST::CHARACTER*)Object;
        if(ScriptTarget->Type == TYPE::Literal){
          Char->Value = ((LITERAL*)ScriptTarget)->Value.GetReal();
        }else{
          // TODO Could be an array, for instance
          // TODO Check that the RHS is non-synthesisable
          error("Not yet implemented");
        }
        break;
      }
      case NETLIST::BASE::TYPE::Number:{
        auto Num = (NETLIST::NUM*)Object;
        if(ScriptTarget->Type == TYPE::Literal){
          Num->Value = ((LITERAL*)ScriptTarget)->Value;
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

bool ASSIGNMENT::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void ASSIGNMENT::Display(){
  DisplayInfo();
  Debug.print("Assignment: ");

  if(Left){
    if(Left->Left || Left->Right) Debug.print("(");
    Left->Display();
    if(Left->Left || Left->Right) Debug.print(")");
  }

  switch(AssignmentType){
    case ASSIGNMENT_TYPE::Assign            : Debug.print(  " = "); break;
    case ASSIGNMENT_TYPE::Raw_Assign        : Debug.print( " := "); break;
    case ASSIGNMENT_TYPE::Append_Assign     : Debug.print( " ~= "); break;
    case ASSIGNMENT_TYPE::Add_Assign        : Debug.print( " += "); break;
    case ASSIGNMENT_TYPE::Subtract_Assign   : Debug.print( " -= "); break;
    case ASSIGNMENT_TYPE::Multiply_Assign   : Debug.print( " *= "); break;
    case ASSIGNMENT_TYPE::Divide_Assign     : Debug.print( " /= "); break;
    case ASSIGNMENT_TYPE::Modulus_Assign    : Debug.print(" %%= "); break;
    case ASSIGNMENT_TYPE::Exponential_Assign: Debug.print( "**= "); break;
    case ASSIGNMENT_TYPE::AND_Assign        : Debug.print( " &= "); break;
    case ASSIGNMENT_TYPE::OR_Assign         : Debug.print( " |= "); break;
    case ASSIGNMENT_TYPE::XOR_Assign        : Debug.print( " ^= "); break;
    case ASSIGNMENT_TYPE::Shift_Left_Assign : Debug.print(" <<= "); break;
    case ASSIGNMENT_TYPE::Shift_Right_Assign: Debug.print(" >>= "); break;

    default: Debug.print("(Unknown assignment type)");
  }

  if(Right){
    if(Right->Left || Right->Right) Debug.print("(");
    Right->Display();
    if(Right->Left || Right->Right) Debug.print(")");
  }

  if(Fence) Debug.print("{Fence}");

  Debug.print("\n");
  if(Next) Next->Display();
}
//------------------------------------------------------------------------------
