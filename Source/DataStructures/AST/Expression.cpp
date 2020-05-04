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
#include "Expression/Shift_Left.h"
#include "Expression/Shift_Right.h"

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

EXPRESSION* EXPRESSION::ScaleWith(NUMBER Scale, int Width, NUMBER FullScale){
  if(Scale == 1) return this;

  bool Signed    = GetSigned();
  int  ThisWidth = GetWidth ();

  int ScaleWidth = 0;
  NUMBER Num = Scale;
  if(Num < 0) Num.Mul(-1);
  while(Num < 1){
    Num.BinScale(1);
    ScaleWidth--;
  }
  while(Num > 1){
    Num.BinScale(-1);
    ScaleWidth++;
  }

  if(ThisWidth + ScaleWidth > Width){
    FullScale.BinScale(ThisWidth + ScaleWidth - Width);
    Width = ThisWidth + ScaleWidth;
  }

  // Calculate the limit of the inferred multiplier size.  Most FPGAs have 
  // 18-bit multipliers, so make that the minimum limit, otherwise use the 
  // target width as the limit so that no to little resolution is lost.
  NUMBER Limit(1);
  if(Signed) Limit.BinScale(18);
  else       Limit.BinScale(17);

  // Convert the multiplication to a shift, as far as possible
  int Shift = 0;
  while(Scale.IsInt()){
    Scale.BinScale(-1);
    Shift--;
  }
  while(!Scale.IsInt() && (Scale < Limit)){
    Scale.BinScale(1);
    Shift++;
  }
  while(Scale >= Limit){
    Scale.BinScale(-1);
    Shift--;
  }
  NUMBER FullFactor(Scale);
  Scale.Round();
  if(Scale != FullFactor){
    Warning("Rounding the scaling factor - this can be fixed "
            "with an explicit scaling multiplication.");
    while(Scale.IsInt()){ // Make sure it's still minimised after rounding
      Scale.BinScale(-1);
      Shift--;
    }
    while(!Scale.IsInt()){
      Scale.BinScale(1);
      Shift++;
    }
  }
  
  auto Net = new NETLIST::NET(Source.Line, Source.Filename, 0);
  Net->SetFixedPoint(Width, FullScale, Signed);
  NETLIST::NamespaceStack.front()->Symbols[Net->Name] = Net;
  
  if(Scale == 1){ // Shift only
    auto Literal = new LITERAL(Source.Line, Source.Filename);

    if(Shift > 0){
      Net->Value = new SHIFT_RIGHT(Source.Line, Source.Filename);
      Literal->Value = Shift;

    }else{
      Net->Value = new SHIFT_LEFT(Source.Line, Source.Filename);
      Literal->Value = -Shift;
    }

    Net->Value->Left  = this;
    Net->Value->Right = Literal;

  }else if(Shift == 0){ // Multiply only
    auto Literal = new LITERAL(Source.Line, Source.Filename);
    Literal->Value = Scale;

    auto Mul = new MULTIPLY(Source.Line, Source.Filename);
    Mul->Left  = this;
    Mul->Right = Literal;

    Net->Value = Mul;

  }else{ // Multiply and shift
    auto MulLiteral = new LITERAL(Source.Line, Source.Filename);
    MulLiteral->Value = Scale;

    auto Mul = new MULTIPLY(Source.Line, Source.Filename);
    Mul->Left  = this;
    Mul->Right = MulLiteral;

    auto MulNet = new NETLIST::NET(Source.Line, Source.Filename, 0);
    NUMBER MulFullScale = FullScale;
    MulFullScale.BinScale(Shift);
    MulNet->SetFixedPoint(Mul->GetWidth(), MulFullScale, GetSigned());
    MulNet->Value = Mul;
    NETLIST::NamespaceStack.front()->Symbols[MulNet->Name] = MulNet;

    auto MulObject = new OBJECT(Source.Line, Source.Filename);
    MulObject->ObjectRef = MulNet;

    auto ShiftLiteral = new LITERAL(Source.Line, Source.Filename);

    if(Shift > 0){
      Net->Value = new SHIFT_RIGHT(Source.Line, Source.Filename);
      ShiftLiteral->Value = Shift;

    }else{
      Net->Value = new SHIFT_LEFT(Source.Line, Source.Filename);
      ShiftLiteral->Value = -Shift;
    }

    Net->Value->Left  = MulObject;
    Net->Value->Right = ShiftLiteral;
  }

  auto Object = new OBJECT(Source.Line, Source.Filename);
  Object->ObjectRef = Net;
  return Object;
}
//------------------------------------------------------------------------------

EXPRESSION* EXPRESSION::MakeObject(){
  if(Type == TYPE::Object) return this;

  auto Object = new OBJECT      (Source.Line, Source.Filename);
  auto Net    = new NETLIST::NET(Source.Line, Source.Filename, 0);
  Object->ObjectRef = Net;

  Net->SetFixedPoint(GetWidth(), GetFullScale(), GetSigned());
  Net->Value = this;

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

