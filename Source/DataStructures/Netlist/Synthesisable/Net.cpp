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

#include "Net.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

NET::NET(int Line, const string& Filename, const char* Name) : SYNTHESISABLE(Line, Filename, Name, TYPE::Net){
  Value = 0;
}
//------------------------------------------------------------------------------

NET::~NET(){
  if(Value) delete Value;
}
//------------------------------------------------------------------------------

AST::EXPRESSION* NET::GetExpression(int Line, const string& Filename){
  if(Value) return (AST::EXPRESSION*)Value->Copy(false);
  Error(Line, Filename, "Cannot get expression: no value has been assigned yet");
  return 0;
}
//------------------------------------------------------------------------------

bool NET::Assign(AST::EXPRESSION* Expression){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void NET::Display(int Indent){
  Debug.Indent(Indent);
  Debug.Print("Net: ");

  Indent++;
  DisplayParameters(Indent);

  Debug.Indent(Indent);
  Debug.Print("Value      = ");
  if(Value) Value->Display();
  else      Debug.Print("{open}");
  Debug.Print("\n");

  DisplayAttributes(Indent);
}
//------------------------------------------------------------------------------

void NET::Validate(){
  assert(Type == TYPE::Net);

  SYNTHESISABLE::Validate();

  if(Value) Value->Validate();
}
//------------------------------------------------------------------------------

