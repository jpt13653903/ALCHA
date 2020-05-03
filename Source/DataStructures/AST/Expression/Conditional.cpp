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

#include "Conditional.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

CONDITIONAL::CONDITIONAL(int Line, const string& Filename): CONDITIONAL(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

CONDITIONAL::CONDITIONAL(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Conditional){
}
//------------------------------------------------------------------------------

CONDITIONAL::~CONDITIONAL(){
}
//------------------------------------------------------------------------------

BASE* CONDITIONAL::Copy(){
  CONDITIONAL* Copy = new CONDITIONAL(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool CONDITIONAL::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* CONDITIONAL::Evaluate(){
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

int CONDITIONAL::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

NUMBER& CONDITIONAL::GetFullScale(){
  error("Not yet implemented");
  static NUMBER zero = 0;
  return zero;
}
//------------------------------------------------------------------------------

bool CONDITIONAL::GetSigned(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool CONDITIONAL::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void CONDITIONAL::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

EXPRESSION* CONDITIONAL::RemoveTempNet(int Width, bool Signed){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

void CONDITIONAL::Display(){
  DisplayStart();

  Debug.Print(" ? ");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void CONDITIONAL::ValidateMembers(){
  assert(Type == TYPE::Conditional);

  assert(!Next);
  assert(!Prev);

  // TODO: assert(!Left );
  // TODO: assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

