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

#include "Subtract.h"
#include "Literal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

SUBTRACT::SUBTRACT(int Line, const string& Filename): SUBTRACT(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

SUBTRACT::SUBTRACT(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Subtract){
}
//------------------------------------------------------------------------------

SUBTRACT::~SUBTRACT(){
}
//------------------------------------------------------------------------------

BASE* SUBTRACT::Copy(){
  SUBTRACT* Copy = new SUBTRACT(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool SUBTRACT::GetVerilog(string& Body){
  Body += "(";
  Left->GetVerilog(Body);
  Body += ") - (";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* SUBTRACT::Evaluate(){
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

int SUBTRACT::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

NUMBER& SUBTRACT::GetFullScale(){
  error("Not yet implemented");
  static NUMBER zero = 0;
  return zero;
}
//------------------------------------------------------------------------------

bool SUBTRACT::GetSigned(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* SUBTRACT::FixedPointScale(int Width, NUMBER& FullScale){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

bool SUBTRACT::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void SUBTRACT::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

void SUBTRACT::Display(){
  DisplayStart();

  Debug.Print(" - " );

  DisplayEnd();
}
//------------------------------------------------------------------------------

void SUBTRACT::ValidateMembers(){
  assert(Type == TYPE::Subtract);

  assert(!Next);
  assert(!Prev);

  // TODO: assert(!Left );
  // TODO: assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

