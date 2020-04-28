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

#include "Multiply.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

MULTIPLY::MULTIPLY(int Line, const string& Filename): MULTIPLY(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

MULTIPLY::MULTIPLY(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Multiply){
}
//------------------------------------------------------------------------------

MULTIPLY::~MULTIPLY(){
}
//------------------------------------------------------------------------------

BASE* MULTIPLY::Copy(){
  MULTIPLY* Copy = new MULTIPLY(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool MULTIPLY::GetVerilog(string& Body){
  Body += "(";
  Left->GetVerilog(Body);
  Body += ") * (";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* MULTIPLY::Evaluate(){
  error("Not yet implemented");
  return this;
//   EXPRESSION* Result = (EXPRESSION*)Copy(true);
//   return Result->Simplify(false);
}
//------------------------------------------------------------------------------

int MULTIPLY::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

EXPRESSION* MULTIPLY::FixedPointScale(int Width, NUMBER& FullScale){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

// EXPRESSION* MULTIPLY::Simplify(bool GenWire){
//   assert(Left && Right, return this);
// 
//   Left = Left->Simplify(true);
//   Right = Right->Simplify(true);
// 
//   error("Not yet implemented");
//   return this;
// }
//------------------------------------------------------------------------------

void MULTIPLY::Display(){
  DisplayStart();

  Debug.Print(" * " );

  DisplayEnd();
}
//------------------------------------------------------------------------------

void MULTIPLY::ValidateMembers(){
  assert(Type == TYPE::Multiply);

  assert(!Next);
  assert(!Prev);

  assert(Left , return); Left ->Validate();
  assert(Right, return); Right->Validate();
}
//------------------------------------------------------------------------------

