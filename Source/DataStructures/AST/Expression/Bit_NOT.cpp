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

#include "Bit_NOT.h"
#include "Literal.h"
#include "Object.h"

#include "Netlist/Namespace/Module.h"
#include "Netlist/Synthesisable/Net.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

BIT_NOT::BIT_NOT(int Line, const string& Filename): BIT_NOT(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

BIT_NOT::BIT_NOT(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Bit_NOT){
}
//------------------------------------------------------------------------------

BIT_NOT::~BIT_NOT(){
}
//------------------------------------------------------------------------------

BASE* BIT_NOT::Copy(){
  BIT_NOT* Copy = new BIT_NOT(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool BIT_NOT::GetVerilog(string& Body){
  Body += "~(";
  Right->GetVerilog(Body);
  Body += ")";

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* BIT_NOT::Evaluate(){
  assert(Right, delete this; return 0);
  Right = Right->Evaluate();
  assert(Right, delete this; return 0);

  switch(Right->Type){
    case TYPE::Literal:{
      auto Result = (LITERAL*)Right;
      Right = 0;
      int ResultWidth = Result->GetWidth();
      if(Result->GetWidth()){
        NUMBER n(1);
        n.BinScale(ResultWidth);
        n.Sub(Result->Value);
        n.Sub(1);
        Result->Value = n;
      }else{
        Error("Taking bitwise NOT of literal with unknown width");
        delete Result;
        Result = 0;
      }
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

int BIT_NOT::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

EXPRESSION* BIT_NOT::FixedPointScale(int Width, NUMBER& FullScale){
  NUMBER Scale = 1;
  Scale.BinScale(Width);
  Scale.Div(FullScale);

  return ScaleWith(Scale, Width, FullScale);
}
//------------------------------------------------------------------------------

bool BIT_NOT::HasCircularReference(NETLIST::BASE* Object){
  assert(Right, return false);
  return Right->HasCircularReference(Object);
}
//------------------------------------------------------------------------------

void BIT_NOT::PopulateUsed(){
  assert(Right, return);
  Right->PopulateUsed();
}
//------------------------------------------------------------------------------

void BIT_NOT::Display(){
  DisplayStart();

  Debug.Print(" ~");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void BIT_NOT::ValidateMembers(){
  assert(Type == TYPE::Bit_NOT);

  assert(!Next);
  assert(!Prev);

  assert(!Left);
  assert(Right, return); Right->Validate();
}
//------------------------------------------------------------------------------

