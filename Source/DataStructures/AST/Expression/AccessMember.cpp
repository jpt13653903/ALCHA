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

#include "AccessMember.h"
#include "Object.h"
#include "Identifier.h"
#include "Netlist/Alias.h"
#include "Netlist/Namespace/Module.h"
#include "Netlist/Namespace.h"
#include "Netlist/Synthesisable.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

ACCESSMEMBER::ACCESSMEMBER(int Line, const string& Filename): ACCESSMEMBER(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

ACCESSMEMBER::ACCESSMEMBER(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::AccessMember){
}
//------------------------------------------------------------------------------

ACCESSMEMBER::~ACCESSMEMBER(){
}
//------------------------------------------------------------------------------

BASE* ACCESSMEMBER::Copy(){
  ACCESSMEMBER* Copy = new ACCESSMEMBER(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool ACCESSMEMBER::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* ACCESSMEMBER::Evaluate(){
  assert(Left , delete this; return 0);
  assert(Right, delete this; return 0);

  Left = Left->Evaluate();

  assert(Left, delete this; return 0);

  if(Left->Type != TYPE::Object){
    Error("Invalid member access expression");
    delete this;
    return 0;
  }
  switch(Right->Type){
    case TYPE::Identifier:
      break;

    case TYPE::Slice:
      error("Not yet implemented");
      delete this;
      return 0;
      
    default:
      Error("Invalid member access expression");
      delete this;
      return 0;
  }
  
  auto Object = ((OBJECT    *)Left )->ObjectRef;
  auto Name   = ((IDENTIFIER*)Right)->Name;

  auto Member = Object->GetMember(Name);
  if(!Member){
    Error();
    printf("Member %s not found in object %s\n",
           Name.c_str(), Object->Name.c_str());
    delete this;
    return 0;
  }

  auto Result = new OBJECT(Source.Line, Source.Filename);
  Result->ObjectRef = Member;
  delete this;
  return Result->Evaluate();
}
//------------------------------------------------------------------------------

int ACCESSMEMBER::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

NUMBER& ACCESSMEMBER::GetFullScale(){
  error("Not yet implemented");
  static NUMBER zero = 0;
  return zero;
}
//------------------------------------------------------------------------------

bool ACCESSMEMBER::GetSigned(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool ACCESSMEMBER::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void ACCESSMEMBER::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

void ACCESSMEMBER::Display(){
  DisplayStart();

  Debug.Print("." );

  DisplayEnd();
}
//------------------------------------------------------------------------------

void ACCESSMEMBER::ValidateMembers(){
  assert(Type == TYPE::AccessMember);
  
  assert(!Next);
  assert(!Prev);

  // TODO: The "Left" assertion fails when the member cannot be found
  assert(Left , return); Left ->Validate();
  assert(Right, return); Right->Validate();
}
//------------------------------------------------------------------------------

