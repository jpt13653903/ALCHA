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

#include "OR_Assign.h"

#include "Netlist/Byte.h"
#include "Netlist/Character.h"
#include "Netlist/Num.h"
#include "Netlist/Synthesisable/Pin.h"
#include "Netlist/Synthesisable/Net.h"

#include "../Expression/Bit_OR.h"
#include "../Expression/Object.h"
#include "../Expression/Literal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

OR_ASSIGN::OR_ASSIGN(int Line, std::string Filename):
OR_ASSIGN(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

OR_ASSIGN::OR_ASSIGN(int Line, const char* Filename):
ASSIGNMENT(Line, Filename, TYPE::OR_Assign){}
//------------------------------------------------------------------------------

OR_ASSIGN::~OR_ASSIGN(){
}
//------------------------------------------------------------------------------

BASE* OR_ASSIGN::Copy(){
  OR_ASSIGN* Copy = new OR_ASSIGN(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool OR_ASSIGN::RunAST(){
  target_list TargetList;

  assert(Left , return false);
  assert(Right, return false);

  if(!GetLHS(Left, TargetList)) return false;
  if(TargetList.empty()){
    Error("Target object list is empty");
    return false;
  }

  if(TargetList.size() > 1){
    error("Multiple assignment targets not supported yet");
    return false;
  }

  NETLIST::BASE* Target = TargetList.front();
  assert(Target, return false);

  // Move the expression
  EXPRESSION* Expression = new BIT_OR(Right->Source.Line, Right->Source.Filename);
  Expression->Left = Target->GetExpression(Right->Source.Line, Right->Source.Filename);

  if(!Expression->Left){
    delete Expression;
    return false;
  }

  Expression->Right = Right;
  Right = 0;

  bool Result = Target->Assign(Expression);

  return Result;
}
//------------------------------------------------------------------------------

bool OR_ASSIGN::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void OR_ASSIGN::Display(){
  DisplayAssignment("|=");
}
//------------------------------------------------------------------------------

void OR_ASSIGN::ValidateMembers(){
  assert(Type == TYPE::OR_Assign);

  assert(Left, return);
  Left->Validate();

  if(Right) Right->Validate();
}
//------------------------------------------------------------------------------

