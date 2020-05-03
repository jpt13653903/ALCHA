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

#include "Bit_NAND.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

BIT_NAND::BIT_NAND(int Line, const string& Filename): BIT_NAND(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

BIT_NAND::BIT_NAND(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Bit_NAND){
}
//------------------------------------------------------------------------------

BIT_NAND::~BIT_NAND(){
}
//------------------------------------------------------------------------------

BASE* BIT_NAND::Copy(){
  BIT_NAND* Copy = new BIT_NAND(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool BIT_NAND::GetVerilog(string& Body){
  Body += "(";
  Left->GetVerilog(Body);
  Body += ") ~& (";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* BIT_NAND::Evaluate(){
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

int BIT_NAND::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

NUMBER& BIT_NAND::GetFullScale(){
  error("Not yet implemented");
  static NUMBER zero = 0;
  return zero;
}
//------------------------------------------------------------------------------

bool BIT_NAND::GetSigned(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool BIT_NAND::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void BIT_NAND::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

EXPRESSION* BIT_NAND::RemoveTempNet(){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

void BIT_NAND::Display(){
  DisplayStart();

  Debug.Print(" ~& ");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void BIT_NAND::ValidateMembers(){
  assert(Type == TYPE::Bit_NAND);

  assert(!Next);
  assert(!Prev);

  // TODO: assert(!Left );
  // TODO: assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

