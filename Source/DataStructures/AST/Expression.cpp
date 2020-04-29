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
#include "Expression/Literal.h"
#include "Expression/Multiply.h"
#include "Expression/Object.h"

#include "Netlist/Namespace/Module.h"
#include "Netlist/Synthesisable/Net.h"
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

EXPRESSION* EXPRESSION::ScaleWith(NUMBER& Scale, int Width, NUMBER& FullScale){
  if(Scale == 1) return this;
  
  auto Object  = new OBJECT      (Source.Line, Source.Filename);
  auto Net     = new NETLIST::NET(Source.Line, Source.Filename, 0);
  auto Mul     = new MULTIPLY    (Source.Line, Source.Filename);
  auto Literal = new LITERAL     (Source.Line, Source.Filename);

  Net->SetFixedPoint(Width, FullScale);

  Literal->Value     = Scale;
  Mul    ->Left      = this;
  Mul    ->Right     = Literal;
  Net    ->Value     = Mul;
  Object ->ObjectRef = Net;

  NETLIST::NamespaceStack.front()->Symbols[Net->Name] = Net;

  return Object;
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

