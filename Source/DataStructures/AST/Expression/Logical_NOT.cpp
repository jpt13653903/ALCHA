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

#include "Logical_NOT.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

LOGICAL_NOT::LOGICAL_NOT(int Line, const string& Filename): LOGICAL_NOT(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

LOGICAL_NOT::LOGICAL_NOT(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Logical_NOT){
}
//------------------------------------------------------------------------------

LOGICAL_NOT::~LOGICAL_NOT(){
}
//------------------------------------------------------------------------------

BASE* LOGICAL_NOT::Copy(){
  LOGICAL_NOT* Copy = new LOGICAL_NOT(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool LOGICAL_NOT::GetVerilog(string& Body){
  Body += "!(";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* LOGICAL_NOT::Evaluate(){
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

int LOGICAL_NOT::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

EXPRESSION* LOGICAL_NOT::FixedPointScale(int Width, NUMBER& FullScale){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

bool LOGICAL_NOT::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

// EXPRESSION* LOGICAL_NOT::Simplify(bool GenWire){
//   assert(Right, return this);
// 
//   Right = Right->Simplify(true);
// 
//   error("Not yet implemented");
//   return this;
// }
//------------------------------------------------------------------------------

void LOGICAL_NOT::Display(){
  DisplayStart();

  Debug.Print( " !");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void LOGICAL_NOT::ValidateMembers(){
  assert(Type == TYPE::Logical_NOT);

  assert(!Next);
  assert(!Prev);

  // TODO: assert(!Left );
  // TODO: assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

