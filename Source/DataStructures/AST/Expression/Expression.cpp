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
#include "../Assignment.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

EXPRESSION::EXPRESSION(int Line, const char* Filename, TYPE ExpressionType): BASE(Line, Filename, ExpressionType){
  Left  = 0;
  Right = 0;
}
//------------------------------------------------------------------------------

EXPRESSION::~EXPRESSION(){
  if(Left ) delete Left;
  if(Right) delete Right;
}
//------------------------------------------------------------------------------

void EXPRESSION::DisplayStart(){
  if(Left){
    if(Left->Left || Left->Right) Debug.print("(");
    Left->Display();
    if(Left->Left || Left->Right) Debug.print(")");
  }
}
//------------------------------------------------------------------------------

void EXPRESSION::DisplayEnd(){
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

  assert(Next == 0, info("Line: %d", Line)); // Expressions should not have a "Next"
}
//------------------------------------------------------------------------------

