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

#include "Object.h"
#include "Literal.h"

#include "Netlist/Alias.h"
#include "Netlist/Byte.h"
#include "Netlist/Character.h"
#include "Netlist/Num.h"
#include "Netlist/Namespace/Module.h"
#include "Netlist/Synthesisable/Net.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

OBJECT::OBJECT(int Line, const string& Filename): OBJECT(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

OBJECT::OBJECT(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Object){
  ObjectRef = 0;
}
//------------------------------------------------------------------------------

OBJECT::~OBJECT(){
  // Don't delete the Object reference -- it's part of the namespace tree
}
//------------------------------------------------------------------------------

BASE* OBJECT::Copy(){
  OBJECT* Copy = new OBJECT(Source.Line, Source.Filename.c_str());

  Copy->ObjectRef = ObjectRef;

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool OBJECT::GetVerilog(string& Body){
  assert(ObjectRef, return false);
  Body += ObjectRef->EscapedName();

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* OBJECT::Evaluate(){
  assert(ObjectRef, delete this; return 0);

  switch(ObjectRef->Type){
    case NETLIST::BASE::TYPE::Byte:{
      auto Byte = (NETLIST::BYTE*)ObjectRef;
      auto Result = new LITERAL(Source.Line, Source.Filename);
      Result->Value = Byte->Value;
      Result->SetWidth(8);
      delete this;
      return Result;
    }
    case NETLIST::BASE::TYPE::Character:{
      auto Char = (NETLIST::CHARACTER*)ObjectRef;
      auto Result = new LITERAL(Source.Line, Source.Filename);
      Result->Value = Char->Value;
      Result->SetWidth(32);
      delete this;
      return Result;
    }
    case NETLIST::BASE::TYPE::Number:{
      auto Num = (NETLIST::NUM*)ObjectRef;
      auto Result = new LITERAL(Source.Line, Source.Filename);
      Result->Value = Num->Value;
      delete this;
      return Result;
    }
    case NETLIST::BASE::TYPE::Alias:{
      auto Alias  = (NETLIST::ALIAS*)ObjectRef;
      auto Result = Alias->Expression;
      if(!Result){
        Error("Alias has no expression");
        delete this;
        return 0;
      }
      Result = (EXPRESSION*)Result->Copy();
      NETLIST::NamespaceStack.push_front(Alias->Namespace);
        Result = Result->Evaluate();
      NETLIST::NamespaceStack.pop_front();
      if(!Result){
        Error("Error evaluating alias");
        delete this;
        return 0;
      }
      delete this;
      return Result;
    }
    default:
      break;
  }
  return this;
}
//------------------------------------------------------------------------------

int OBJECT::GetWidth(){
  assert(ObjectRef, return 0);
  return ObjectRef->Width();
}
//------------------------------------------------------------------------------

NUMBER& OBJECT::GetFullScale(){
  assert(ObjectRef);
  return ObjectRef->FullScale();
}
//------------------------------------------------------------------------------

bool OBJECT::GetSigned(){
  assert(ObjectRef, return false);
  return ObjectRef->Signed();
}
//------------------------------------------------------------------------------

bool OBJECT::HasCircularReference(NETLIST::BASE* Object){
  if(!ObjectRef) return false;
  return ObjectRef->HasCircularReference(Object);
}
//------------------------------------------------------------------------------

void OBJECT::PopulateUsed(){
  if(ObjectRef) ObjectRef->PopulateUsed(true);
}
//------------------------------------------------------------------------------

EXPRESSION* OBJECT::RemoveTempNet(int Width, bool Signed){
  // At this point, everything has been broken down to raw bits, so the
  // full-scale is not important.

  if(ObjectRef && ObjectRef->Type == NETLIST::BASE::TYPE::Net){
    auto Net = (NETLIST::NET*)ObjectRef;
    if(Net->Value && Net->IsTemporary()){
      if(Width  == Net->Width () &&
         Signed == Net->Signed() ){
        delete this;
        return ((EXPRESSION*)Net->Value->Copy())->RemoveTempNet(Width, Signed);
      }
      if(Net->Value->Type == TYPE::Object){
        auto Object = (OBJECT*)Net->Value;
        if(GetWidth () == Object->GetWidth () &&
           GetSigned() == Object->GetSigned() ){
          delete this;
          return ((EXPRESSION*)Object->Copy())->RemoveTempNet(Width, Signed);
        }
      }
    }
  }
  return this;
}
//------------------------------------------------------------------------------

void OBJECT::Display(){
  DisplayStart();

  if(ObjectRef){
    // TODO: Display the width here, because it should be a property
    //       of the target object, not the expression itself
    ObjectRef->DisplayLongName();
  }else{
    error("Null object reference");
  }

  DisplayEnd();
}
//------------------------------------------------------------------------------

void OBJECT::ValidateMembers(){
  assert(Type == TYPE::Object);

  assert(!Next);
  assert(!Prev);

  assert(!Left );
  assert(!Right);

  assert(ObjectRef);
}
//------------------------------------------------------------------------------

