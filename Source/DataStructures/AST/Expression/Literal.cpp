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

#include "Literal.h"
#include "Multiply.h"
#include "Object.h"

#include "Netlist/Namespace/Module.h"
#include "Netlist/Synthesisable/Net.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

LITERAL::LITERAL(int Line, const string& Filename): LITERAL(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

LITERAL::LITERAL(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Literal){
  Signed = false;
  Width  = 0;
}
//------------------------------------------------------------------------------

LITERAL::~LITERAL(){
}
//------------------------------------------------------------------------------

BASE* LITERAL::Copy(){
  LITERAL* Copy = new LITERAL(Source.Line, Source.Filename.c_str());

  Copy->Value = Value;

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool LITERAL::GetVerilog(string& Body){
  if(!Width){
    Error("Literal has unknown width");
    return false;
  }
  if(!Value.IsReal()){
    Error("non-real literal");
    return false;
  }
  NUMBER Result(Value);
  bool IsPositive = Result.IsPositive();
  if(!IsPositive){
    if(!Signed){
      Error("Cannot store a negative literal to an unsigned target");
      return false;
    }
    Body += "-";
    Result.Mul(-1);
  }
  if(Signed) Body += to_string(Width+1) + "'h";
  else       Body += to_string(Width  ) + "'h";
  Result.Round();
  Body += Result.GetString(16);
  Result.BinScale(-Width);
  if((IsPositive && Result >= 1) || (!IsPositive && Result > 1)){
    Error("The literal does not fit in its full-scale range");
    return false;
  }
  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* LITERAL::Evaluate(bool CreateWires){
  return this;
}
//------------------------------------------------------------------------------

int LITERAL::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

EXPRESSION* LITERAL::FixedPointScale(int Width, NUMBER& FullScale){
  NUMBER Scale = 1;
  Scale.BinScale(Width);
  Scale.Div(FullScale);

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

void LITERAL::Display(){
  DisplayStart();

  Debug.Print(Value.Display());

  DisplayEnd();
}
//------------------------------------------------------------------------------

void LITERAL::ValidateMembers(){
  assert(Type == TYPE::Literal);

  assert(!Next);
  assert(!Prev);

  assert(!Left );
  assert(!Right);
}
//------------------------------------------------------------------------------

