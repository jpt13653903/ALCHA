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

AST_Expression::AST_Expression(int Line, EXPRESSION_TYPE ExpressionType):
AST_Base(Line){
 this->Type           = Expression;
 this->ExpressionType = ExpressionType;

 Name  = 0;
 Value = 0;

 Left = Right = 0;
}
//------------------------------------------------------------------------------

AST_Expression::~AST_Expression(){
 if(Name ) delete Name;
 if(Value) delete Value;

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
  case Dot: printf("."); break;

  case Identifier:
   if(Name) printf("%s", Name->String());
   else     printf("(Identifier node has no name)");
   break;

  case Literal:
   if(Value) Value->Display();
   else      printf("(Literal node has no value)");
   break;

  default: printf("(Unknown expression type)");
 }

 if(Right){
  if(Right->Left || Right->Right) printf("(");
  Right->Display();
  if(Right->Left || Right->Right) printf(")");
 }

 if(Next) printf("\nIllegal \"Next\" on Expression\n");
}
//------------------------------------------------------------------------------
