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

#include "AST_Expression.h"
//------------------------------------------------------------------------------

AST_Expression::AST_Expression(
  int             Line,
  const byte*     Filename,
  EXPRESSION_TYPE ExpressionType
): AST_Base(Line, Filename){
  this->Type           = Expression;
  this->ExpressionType = ExpressionType;

  Name     = 0;
  Value    = 0;
  StrValue = 0;

  Left = Right = 0;
}
//------------------------------------------------------------------------------

AST_Expression::~AST_Expression(){
  if(Value   ) delete Value;
  if(StrValue) delete StrValue;

  if(Left ) delete Left;
  if(Right) delete Right;
}
//------------------------------------------------------------------------------

void AST_Expression::Display(){
  if(Left){
    if(Left->Left || Left->Right) printf("(");
    Left->Display();
    if(Left->Left || Left->Right) printf(")");
  }

  switch(ExpressionType){
    case String:
      if(StrValue) printf("\"%s\"", StrValue->String());
      else         printf("(String literal node has no value)");
      break;

    case Literal:
      if(Value) Value->Display();
      else      printf("(Literal node has no value)");
      break;

    case Identifier:
      if(Name) printf("%s", Name);
      else     printf("(Identifier node has no name)");
      break;

    case Array:             printf("{Array}"       ); break;
    case VectorConcatenate: printf("{VectorConcat}"); break;
    case ArrayConcatenate:  printf("{ArrayConcat}" ); break;

    case FunctionCall: printf("{call}" ); break;

    case Slice: printf("{slice}"); break;

    case AccessMember:     printf("." ); break;
    case AccessMemberSafe: printf("?." ); break;
    case AccessAttribute:  printf("'" ); break;

    case Increment: printf("++"); break;
    case Decrement: printf("--"); break;
    case Factorial: printf("!" ); break;

    case Range: printf(".."); break;

    case Negate : printf(" -"); break;
    case Bit_NOT: printf(" ~"); break;
    case Raw    : printf(" :"); break;

    case AND_Reduce : printf( " &"); break;
    case NAND_Reduce: printf(" ~&"); break;
    case OR_Reduce  : printf( " |"); break;
    case NOR_Reduce : printf(" ~|"); break;
    case XOR_Reduce : printf( " #"); break;
    case XNOR_Reduce: printf(" ~#"); break;
    case Logical_NOT: printf( " !"); break;

    case Cast       : printf(" {cast} "); break;

    case Replicate  : printf("{rep}"); break;

    case Exponential: printf(" ^ " ); break;
    case Multiply   : printf(" * " ); break;
    case Divide     : printf(" / " ); break;
    case Modulus    : printf(" %% "); break;
    case Add        : printf(" + " ); break;
    case Subtract   : printf(" - " ); break;

    case Shift_Left : printf(" << "); break;
    case Shift_Right: printf(" >> "); break;

    case Less         : printf(" < " ); break;
    case Greater      : printf(" > " ); break;
    case Less_Equal   : printf(" <= "); break;
    case Greater_Equal: printf(" >= "); break;
    case Equal        : printf(" == "); break;
    case Not_Equal    : printf(" != "); break;

    case Bit_AND : printf( " & "); break;
    case Bit_NAND: printf(" ~& "); break;
    case Bit_OR  : printf( " | "); break;
    case Bit_NOR : printf(" ~| "); break;
    case Bit_XOR : printf( " # "); break;
    case Bit_XNOR: printf(" ~# "); break;

    case Logical_AND: printf(" && "); break;
    case Logical_OR : printf(" || "); break;

    case Conditional : printf(" ? "); break;

    default: printf("(Unknown expression type: %d)", ExpressionType);
  }

  if(Right){
    if(Right->Left || Right->Right || Right->Next) printf("(");
    Right->Display();
    if(Right->Left || Right->Right || Right->Next) printf(")");
  }

  if(Next){
    printf(", ");
    Next->Display();
  }
}
//------------------------------------------------------------------------------
