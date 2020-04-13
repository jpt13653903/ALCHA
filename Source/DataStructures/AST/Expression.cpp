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

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

EXPRESSION::EXPRESSION(int Line, const char* Filename, TYPE ExpressionType):
BASE(Line, Filename, ExpressionType){
  Left  = 0;
  Right = 0;
}
//------------------------------------------------------------------------------

EXPRESSION::~EXPRESSION(){
  if(Left ) delete Left;
  if(Right) delete Right;
  assert(!Next);
}
//------------------------------------------------------------------------------

bool EXPRESSION::IsExpression(){
  return true;
}
//------------------------------------------------------------------------------

void EXPRESSION::DisplayStart(){
  if(Left){
    if(Left->Left || Left->Right) Debug.Print("(");
    Left->Display();
    if(Left->Left || Left->Right) Debug.Print(")");
  }
}
//------------------------------------------------------------------------------

void EXPRESSION::DisplayEnd(){
  if(Right){
    if(Right->Left || Right->Right || Right->Next) Debug.Print("(");
    Right->Display();
    if(Right->Left || Right->Right || Right->Next) Debug.Print(")");
  }
}
//------------------------------------------------------------------------------

bool EXPRESSION::RunAST(){
  // For most expression types, this function is never called
  error("Unexpected RunAST call");
  return false;
}
//------------------------------------------------------------------------------

