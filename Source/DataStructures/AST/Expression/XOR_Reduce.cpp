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

#include "XOR_Reduce.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

XOR_REDUCE::XOR_REDUCE(int Line, const string& Filename): XOR_REDUCE(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

XOR_REDUCE::XOR_REDUCE(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::XOR_Reduce){
}
//------------------------------------------------------------------------------

XOR_REDUCE::~XOR_REDUCE(){
}
//------------------------------------------------------------------------------

BASE* XOR_REDUCE::Copy(){
  XOR_REDUCE* Copy = new XOR_REDUCE(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool XOR_REDUCE::GetVerilog(string& Body){
  Body += "^(";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* XOR_REDUCE::Evaluate(){
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

int XOR_REDUCE::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

EXPRESSION* XOR_REDUCE::FixedPointScale(int Width, NUMBER& FullScale){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

bool XOR_REDUCE::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void XOR_REDUCE::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

// EXPRESSION* XOR_REDUCE::Simplify(bool GenWire){
//   assert(Right, return this);
// 
//   Right = Right->Simplify(true);
// 
//   error("Not yet implemented");
//   return this;
// }
//------------------------------------------------------------------------------

void XOR_REDUCE::Display(){
  DisplayStart();

  Debug.Print(" #");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void XOR_REDUCE::ValidateMembers(){
  assert(Type == TYPE::XOR_Reduce);

  assert(!Next);
  assert(!Prev);

  // TODO: assert(!Left );
  // TODO: assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

