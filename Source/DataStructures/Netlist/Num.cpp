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

#include "Num.h"

#include "AST/Expression/Literal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

NUM::NUM(int Line, const string& Filename, const char* Name) : BASE(Line, Filename, Name, TYPE::Number){
  Value = 0;
}
//------------------------------------------------------------------------------

NUM::~NUM(){
}
//------------------------------------------------------------------------------

AST::EXPRESSION* NUM::GetExpression(int Line, const string& Filename){
  AST::LITERAL* Result = new AST::LITERAL(Line, Filename);
  Result->Value = Value;
  return Result;
}
//------------------------------------------------------------------------------

bool NUM::Assign(AST::EXPRESSION* Expression){
  return RawAssign(Expression);
}
//------------------------------------------------------------------------------

bool NUM::RawAssign(AST::EXPRESSION* Expression){
  if(!Expression) return false;

  Expression = Expression->Evaluate();
  if(!Expression) return false;

  bool Result = false;

  if(Expression->Type == AST::BASE::TYPE::Literal){
    Value = ((AST::LITERAL*)Expression)->Value;
    Result = true;
  }else{
    Expression->Error("Expression must evaluate to a literal");
    Result = false;
  }

  // Expression has been moved and then used, so it must be deleted
  delete Expression;
  return Result;
}
//------------------------------------------------------------------------------

bool NUM::HasCircularReference(BASE* Object){
  if(this == Object) return true;
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void NUM::Display(int Indent){
  Debug.Indent(Indent);
  Debug.Print("Num: %s\n", Name.c_str());

  Debug.Indent(Indent+1);
  Debug.Print("Value = ");
  Debug.Print(Value.Display());
  Debug.Print("\n");
}
//------------------------------------------------------------------------------

void NUM::Validate(){
  assert(Type == TYPE::Number);

  BASE::Validate();
}
//------------------------------------------------------------------------------

