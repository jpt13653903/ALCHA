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

#include "Add.h"
#include "Literal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

ADD::ADD(int Line, const string& Filename): ADD(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

ADD::ADD(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Add){
}
//------------------------------------------------------------------------------

ADD::~ADD(){
}
//------------------------------------------------------------------------------

BASE* ADD::Copy(){
  ADD* Copy = new ADD(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool ADD::GetVerilog(string& Body){
  Body += "(";
  Left->GetVerilog(Body);
  Body += ") + (";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* ADD::Evaluate(){
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

int ADD::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

EXPRESSION* ADD::FixedPointScale(int Width, NUMBER& FullScale){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

bool ADD::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void ADD::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

// EXPRESSION* ADD::Simplify(bool GenWire){
//   assert(Left && Right, return this);
// 
//   Left = Left->Simplify(true);
//   Right = Right->Simplify(true);
// 
//   EXPRESSION* Result = this;
// 
//   if(Left->Type == TYPE::Literal && Right->Type == TYPE::Literal){
//     auto Literal = new LITERAL(Source.Line, Source.Filename);
//     Literal->Value =   ((LITERAL*)Left )->Value;
//     Literal->Value.Add(((LITERAL*)Right)->Value);
//     delete this;
//     Result = Literal;
//   }
//   // TODO When adding an expression to a literal, follow the rules
//   //      in the SIPS article
// 
//   error("Not yet implemented");
//   return Result;
// }
//------------------------------------------------------------------------------

void ADD::Display(){
  DisplayStart();

  Debug.Print(" + " );

  DisplayEnd();
}
//------------------------------------------------------------------------------

void ADD::ValidateMembers(){
  assert(Type == TYPE::Add);
  
  assert(!Next);
  assert(!Prev);
  
  // TODO: assert(Left );
  // TODO: assert(Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

