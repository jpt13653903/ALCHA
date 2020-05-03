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

#include "Bit_XOR.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

BIT_XOR::BIT_XOR(int Line, const string& Filename): BIT_XOR(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

BIT_XOR::BIT_XOR(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Bit_XOR){
}
//------------------------------------------------------------------------------

BIT_XOR::~BIT_XOR(){
}
//------------------------------------------------------------------------------

BASE* BIT_XOR::Copy(){
  BIT_XOR* Copy = new BIT_XOR(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool BIT_XOR::GetVerilog(string& Body){
  Body += "(";
  Left->GetVerilog(Body);
  Body += ") ^ (";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* BIT_XOR::Evaluate(){
  error("Not yet implemented");
  return this;
//   EXPRESSION* Result = 0;
// 
//   error("Not yet implemented");
// 
//   if(!Result) return 0;
//   return Result->Simplify(false);
}
//------------------------------------------------------------------------------

int BIT_XOR::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

NUMBER& BIT_XOR::GetFullScale(){
  error("Not yet implemented");
  static NUMBER zero = 0;
  return zero;
}
//------------------------------------------------------------------------------

bool BIT_XOR::GetSigned(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* BIT_XOR::FixedPointScale(int Width, NUMBER& FullScale){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

bool BIT_XOR::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void BIT_XOR::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

void BIT_XOR::Display(){
  DisplayStart();

  Debug.Print( " # ");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void BIT_XOR::ValidateMembers(){
  assert(Type == TYPE::Bit_XOR);

  assert(!Next);
  assert(!Prev);

  // TODO: assert(!Left );
  // TODO: assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

