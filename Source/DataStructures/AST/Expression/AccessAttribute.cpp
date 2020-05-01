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

#include "AccessAttribute.h"
#include "Object.h"
#include "Identifier.h"
#include "Netlist/Base.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

ACCESSATTRIBUTE::ACCESSATTRIBUTE(int Line, const string& Filename): ACCESSATTRIBUTE(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

ACCESSATTRIBUTE::ACCESSATTRIBUTE(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::AccessAttribute){
}
//------------------------------------------------------------------------------

ACCESSATTRIBUTE::~ACCESSATTRIBUTE(){
}
//------------------------------------------------------------------------------

BASE* ACCESSATTRIBUTE::Copy(){
  ACCESSATTRIBUTE* Copy = new ACCESSATTRIBUTE(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool ACCESSATTRIBUTE::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* ACCESSATTRIBUTE::Evaluate(){
  assert(Left , delete this; return 0);
  assert(Right, delete this; return 0);

  Left = Left->Evaluate();

  assert(Left, delete this; return 0);

  if(Left->Type != TYPE::Object){
    Error("Invalid attribute access expression");
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
      Error("Invalid attribute access expression");
      delete this;
      return 0;
  }
  
  auto Object = ((OBJECT    *)Left )->ObjectRef;
  auto Name   = ((IDENTIFIER*)Right)->Name;

  auto Result = Object->GetBuiltInAttributeValue(Name);
  if(Result){
    delete this;
    return Result;
  }

  Result = Object->GetAttribValue(Name);
  if(Result){
    delete this;
    return (decltype(Result))Result->Copy();
  }

  Error();
  printf("Attribute %s not found in object %s\n",
         Name.c_str(), Object->Name.c_str());
  delete this;
  return 0;
}
//------------------------------------------------------------------------------

int ACCESSATTRIBUTE::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

EXPRESSION* ACCESSATTRIBUTE::FixedPointScale(int Width, NUMBER& FullScale){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

bool ACCESSATTRIBUTE::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void ACCESSATTRIBUTE::PopulateUsed(){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

void ACCESSATTRIBUTE::Display(){
  DisplayStart();

  Debug.Print("'" );

  DisplayEnd();
}
//------------------------------------------------------------------------------

void ACCESSATTRIBUTE::ValidateMembers(){
  assert(Type == TYPE::AccessAttribute);
  
  assert(!Next);
  assert(!Prev);

  if(Left) Left->Validate();
  assert(Right, return); Right->Validate();
}
//------------------------------------------------------------------------------

