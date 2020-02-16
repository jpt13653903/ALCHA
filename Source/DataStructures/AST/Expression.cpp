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

#include "Expression.h"
#include "Assignment.h"
#include "Netlist/Base.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

EXPRESSION::EXPRESSION(
  int           Line,
  const string& Filename,
  TYPE          ExpressionType
): EXPRESSION(Line, Filename.c_str(), ExpressionType){}
//------------------------------------------------------------------------------

EXPRESSION::EXPRESSION(
  int         Line,
  const char* Filename,
  TYPE        ExpressionType
): BASE(Line, Filename, ExpressionType){
  ObjectRef = 0;
  Left      = 0;
  Right     = 0;
}
//------------------------------------------------------------------------------

EXPRESSION::~EXPRESSION(){
  // Don't delete the Object reference -- it's part of the namespace tree

  if(Left ) delete Left;
  if(Right) delete Right;
}
//------------------------------------------------------------------------------

BASE* EXPRESSION::Element(int n){
  int e = 0;

  BASE* Node = Right;
  while(Node){
    if(e == n) return Node;
    e++;
    Node = Node->Next;
  }
  return 0;
}
//------------------------------------------------------------------------------

int EXPRESSION::ElementCount(){
  int n = 0;

  BASE* Node = Right;
  while(Node){
    n++;
    Node = Node->Next;
  }
  return n;
}
//------------------------------------------------------------------------------

BASE* EXPRESSION::Copy(bool CopyNext){
  EXPRESSION* Copy = new EXPRESSION(Line, Filename.c_str(), Type);

  Copy->Name      = Name;
  Copy->Value     = Value;
  Copy->StrValue  = StrValue;
  Copy->ObjectRef = ObjectRef;

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

void EXPRESSION::Display(){
  if(Left){
    if(Left->Left || Left->Right) Debug.print("(");
    Left->Display();
    if(Left->Left || Left->Right) Debug.print(")");
  }

  switch(Type){
    case TYPE::String:
      Debug.print("\"%s\"", StrValue.c_str());
      break;

    case TYPE::Literal:
      Debug.print(Value.Display());
      break;

    case TYPE::Identifier:
      if(Name.empty()) error ("(Identifier node has no name)");
      else             Debug.print("%s", Name.c_str());
      break;

    case TYPE::Object:
      if(ObjectRef){
        // TODO: Display the width here, because it should be a property
        //       of the target object, not the expression itself
        ObjectRef->DisplayLongName();
      }else{
        error("Null object reference");
      }
      break;

    case TYPE::Array            : Debug.print("{Array}"       ); break;
    case TYPE::VectorConcatenate: Debug.print("{VectorConcat}"); break;
    case TYPE::ArrayConcatenate : Debug.print("{ArrayConcat}" ); break;

    case TYPE::FunctionCall: Debug.print("{call}" ); break;

    case TYPE::Slice: Debug.print("{slice}"); break;

    case TYPE::AccessMember:     Debug.print("." ); break;
    case TYPE::AccessMemberSafe: Debug.print("?." ); break;
    case TYPE::AccessAttribute:  Debug.print("'" ); break;

    case TYPE::Increment: Debug.print("++"); break;
    case TYPE::Decrement: Debug.print("--"); break;
    case TYPE::Factorial: Debug.print("!" ); break;

    case TYPE::Range: Debug.print(".."); break;

    case TYPE::Negate : Debug.print(" -"); break;
    case TYPE::Bit_NOT: Debug.print(" ~"); break;
    case TYPE::Raw    : Debug.print(" :"); break;

    case TYPE::AND_Reduce : Debug.print( " &"); break;
    case TYPE::NAND_Reduce: Debug.print(" ~&"); break;
    case TYPE::OR_Reduce  : Debug.print( " |"); break;
    case TYPE::NOR_Reduce : Debug.print(" ~|"); break;
    case TYPE::XOR_Reduce : Debug.print( " #"); break;
    case TYPE::XNOR_Reduce: Debug.print(" ~#"); break;
    case TYPE::Logical_NOT: Debug.print( " !"); break;

    case TYPE::Cast       : Debug.print(" {cast} "); break;

    case TYPE::Replicate  : Debug.print("{rep}"); break;

    case TYPE::Exponential: Debug.print(" ^ " ); break;
    case TYPE::Multiply   : Debug.print(" * " ); break;
    case TYPE::Divide     : Debug.print(" / " ); break;
    case TYPE::Modulus    : Debug.print(" %% "); break;
    case TYPE::Add        : Debug.print(" + " ); break;
    case TYPE::Subtract   : Debug.print(" - " ); break;

    case TYPE::Shift_Left : Debug.print(" << "); break;
    case TYPE::Shift_Right: Debug.print(" >> "); break;

    case TYPE::Less         : Debug.print(" < " ); break;
    case TYPE::Greater      : Debug.print(" > " ); break;
    case TYPE::Less_Equal   : Debug.print(" <= "); break;
    case TYPE::Greater_Equal: Debug.print(" >= "); break;
    case TYPE::Equal        : Debug.print(" == "); break;
    case TYPE::Not_Equal    : Debug.print(" != "); break;

    case TYPE::Bit_AND : Debug.print( " & "); break;
    case TYPE::Bit_NAND: Debug.print(" ~& "); break;
    case TYPE::Bit_OR  : Debug.print( " | "); break;
    case TYPE::Bit_NOR : Debug.print(" ~| "); break;
    case TYPE::Bit_XOR : Debug.print( " # "); break;
    case TYPE::Bit_XNOR: Debug.print(" ~# "); break;

    case TYPE::Logical_AND: Debug.print(" && "); break;
    case TYPE::Logical_OR : Debug.print(" || "); break;

    case TYPE::Conditional : Debug.print(" ? "); break;

    default: error("(Unknown expression type: %d)", (int)Type);
  }

  if(Right){
    EXPRESSION* ExprRight;
    ASSIGNMENT* AssignRight;
    if(Right->Type > TYPE::Expression){
      ExprRight = (EXPRESSION*)Right;
      if(ExprRight->Left || ExprRight->Right || ExprRight->Next) Debug.print("(");
      ExprRight->Display();
      if(ExprRight->Left || ExprRight->Right || ExprRight->Next) Debug.print(")");

    }else if(Right->Type == TYPE::Assignment){
      AssignRight = (ASSIGNMENT*)Right;
      if(AssignRight->Left || AssignRight->Right || AssignRight->Next) Debug.print("(");
      AssignRight->Display();
      if(AssignRight->Left || AssignRight->Right || AssignRight->Next) Debug.print(")");

    }else{
      error("Invalid type");
      return;
    }
  }

  if(Next){
    Debug.print(", ");
    Next->Display();
  }
}
//------------------------------------------------------------------------------
