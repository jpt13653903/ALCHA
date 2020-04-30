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

#include "Array.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

ARRAY::ARRAY(int Line, const string& Filename, const char* Name) : BASE(Line, Filename, Name, TYPE::Array){
  error("Not yet implemented");
}
//------------------------------------------------------------------------------

ARRAY::~ARRAY(){
}
//------------------------------------------------------------------------------

AST::EXPRESSION* ARRAY::GetExpression(int Line, const string& Filename){
  error("Not yet implemented");
  // TODO: Generate an AST::ARRAY expression and fill with elements
  return 0;
}
//------------------------------------------------------------------------------

bool ARRAY::Assign(AST::EXPRESSION* Expression){
  error("Not yet implemented");
  return RawAssign(Expression);
}
//------------------------------------------------------------------------------

bool ARRAY::RawAssign(AST::EXPRESSION* Expression){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool ARRAY::HasCircularReference(BASE* Object){
  if(this == Object) return true;
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void ARRAY::Display(int Indent){
  error("Not implemented yet");
}
//------------------------------------------------------------------------------

void ARRAY::Validate(){
  assert(Type == TYPE::Array);

  BASE::Validate();

  error("Not implemented yet");
}
//------------------------------------------------------------------------------

