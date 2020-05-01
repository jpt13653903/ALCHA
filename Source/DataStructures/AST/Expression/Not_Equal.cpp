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

#include "Not_Equal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

NOT_EQUAL::NOT_EQUAL(int Line, const string& Filename): NOT_EQUAL(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

NOT_EQUAL::NOT_EQUAL(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Not_Equal){
}
//------------------------------------------------------------------------------

NOT_EQUAL::~NOT_EQUAL(){
}
//------------------------------------------------------------------------------

BASE* NOT_EQUAL::Copy(){
  NOT_EQUAL* Copy = new NOT_EQUAL(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool NOT_EQUAL::GetVerilog(string& Body){
  Body += "(";
  Left->GetVerilog(Body);
  Body += ") != (";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* NOT_EQUAL::Evaluate(){
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

int NOT_EQUAL::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

EXPRESSION* NOT_EQUAL::FixedPointScale(int Width, NUMBER& FullScale){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

bool NOT_EQUAL::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void NOT_EQUAL::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

// EXPRESSION* NOT_EQUAL::Simplify(bool GenWire){
//   assert(Left && Right, return this);
// 
//   Left = Left->Simplify(true);
//   Right = Right->Simplify(true);
// 
//   error("Not yet implemented");
//   return this;
// }
//------------------------------------------------------------------------------

void NOT_EQUAL::Display(){
  DisplayStart();

  Debug.Print(" != ");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void NOT_EQUAL::ValidateMembers(){
  assert(Type == TYPE::Not_Equal);

  assert(!Next);
  assert(!Prev);

  // TODO: assert(!Left );
  // TODO: assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

