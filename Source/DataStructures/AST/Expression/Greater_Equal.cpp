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

#include "Greater_Equal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

GREATER_EQUAL::GREATER_EQUAL(int Line, const string& Filename): GREATER_EQUAL(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

GREATER_EQUAL::GREATER_EQUAL(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Greater_Equal){
}
//------------------------------------------------------------------------------

GREATER_EQUAL::~GREATER_EQUAL(){
}
//------------------------------------------------------------------------------

BASE* GREATER_EQUAL::Copy(){
  GREATER_EQUAL* Copy = new GREATER_EQUAL(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool GREATER_EQUAL::GetVerilog(string& Body){
  Body += "(";
  Left->GetVerilog(Body);
  Body += ") >= (";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* GREATER_EQUAL::Evaluate(){
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

int GREATER_EQUAL::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

NUMBER& GREATER_EQUAL::GetFullScale(){
  error("Not yet implemented");
  static NUMBER zero = 0;
  return zero;
}
//------------------------------------------------------------------------------

bool GREATER_EQUAL::GetSigned(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* GREATER_EQUAL::FixedPointScale(int Width, NUMBER& FullScale){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

bool GREATER_EQUAL::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void GREATER_EQUAL::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

void GREATER_EQUAL::Display(){
  DisplayStart();

  Debug.Print(" >= ");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void GREATER_EQUAL::ValidateMembers(){
  assert(Type == TYPE::Greater_Equal);

  assert(!Next);
  assert(!Prev);

  // TODO: assert(!Left );
  // TODO: assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

