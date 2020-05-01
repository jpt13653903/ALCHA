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

#include "Divide.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

DIVIDE::DIVIDE(int Line, const string& Filename): DIVIDE(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

DIVIDE::DIVIDE(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Divide){
}
//------------------------------------------------------------------------------

DIVIDE::~DIVIDE(){
}
//------------------------------------------------------------------------------

BASE* DIVIDE::Copy(){
  DIVIDE* Copy = new DIVIDE(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool DIVIDE::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* DIVIDE::Evaluate(){
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

int DIVIDE::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

EXPRESSION* DIVIDE::FixedPointScale(int Width, NUMBER& FullScale){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

bool DIVIDE::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void DIVIDE::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

void DIVIDE::Display(){
  DisplayStart();

  Debug.Print(" / " );

  DisplayEnd();
}
//------------------------------------------------------------------------------

void DIVIDE::ValidateMembers(){
  assert(Type == TYPE::Divide);

  assert(!Next);
  assert(!Prev);

  // TODO: assert(!Left );
  // TODO: assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

