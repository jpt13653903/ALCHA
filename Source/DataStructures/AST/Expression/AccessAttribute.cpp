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

BASE* ACCESSATTRIBUTE::Copy(bool CopyNext){
  ACCESSATTRIBUTE* Copy = new ACCESSATTRIBUTE(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next){
    assert(false);
    // Copy->Next = Next->Copy(CopyNext);
  }

  return Copy;
}
//------------------------------------------------------------------------------

bool ACCESSATTRIBUTE::RunAST(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool ACCESSATTRIBUTE::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* ACCESSATTRIBUTE::Evaluate(){
  EXPRESSION* Result = 0;

  EXPRESSION* Left = 0;
  if(this->Left) Left = this->Left->Evaluate();

  if(!Left || !this->Right || !this->Right->IsExpression()){
    error("Invalid attribute access expression");
    delete Left;
    return 0;
  }
  auto Right = (EXPRESSION*)this->Right;

  if(Left->Type == TYPE::Object){
    assert(Right->Type == TYPE::Identifier, delete Left; return 0);
    auto Right = (IDENTIFIER*)this->Right;
    auto Object = ((OBJECT*)Left)->ObjectRef;
    auto Found  = Object->GetAttrib(Right->Name);
    if(!Found){
      Error();
      printf("Attribute %s not found in object %s\n",
             Right->Name.c_str(), Object->Name.c_str());
      delete Left;
      return 0;
    }
    Result = (EXPRESSION*)Found->Copy(true);
    assert(Result->IsExpression());
    delete Left;
  }else{
    // TODO Could be a slice expression, which is not supported yet
    error("Unimplemented attribute access expression");
    delete Left;
    return 0;
  }

  if(!Result) return 0;
  return Result->Simplify(false);
}
//------------------------------------------------------------------------------

EXPRESSION* ACCESSATTRIBUTE::Simplify(bool GenWire){
  error("Not yet implemented");
  return this;
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

