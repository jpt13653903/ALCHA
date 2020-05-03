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
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

LITERAL::LITERAL(int Line, const string& Filename): LITERAL(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

LITERAL::LITERAL(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Literal){
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
  if(!Value.IsReal()){
    Error("non-real literal");
    return false;
  }

  int  Width  = GetWidth ();
  bool Signed = GetSigned();

  NUMBER Result;
  if(Signed){
    Body += to_string(Width+1) + "'sh";
    Result = GetFullScale();
    Result.BinScale(1);
    Result.Add(Value);
  }else{
    Body += to_string(Width  ) + "'h";
    Result = Value;
  }
  Result.Round();
  Body += Result.GetString(16);
  Result.BinScale(-Width);
  if((!Signed && Result >= 1) || (Signed && Result > 2)){
    Error("The literal does not fit in its full-scale range");
    return false;
  }
  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* LITERAL::Evaluate(){
  return this;
}
//------------------------------------------------------------------------------

void LITERAL::SetWidth(int Width){
  WidthOverride = Width;
}
//------------------------------------------------------------------------------

int LITERAL::GetWidth(){
  if(WidthOverride) return WidthOverride;

  int Width = 0;
  NUMBER Num = Value;
  if(Num < 0) Num.Mul(-1);
  while(Num > 1){
    Num.BinScale(-1);
    Width++;
  }
  if(!GetSigned() && Num == 1){
    Num.BinScale(-1);
    Width++;
  }
  if(!Width) return 1;
  return Width;
}
//------------------------------------------------------------------------------

NUMBER& LITERAL::GetFullScale(){
  static NUMBER Result;
  Result = 1;
  Result.BinScale(GetWidth());
  return Result;
}
//------------------------------------------------------------------------------

bool LITERAL::GetSigned(){
  return Value < 0;
}
//------------------------------------------------------------------------------

bool LITERAL::HasCircularReference(NETLIST::BASE* Object){
  return false;
}
//------------------------------------------------------------------------------

void LITERAL::PopulateUsed(){
}
//------------------------------------------------------------------------------

EXPRESSION* LITERAL::RemoveTempNet(){
  return this;
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

