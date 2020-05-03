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

#include "Bit_NOR.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

BIT_NOR::BIT_NOR(int Line, const string& Filename): BIT_NOR(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

BIT_NOR::BIT_NOR(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Bit_NOR){
}
//------------------------------------------------------------------------------

BIT_NOR::~BIT_NOR(){
}
//------------------------------------------------------------------------------

BASE* BIT_NOR::Copy(){
  BIT_NOR* Copy = new BIT_NOR(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool BIT_NOR::GetVerilog(string& Body){
  Body += "(";
  Left->GetVerilog(Body);
  Body += ") ~| (";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* BIT_NOR::Evaluate(){
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

int BIT_NOR::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

NUMBER& BIT_NOR::GetFullScale(){
  error("Not yet implemented");
  static NUMBER zero = 0;
  return zero;
}
//------------------------------------------------------------------------------

bool BIT_NOR::GetSigned(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* BIT_NOR::FixedPointScale(int Width, NUMBER& FullScale){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

bool BIT_NOR::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void BIT_NOR::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

void BIT_NOR::Display(){
  DisplayStart();

  Debug.Print(" ~| ");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void BIT_NOR::ValidateMembers(){
  assert(Type == TYPE::Bit_NOR);

  assert(!Next);
  assert(!Prev);

  // TODO: assert(!Left );
  // TODO: assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

