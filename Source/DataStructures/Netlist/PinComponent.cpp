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

#include "PinComponent.h"

#include "Synthesisable/Pin.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

PIN_COMPONENT::PIN_COMPONENT(int Line, const string& Filename, const char* Name, PIN* Pin):
BASE(Line, Filename, Name, TYPE::PinComponent){
  this->Pin = Pin;
  Namespace = Pin->Namespace;
  Value     = 0;
}
//------------------------------------------------------------------------------

PIN_COMPONENT::~PIN_COMPONENT(){
  if(Value) delete Value;
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PIN_COMPONENT::GetExpression(int Line, const string& Filename){
  if(Value) return (AST::EXPRESSION*)Value->Copy();
  ::Error(Line, Filename, "Operate-assign on empty object");
  return 0;
}
//------------------------------------------------------------------------------

bool PIN_COMPONENT::Assign(AST::EXPRESSION* Expression){
  assert(Expression, return false);
  return RawAssign(Expression->FixedPointScale(Pin->Width(), Pin->FullScale()));
}
//------------------------------------------------------------------------------

bool PIN_COMPONENT::RawAssign(AST::EXPRESSION* Expression){
  Pin->Used = true;

  assert(Expression, return false);

  if(Value) delete Value;
  Value = Expression->Evaluate();
  if(Value){
    if(Value->HasCircularReference(this) ||
       Value->HasCircularReference(Pin ) ){
      Value->Error("Circular combinational circuit");
    }
  }
  return Value;
}
//------------------------------------------------------------------------------

bool PIN_COMPONENT::HasCircularReference(BASE* Object){
  Pin->Used = true;

  if(this == Object) return true;
  if(!Value) return false;
  return Value->HasCircularReference(Object);
}
//------------------------------------------------------------------------------

BASE* PIN_COMPONENT::GetAttribute(const std::string& Name){
  return Pin->GetAttribute(Name);
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PIN_COMPONENT::GetAttribValue(const std::string& Name){
  return Pin->GetAttribValue(Name);
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PIN_COMPONENT::GetBuiltInAttributeValue(const std::string& Name){
  return Pin->GetBuiltInAttributeValue(Name);
}
//------------------------------------------------------------------------------

int PIN_COMPONENT::Width(){
  return Pin->Width();
}
//------------------------------------------------------------------------------

NUMBER& PIN_COMPONENT::FullScale(){
  return Pin->FullScale();
}
//------------------------------------------------------------------------------

void PIN_COMPONENT::Display(int Indent){
  Debug.Indent(Indent);
  Debug.Print("Pin Component: %s\n", Name.c_str());

  Indent++;
  Debug.Indent(Indent);
  Debug.Print("Value = ");
  if(Value) Value->Display();
  else      Debug.Print("{null}");
  Debug.Print("\n");
}
//------------------------------------------------------------------------------

void PIN_COMPONENT::Validate(){
  assert(Type == TYPE::PinComponent);

  assert(Attributes.size() == 0);
  assert(Namespace == Pin->Namespace);

  BASE::Validate();
}
//------------------------------------------------------------------------------

