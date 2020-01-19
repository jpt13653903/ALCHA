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
    case String:
      if(StrValue) Debug.print("\"%s\"", StrValue->c_str());
      else         error ("(String literal node has no value)");
      break;

    case Literal:
      if(Value) Debug.print(Value->Display());
      else      error("(Literal node has no value)");
      break;

    case Identifier:
      if(Name.empty()) error ("(Identifier node has no name)");
      else             Debug.print("%s", Name.c_str());
      break;

    case Array:             Debug.print("{Array}"       ); break;
    case VectorConcatenate: Debug.print("{VectorConcat}"); break;
    case ArrayConcatenate:  Debug.print("{ArrayConcat}" ); break;

    case FunctionCall: Debug.print("{call}" ); break;

    case Slice: Debug.print("{slice}"); break;

    case AccessMember:     Debug.print("." ); break;
    case AccessMemberSafe: Debug.print("?." ); break;
    case AccessAttribute:  Debug.print("'" ); break;

    case Increment: Debug.print("++"); break;
    case Decrement: Debug.print("--"); break;
    case Factorial: Debug.print("!" ); break;

    case Range: Debug.print(".."); break;

    case Negate : Debug.print(" -"); break;
    case Bit_NOT: Debug.print(" ~"); break;
    case Raw    : Debug.print(" :"); break;

    case AND_Reduce : Debug.print( " &"); break;
    case NAND_Reduce: Debug.print(" ~&"); break;
    case OR_Reduce  : Debug.print( " |"); break;
    case NOR_Reduce : Debug.print(" ~|"); break;
    case XOR_Reduce : Debug.print( " #"); break;
    case XNOR_Reduce: Debug.print(" ~#"); break;
    case Logical_NOT: Debug.print( " !"); break;

    case Cast       : Debug.print(" {cast} "); break;

    case Replicate  : Debug.print("{rep}"); break;

    case Exponential: Debug.print(" ^ " ); break;
    case Multiply   : Debug.print(" * " ); break;
    case Divide     : Debug.print(" / " ); break;
    case Modulus    : Debug.print(" %% "); break;
    case Add        : Debug.print(" + " ); break;
    case Subtract   : Debug.print(" - " ); break;

    case Shift_Left : Debug.print(" << "); break;
    case Shift_Right: Debug.print(" >> "); break;

    case Less         : Debug.print(" < " ); break;
    case Greater      : Debug.print(" > " ); break;
    case Less_Equal   : Debug.print(" <= "); break;
    case Greater_Equal: Debug.print(" >= "); break;
    case Equal        : Debug.print(" == "); break;
    case Not_Equal    : Debug.print(" != "); break;

    case Bit_AND : Debug.print( " & "); break;
    case Bit_NAND: Debug.print(" ~& "); break;
    case Bit_OR  : Debug.print( " | "); break;
    case Bit_NOR : Debug.print(" ~| "); break;
    case Bit_XOR : Debug.print( " # "); break;
    case Bit_XNOR: Debug.print(" ~# "); break;

    case Logical_AND: Debug.print(" && "); break;
    case Logical_OR : Debug.print(" || "); break;

    case Conditional : Debug.print(" ? "); break;

    default: error("(Unknown expression type: %d)", ExpressionType);
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
