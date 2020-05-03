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

#include "Logical_AND.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

LOGICAL_AND::LOGICAL_AND(int Line, const string& Filename): LOGICAL_AND(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

LOGICAL_AND::LOGICAL_AND(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Logical_AND){
}
//------------------------------------------------------------------------------

LOGICAL_AND::~LOGICAL_AND(){
}
//------------------------------------------------------------------------------

BASE* LOGICAL_AND::Copy(){
  LOGICAL_AND* Copy = new LOGICAL_AND(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool LOGICAL_AND::GetVerilog(string& Body){
  Body += "(";
  Left->GetVerilog(Body);
  Body += ") && (";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* LOGICAL_AND::Evaluate(){
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

int LOGICAL_AND::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

NUMBER& LOGICAL_AND::GetFullScale(){
  error("Not yet implemented");
  static NUMBER zero = 0;
  return zero;
}
//------------------------------------------------------------------------------

bool LOGICAL_AND::GetSigned(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* LOGICAL_AND::FixedPointScale(int Width, NUMBER& FullScale){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

bool LOGICAL_AND::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void LOGICAL_AND::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

void LOGICAL_AND::Display(){
  DisplayStart();

  Debug.Print(" && ");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void LOGICAL_AND::ValidateMembers(){
  assert(Type == TYPE::Logical_AND);

  assert(!Next);
  assert(!Prev);

  // TODO: assert(!Left );
  // TODO: assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

