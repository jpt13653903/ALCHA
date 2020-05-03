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
  Right->GetVerilog(Body);
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
      auto Net = new NETLIST::NET(Source.Line, Source.Filename, 0);
      Net->Value = this;
  
      auto ObjectRef = ((OBJECT*)Right)->ObjectRef;
      if(ObjectRef && ObjectRef->IsSynthesisable()){
        auto Synthesisable = (NETLIST::SYNTHESISABLE*)ObjectRef;
        Net->SetFixedPoint(Synthesisable->Width(), Synthesisable->FullScale());
      }
      NETLIST::NamespaceStack.front()->Symbols[Net->Name] = Net;
  
      auto Object = new OBJECT(Source.Line, Source.Filename);
      Object->ObjectRef = Net;
      return Object;
    }
    default:
      break;
  }
  return this;
}
//------------------------------------------------------------------------------

int NEGATE::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

NUMBER& NEGATE::GetFullScale(){
  error("Not yet implemented");
  static NUMBER zero = 0;
  return zero;
}
//------------------------------------------------------------------------------

bool NEGATE::GetSigned(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* NEGATE::FixedPointScale(int Width, NUMBER& FullScale){
  if(Right) Right = Right->FixedPointScale(Width, FullScale);
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

