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

#include "Replicate.h"
#include "Literal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

REPLICATE::REPLICATE(int Line, const string& Filename): REPLICATE(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

REPLICATE::REPLICATE(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Replicate){
}
//------------------------------------------------------------------------------

REPLICATE::~REPLICATE(){
}
//------------------------------------------------------------------------------

BASE* REPLICATE::Copy(){
  REPLICATE* Copy = new REPLICATE(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool REPLICATE::GetVerilog(string& Body){
  Body += "{(";
  Right->GetVerilog(Body);
  Body += "){";
  Left->GetVerilog(Body);
  Body += "}}";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* REPLICATE::Evaluate(){
  assert(Left , return this);
  assert(Right, return this);

  Left  = Left ->Evaluate();
  Right = Right->Evaluate();

  assert(Left , return this);
  assert(Right, return this);

  if(Right->Type != TYPE::Literal){
    Error("Replicate number should evaluate to a literal");
    return this;
  }

  return MakeObject();
}
//------------------------------------------------------------------------------

int REPLICATE::GetWidth(){
  assert(Left , return 0);
  assert(Right, return 0);

  Left  = Left ->Evaluate();
  Right = Right->Evaluate();

  assert(Left , return 0);
  assert(Right, return 0);

  if(Right->Type != TYPE::Literal){
    Error("Replicate number should evaluate to a literal");
    return 0;
  }

  return Left->GetWidth() * ((LITERAL*)Right)->Value.GetReal();
}
//------------------------------------------------------------------------------

NUMBER& REPLICATE::GetFullScale(){
  static NUMBER Result;
  Result = 1;
  Result.BinScale(GetWidth());
  return Result;
}
//------------------------------------------------------------------------------

bool REPLICATE::GetSigned(){
  return false;
}
//------------------------------------------------------------------------------

bool REPLICATE::HasCircularReference(NETLIST::BASE* Object){
  assert(Left , return false);
  assert(Right, return false);
  
  if(Left ->HasCircularReference(Object)) return true;
  if(Right->HasCircularReference(Object)) return true;

  return false;
}
//------------------------------------------------------------------------------

void REPLICATE::PopulateUsed(){
  assert(Left , return);
  assert(Right, return);
  
  Left ->PopulateUsed();
  Right->PopulateUsed();
}
//------------------------------------------------------------------------------

EXPRESSION* REPLICATE::RemoveTempNet(){
  if(Left ) Left  = Left ->RemoveTempNet();
  if(Right) Right = Right->RemoveTempNet();
  return this;
}
//------------------------------------------------------------------------------

void REPLICATE::Display(){
  DisplayStart();

  Debug.Print("{rep}");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void REPLICATE::ValidateMembers(){
  assert(Type == TYPE::Replicate);

  assert(!Next);
  assert(!Prev);

  assert(Left , return); Left ->Validate();
  assert(Right, return); Right->Validate();
}
//------------------------------------------------------------------------------

