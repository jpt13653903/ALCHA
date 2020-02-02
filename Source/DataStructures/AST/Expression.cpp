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
//------------------------------------------------------------------------------

using namespace AST;
//------------------------------------------------------------------------------

EXPRESSION::EXPRESSION(
  int             Line,
  const char*     Filename,
  EXPRESSION_TYPE ExpressionType
): BASE(Line, Filename, TYPE::Expression){
  this->ExpressionType = ExpressionType;

  Value    = 0;
  StrValue = 0;

  Left  = 0;
  Right = 0;
}
//------------------------------------------------------------------------------

EXPRESSION::~EXPRESSION(){
  if(Value   ) delete Value;
  if(StrValue) delete StrValue;

  if(Left ) delete Left;
  if(Right) delete Right;
}
//------------------------------------------------------------------------------

void EXPRESSION::Display(){
  if(Left){
    if(Left->Left || Left->Right) Debug.print("(");
    Left->Display();
    if(Left->Left || Left->Right) Debug.print(")");
  }

  switch(ExpressionType){
    case EXPRESSION_TYPE::String:
      if(StrValue) Debug.print("\"%s\"", StrValue->c_str());
      else         error ("(String literal node has no value)");
      break;

    case EXPRESSION_TYPE::Literal:
      if(Value) Debug.print(Value->Display());
      else      error("(Literal node has no value)");
      break;

    case EXPRESSION_TYPE::Identifier:
      if(Name.empty()) error ("(Identifier node has no name)");
      else             Debug.print("%s", Name.c_str());
      break;

    case EXPRESSION_TYPE::Array            : Debug.print("{Array}"       ); break;
    case EXPRESSION_TYPE::VectorConcatenate: Debug.print("{VectorConcat}"); break;
    case EXPRESSION_TYPE::ArrayConcatenate : Debug.print("{ArrayConcat}" ); break;

    case EXPRESSION_TYPE::FunctionCall: Debug.print("{call}" ); break;

    case EXPRESSION_TYPE::Slice: Debug.print("{slice}"); break;

    case EXPRESSION_TYPE::AccessMember:     Debug.print("." ); break;
    case EXPRESSION_TYPE::AccessMemberSafe: Debug.print("?." ); break;
    case EXPRESSION_TYPE::AccessAttribute:  Debug.print("'" ); break;

    case EXPRESSION_TYPE::Increment: Debug.print("++"); break;
    case EXPRESSION_TYPE::Decrement: Debug.print("--"); break;
    case EXPRESSION_TYPE::Factorial: Debug.print("!" ); break;

    case EXPRESSION_TYPE::Range: Debug.print(".."); break;

    case EXPRESSION_TYPE::Negate : Debug.print(" -"); break;
    case EXPRESSION_TYPE::Bit_NOT: Debug.print(" ~"); break;
    case EXPRESSION_TYPE::Raw    : Debug.print(" :"); break;

    case EXPRESSION_TYPE::AND_Reduce : Debug.print( " &"); break;
    case EXPRESSION_TYPE::NAND_Reduce: Debug.print(" ~&"); break;
    case EXPRESSION_TYPE::OR_Reduce  : Debug.print( " |"); break;
    case EXPRESSION_TYPE::NOR_Reduce : Debug.print(" ~|"); break;
    case EXPRESSION_TYPE::XOR_Reduce : Debug.print( " #"); break;
    case EXPRESSION_TYPE::XNOR_Reduce: Debug.print(" ~#"); break;
    case EXPRESSION_TYPE::Logical_NOT: Debug.print( " !"); break;

    case EXPRESSION_TYPE::Cast       : Debug.print(" {cast} "); break;

    case EXPRESSION_TYPE::Replicate  : Debug.print("{rep}"); break;

    case EXPRESSION_TYPE::Exponential: Debug.print(" ^ " ); break;
    case EXPRESSION_TYPE::Multiply   : Debug.print(" * " ); break;
    case EXPRESSION_TYPE::Divide     : Debug.print(" / " ); break;
    case EXPRESSION_TYPE::Modulus    : Debug.print(" %% "); break;
    case EXPRESSION_TYPE::Add        : Debug.print(" + " ); break;
    case EXPRESSION_TYPE::Subtract   : Debug.print(" - " ); break;

    case EXPRESSION_TYPE::Shift_Left : Debug.print(" << "); break;
    case EXPRESSION_TYPE::Shift_Right: Debug.print(" >> "); break;

    case EXPRESSION_TYPE::Less         : Debug.print(" < " ); break;
    case EXPRESSION_TYPE::Greater      : Debug.print(" > " ); break;
    case EXPRESSION_TYPE::Less_Equal   : Debug.print(" <= "); break;
    case EXPRESSION_TYPE::Greater_Equal: Debug.print(" >= "); break;
    case EXPRESSION_TYPE::Equal        : Debug.print(" == "); break;
    case EXPRESSION_TYPE::Not_Equal    : Debug.print(" != "); break;

    case EXPRESSION_TYPE::Bit_AND : Debug.print( " & "); break;
    case EXPRESSION_TYPE::Bit_NAND: Debug.print(" ~& "); break;
    case EXPRESSION_TYPE::Bit_OR  : Debug.print( " | "); break;
    case EXPRESSION_TYPE::Bit_NOR : Debug.print(" ~| "); break;
    case EXPRESSION_TYPE::Bit_XOR : Debug.print( " # "); break;
    case EXPRESSION_TYPE::Bit_XNOR: Debug.print(" ~# "); break;

    case EXPRESSION_TYPE::Logical_AND: Debug.print(" && "); break;
    case EXPRESSION_TYPE::Logical_OR : Debug.print(" || "); break;

    case EXPRESSION_TYPE::Conditional : Debug.print(" ? "); break;

    default: error("(Unknown expression type: %d)", (int)ExpressionType);
  }

  if(Right){
    EXPRESSION* ExprRight;
    ASSIGNMENT* AssignRight;
    switch(Right->Type){
      case TYPE::Expression:
        ExprRight = (EXPRESSION*)Right;
        if(ExprRight->Left || ExprRight->Right || ExprRight->Next) Debug.print("(");
        ExprRight->Display();
        if(ExprRight->Left || ExprRight->Right || ExprRight->Next) Debug.print(")");
        break;

      case TYPE::Assignment:
        AssignRight = (ASSIGNMENT*)Right;
        if(AssignRight->Left || AssignRight->Right || AssignRight->Next) Debug.print("(");
        AssignRight->Display();
        if(AssignRight->Left || AssignRight->Right || AssignRight->Next) Debug.print(")");
        break;

      default:
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
