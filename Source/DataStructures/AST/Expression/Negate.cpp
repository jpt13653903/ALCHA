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

#include "Negate.h"
#include "Literal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

NEGATE::NEGATE(int Line, const string& Filename): NEGATE(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

NEGATE::NEGATE(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Negate){
}
//------------------------------------------------------------------------------

NEGATE::~NEGATE(){
}
//------------------------------------------------------------------------------

BASE* NEGATE::Copy(){
  NEGATE* Copy = new NEGATE(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool NEGATE::GetVerilog(string& Body){
  Body += "-(";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* NEGATE::Evaluate(){
  error("Not yet implemented");
  return this;
//   EXPRESSION* Result = 0;
// 
//   Result = (EXPRESSION*)Copy(true);
//   if(!Result->Right){
//     delete Result;
//     return 0;
//   }
// 
//   if(!Result) return 0;
//   return Result->Simplify(false);
}
//------------------------------------------------------------------------------

int NEGATE::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

EXPRESSION* NEGATE::FixedPointScale(int Width, NUMBER& FullScale){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

bool NEGATE::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void NEGATE::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

// EXPRESSION* NEGATE::Simplify(bool GenWire){
//   assert(Right, return this);
// 
//   Right = Right->Simplify(true);
// 
//   EXPRESSION* Result = this;
// 
//   if(Right->Type == TYPE::Literal){
//     Result = Right;
//     Right = 0;
//     ((LITERAL*)Result)->Value.Mul(-1);
//     delete this;
//   }
//   return Result;
// }
//------------------------------------------------------------------------------

void NEGATE::Display(){
  DisplayStart();

  Debug.Print(" -");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void NEGATE::ValidateMembers(){
  assert(Type == TYPE::Negate);

  assert(!Next);
  assert(!Prev);

  // TODO: assert(!Left );
  // TODO: assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

