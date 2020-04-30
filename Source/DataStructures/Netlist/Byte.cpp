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

#include "Byte.h"

#include "AST/Expression/Literal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

NETLIST::BYTE::BYTE(int Line, const string& Filename, const char* Name) : BASE(Line, Filename, Name, TYPE::Byte){
  Value = 0;
}
//------------------------------------------------------------------------------

NETLIST::BYTE::~BYTE(){
}
//------------------------------------------------------------------------------

AST::EXPRESSION* NETLIST::BYTE::GetExpression(int Line, const string& Filename){
  AST::LITERAL* Result = new AST::LITERAL(Line, Filename);
  Result->Value = Value;
  Result->SetWidth(8);
  return Result;
}
//------------------------------------------------------------------------------

bool NETLIST::BYTE::Assign(AST::EXPRESSION* Expression){
  error("Not yet implemented");
  return RawAssign(Expression);
}
//------------------------------------------------------------------------------

bool NETLIST::BYTE::RawAssign(AST::EXPRESSION* Expression){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool NETLIST::BYTE::HasCircularReference(BASE* Object){
  if(this == Object) return true;
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void NETLIST::BYTE::Display(int Indent){
  Debug.Indent(Indent+1);
  Debug.Print("Byte: %s\n", Name.c_str());
  Debug.Indent(Indent+2);
  Debug.Print("Value = 0x%02X\n", Value);
}
//------------------------------------------------------------------------------

void NETLIST::BYTE::Validate(){
  assert(Type == TYPE::Byte);

  BASE::Validate();

  error("Not implemented yet");
}
//------------------------------------------------------------------------------

