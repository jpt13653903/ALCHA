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

#include "Pin.h"

#include "AST/Expression/Object.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

PIN::PIN(int Line, const string& Filename, const char* Name) : SYNTHESISABLE(Line, Filename, Name, TYPE::Pin){
  Driver  = new PIN_COMPONENT(Line, Filename, "Driver" , this);
  Enabled = new PIN_COMPONENT(Line, Filename, "Enabled", this);
}
//------------------------------------------------------------------------------

PIN::~PIN(){
  delete Driver;
  delete Enabled;
}
//------------------------------------------------------------------------------

AST::EXPRESSION* PIN::GetExpression(int Line, const string& Filename){
  return (AST::EXPRESSION*)Driver->GetExpression(Line, Filename);
}
//------------------------------------------------------------------------------

bool PIN::Assign(AST::EXPRESSION* Expression){
  if(Direction == AST::DEFINITION::DIRECTION::Input){
    Expression->Error("Cannot assign to an input pin");
    return false;
  }
  return Driver->Assign(Expression);
}
//------------------------------------------------------------------------------

bool PIN::RawAssign(AST::EXPRESSION* Expression){
  if(Direction == AST::DEFINITION::DIRECTION::Input){
    Expression->Error("Cannot assign to an input pin");
    return false;
  }
  return Driver->RawAssign(Expression);
}
//------------------------------------------------------------------------------

bool PIN::HasCircularReference(BASE* Object){
  Used = true;

  if(this == Object) return true;
  if(Driver ->HasCircularReference(Object)) return true;
  if(Enabled->HasCircularReference(Object)) return true;
  return false;
}
//------------------------------------------------------------------------------

BASE* PIN::GetMember(const std::string& Name){
  if(Name == "driver" ) return Driver;
  if(Name == "enabled") return Enabled;
  return 0;
}
//------------------------------------------------------------------------------

void PIN::Display(int Indent){
  Debug.Indent(Indent);
  Debug.Print("Pin: ");

  Indent++;
  DisplayParameters(Indent);
  DisplayAttributes(Indent);

  Driver ->Display(Indent);
  Enabled->Display(Indent);
}
//------------------------------------------------------------------------------

void PIN::Validate(){
  assert(Type == TYPE::Pin);

  SYNTHESISABLE::Validate();

  Driver ->Validate();
  Enabled->Validate();
}
//------------------------------------------------------------------------------

