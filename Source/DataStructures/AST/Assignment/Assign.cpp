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

#include "Assign.h"

#include "Netlist/Byte.h"
#include "Netlist/Character.h"
#include "Netlist/Num.h"
#include "Netlist/Synthesisable/Pin.h"
#include "Netlist/Synthesisable/Net.h"

#include "../Expression/Object.h"
#include "../Expression/Literal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

ASSIGN::ASSIGN(int Line, std::string Filename):
ASSIGN(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

ASSIGN::ASSIGN(int Line, const char* Filename):
ASSIGNMENT(Line, Filename, TYPE::Assign){}
//------------------------------------------------------------------------------

ASSIGN::~ASSIGN(){
}
//------------------------------------------------------------------------------

BASE* ASSIGN::Copy(){
  ASSIGN* Copy = new ASSIGN(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool ASSIGN::RunAST(){
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
  bool Result = Target->Assign(Right);
  Right  = 0;

  return Result;
}
//------------------------------------------------------------------------------

bool ASSIGN::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void ASSIGN::Display(){
  DisplayAssignment("=");
}
//------------------------------------------------------------------------------

void ASSIGN::ValidateMembers(){
  assert(Type == TYPE::Assign);

  assert(Left, return);
  Left->Validate();

  if(Right) Right->Validate();
}
//------------------------------------------------------------------------------

