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
#include "Object.h"
#include "Netlist/Synthesisable/Net.h"
#include "Netlist/Namespace/Module.h"
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

BASE* BIT_NOT::Copy(bool CopyNext){
  BIT_NOT* Copy = new BIT_NOT(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next){
    assert(false);
    // Copy->Next = Next->Copy(CopyNext);
  }

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

// EXPRESSION* BIT_NOT::Simplify(bool GenWire){
//   assert(Right, return this);
// 
//   EXPRESSION* Result = this;
// 
//   Right = Right->Simplify(true);
//   assert(Right->Type == TYPE::Object, return Result);
// 
//   if(GenWire){
//     auto Net = new NETLIST::NET(Source.Line, Source.Filename, 0);
//     Net->Used  = true;
//     Net->Value = this;
// 
//     auto ObjectRef = ((OBJECT*)Right)->ObjectRef;
//     if(ObjectRef && ObjectRef->IsSynthesisable()){
//       auto Synthesisable = (NETLIST::SYNTHESISABLE*)ObjectRef;
//       Net->Width     = Synthesisable->Width;
//       Net->FullScale = Synthesisable->FullScale;
//     }
//     NETLIST::NamespaceStack.front()->Symbols[Net->Name] = Net;
// 
//     auto Object = new OBJECT(Source.Line, Source.Filename);
//     Object->ObjectRef = Net;
//     Result = Object;
//   }
//   return Result;
// }
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

