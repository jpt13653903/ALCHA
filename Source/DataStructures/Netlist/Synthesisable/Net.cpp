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
  if(Value) return (AST::EXPRESSION*)Value->Copy();
  ::Error(Line, Filename, "Operate-assign on empty object");
  return 0;
}
//------------------------------------------------------------------------------

bool NET::Assign(AST::EXPRESSION* Expression){
  assert(Expression, return false);
  return RawAssign(Expression->FixedPointScale(Width(), FullScale()));
}
//------------------------------------------------------------------------------

bool NET::RawAssign(AST::EXPRESSION* Expression){
  if(!Expression) return false;

  if(Value){
    Expression->Warning();
    printf("Overwriting net value %s\n", Name.c_str());
    delete Value;
  }
  Value = Expression->Evaluate();
  if(Value && Value->HasCircularReference(this)){
    Value->Error("Circular combinational circuit");
  }
  return Value;
}
//------------------------------------------------------------------------------

bool NET::HasCircularReference(BASE* Object){
  if(this == Object) return true;
  if(!Value) return false;
  return Value->HasCircularReference(Object);
}
//------------------------------------------------------------------------------

void NET::PopulateUsed(bool SetUsed){
  if(Used) return; // Prevents circular loops
  Used = SetUsed;
  if(Value) Value->PopulateUsed();
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

