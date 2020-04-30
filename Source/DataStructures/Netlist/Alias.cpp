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

#include "Alias.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

ALIAS::ALIAS(int Line, const string& Filename, const char* Name, AST::EXPRESSION* Expression): BASE(Line, Filename, Name, TYPE::Alias){
  this->Expression = Expression;
}
//------------------------------------------------------------------------------

ALIAS::~ALIAS(){
  if(Expression) delete Expression;
}
//------------------------------------------------------------------------------

AST::EXPRESSION* ALIAS::GetExpression(int Line, const string& Filename){
  assert(Expression, return 0);
  return (AST::EXPRESSION*)Expression->Copy();
}
//------------------------------------------------------------------------------

bool ALIAS::Assign(AST::EXPRESSION* Expression){
  return RawAssign(Expression);
}
//------------------------------------------------------------------------------

bool ALIAS::RawAssign(AST::EXPRESSION* Expression){
  Expression->Error("Cannot assign to an alias");
  return false;
}
//------------------------------------------------------------------------------

bool ALIAS::HasCircularReference(BASE* Object){
  if(this == Object) return true;
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void ALIAS::Display(int Indent){
  Debug.Indent(Indent);
  Debug.Print("Alias: %s\n", Name.c_str());

  Debug.Indent(Indent+1);
  if(Expression) Expression->Display();
  else           Debug.Print("{null}");
  Debug.Print("\n");
}
//------------------------------------------------------------------------------

void ALIAS::Validate(){
  assert(Type == TYPE::Alias);

  BASE::Validate();

  if(Expression) Expression->Validate();
}
//------------------------------------------------------------------------------

