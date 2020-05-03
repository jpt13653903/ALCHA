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

#include "Less_Equal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

LESS_EQUAL::LESS_EQUAL(int Line, const string& Filename): LESS_EQUAL(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

LESS_EQUAL::LESS_EQUAL(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Less_Equal){
}
//------------------------------------------------------------------------------

LESS_EQUAL::~LESS_EQUAL(){
}
//------------------------------------------------------------------------------

BASE* LESS_EQUAL::Copy(){
  LESS_EQUAL* Copy = new LESS_EQUAL(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool LESS_EQUAL::GetVerilog(string& Body){
  Body += "(";
  Left->GetVerilog(Body);
  Body += ") <= (";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* LESS_EQUAL::Evaluate(){
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

int LESS_EQUAL::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

NUMBER& LESS_EQUAL::GetFullScale(){
  error("Not yet implemented");
  static NUMBER zero = 0;
  return zero;
}
//------------------------------------------------------------------------------

bool LESS_EQUAL::GetSigned(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool LESS_EQUAL::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void LESS_EQUAL::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

EXPRESSION* LESS_EQUAL::RemoveTempNet(int Width, bool Signed){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

void LESS_EQUAL::Display(){
  DisplayStart();

  Debug.Print(" <= ");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void LESS_EQUAL::ValidateMembers(){
  assert(Type == TYPE::Less_Equal);

  assert(!Next);
  assert(!Prev);

  // TODO: assert(!Left );
  // TODO: assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

