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

#include "Shift_Right.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

SHIFT_RIGHT::SHIFT_RIGHT(int Line, const string& Filename): SHIFT_RIGHT(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

SHIFT_RIGHT::SHIFT_RIGHT(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Shift_Right){
}
//------------------------------------------------------------------------------

SHIFT_RIGHT::~SHIFT_RIGHT(){
}
//------------------------------------------------------------------------------

BASE* SHIFT_RIGHT::Copy(){
  SHIFT_RIGHT* Copy = new SHIFT_RIGHT(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool SHIFT_RIGHT::GetVerilog(string& Body){
  Body += "(";
  Left->GetVerilog(Body);
  Body += ") >>> (";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* SHIFT_RIGHT::Evaluate(){
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

int SHIFT_RIGHT::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

NUMBER& SHIFT_RIGHT::GetFullScale(){
  error("Not yet implemented");
  static NUMBER zero = 0;
  return zero;
}
//------------------------------------------------------------------------------

bool SHIFT_RIGHT::GetSigned(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool SHIFT_RIGHT::HasCircularReference(NETLIST::BASE* Object){
  assert(Left , return false);
  assert(Right, return false);
  
  if(Left ->HasCircularReference(Object)) return true;
  if(Right->HasCircularReference(Object)) return true;

  return false;
}
//------------------------------------------------------------------------------

void SHIFT_RIGHT::PopulateUsed(){
  assert(Left , return);
  assert(Right, return);
  
  Left ->PopulateUsed();
  Right->PopulateUsed();
}
//------------------------------------------------------------------------------

EXPRESSION* SHIFT_RIGHT::RemoveTempNet(int Width, bool Signed){
  if(Left ) Left  = Left ->RemoveTempNet(0, false);
  if(Right) Right = Right->RemoveTempNet(0, false);
  return this;
}
//------------------------------------------------------------------------------

void SHIFT_RIGHT::Display(){
  DisplayStart();

  Debug.Print(" >> ");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void SHIFT_RIGHT::ValidateMembers(){
  assert(Type == TYPE::Shift_Right);

  assert(!Next);
  assert(!Prev);

  assert(Left , return); Left ->Validate();
  assert(Right, return); Right->Validate();
}
//------------------------------------------------------------------------------

