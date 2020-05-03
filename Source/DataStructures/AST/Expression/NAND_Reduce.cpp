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

#include "NAND_Reduce.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

NAND_REDUCE::NAND_REDUCE(int Line, const string& Filename): NAND_REDUCE(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

NAND_REDUCE::NAND_REDUCE(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::NAND_Reduce){
}
//------------------------------------------------------------------------------

NAND_REDUCE::~NAND_REDUCE(){
}
//------------------------------------------------------------------------------

BASE* NAND_REDUCE::Copy(){
  NAND_REDUCE* Copy = new NAND_REDUCE(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool NAND_REDUCE::GetVerilog(string& Body){
  Body += "~&(";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* NAND_REDUCE::Evaluate(){
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

int NAND_REDUCE::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

NUMBER& NAND_REDUCE::GetFullScale(){
  error("Not yet implemented");
  static NUMBER zero = 0;
  return zero;
}
//------------------------------------------------------------------------------

bool NAND_REDUCE::GetSigned(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool NAND_REDUCE::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void NAND_REDUCE::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

EXPRESSION* NAND_REDUCE::RemoveTempNet(){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

void NAND_REDUCE::Display(){
  DisplayStart();

  Debug.Print(" ~&");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void NAND_REDUCE::ValidateMembers(){
  assert(Type == TYPE::NAND_Reduce);

  assert(!Next);
  assert(!Prev);

  // TODO: assert(!Left );
  // TODO: assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

