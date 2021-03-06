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
#include "Object.h"

#include "Netlist/Namespace/Module.h"
#include "Netlist/Synthesisable/Net.h"
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
  if(Right->GetSigned()){
    Right->GetVerilog(Body);
  }else{
    Body += "$signed(";
    Right->GetVerilog(Body);
    Body += ")";
  }
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* NEGATE::Evaluate(){
  assert(Right, delete this; return 0);
  Right = Right->Evaluate();
  assert(Right, delete this; return 0);

  switch(Right->Type){
    case TYPE::Literal:{
      auto Result = (LITERAL*)Right;
      Right = 0;
      Result->Value.Mul(-1);
      delete this;
      return Result;
    }
    case TYPE::Object:{
      return MakeObject();
    }
    default:
      break;
  }
  return this;
}
//------------------------------------------------------------------------------

int NEGATE::GetWidth(){
  assert(Right, return false);
  return Right->GetWidth();
}
//------------------------------------------------------------------------------

NUMBER& NEGATE::GetFullScale(){
  static NUMBER zero = 0;
  assert(Right, return zero);
  return Right->GetFullScale();
}
//------------------------------------------------------------------------------

bool NEGATE::GetSigned(){
  return true;
}
//------------------------------------------------------------------------------

bool NEGATE::HasCircularReference(NETLIST::BASE* Object){
  assert(Right, return false);
  return Right->HasCircularReference(Object);
}
//------------------------------------------------------------------------------

void NEGATE::PopulateUsed(){
  assert(Right, return);
  Right->PopulateUsed();
}
//------------------------------------------------------------------------------

EXPRESSION* NEGATE::RemoveTempNet(int Width, bool Signed){
  if(Right) Right = Right->RemoveTempNet(0, false);
  return this;
}
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

  assert(!Left);
  assert(Right, return); Right->Validate();
}
//------------------------------------------------------------------------------

