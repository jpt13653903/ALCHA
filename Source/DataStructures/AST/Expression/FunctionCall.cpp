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

#include "FunctionCall.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

FUNCTIONCALL::FUNCTIONCALL(int Line, const string& Filename): FUNCTIONCALL(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

FUNCTIONCALL::FUNCTIONCALL(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::FunctionCall){
}
//------------------------------------------------------------------------------

FUNCTIONCALL::~FUNCTIONCALL(){
  foreach(Parameter, Parameters){
    if(*Parameter) delete *Parameter;
  }
}
//------------------------------------------------------------------------------

BASE* FUNCTIONCALL::Copy(){
  FUNCTIONCALL* Copy = new FUNCTIONCALL(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  foreach(Parameter, Parameters){
    if(*Parameter) Copy->Parameters.push_back((*Parameter)->Copy());
  }

  return Copy;
}
//------------------------------------------------------------------------------

bool FUNCTIONCALL::RunAST(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool FUNCTIONCALL::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* FUNCTIONCALL::Evaluate(){
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

int FUNCTIONCALL::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

EXPRESSION* FUNCTIONCALL::FixedPointScale(int Width, NUMBER& FullScale){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

bool FUNCTIONCALL::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void FUNCTIONCALL::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

void FUNCTIONCALL::Display(){
  DisplayStart();

  Debug.Print("{call}(");

  bool isFirst = true;
  foreach(Parameter, Parameters){
    if(!isFirst) Debug.Print(", ");
    isFirst = false;
    (*Parameter)->Display();
  }
  Debug.Print(")");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void FUNCTIONCALL::ValidateMembers(){
  assert(Type == TYPE::FunctionCall);

  assert(!Next);
  assert(!Prev);

  assert(!Left );
  assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

