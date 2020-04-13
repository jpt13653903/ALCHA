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

#include "WhileLoop.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

WHILE_LOOP::WHILE_LOOP(int Line, std::string& Filename): WHILE_LOOP(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

WHILE_LOOP::WHILE_LOOP(int Line, const char* Filename): BASE(Line, Filename, TYPE::WhileLoop){
  Condition  = 0;
  Statements = 0;
}
//------------------------------------------------------------------------------

WHILE_LOOP::~WHILE_LOOP(){
  if(Condition ) delete Condition;
  if(Statements) delete Statements;
}
//------------------------------------------------------------------------------

BASE* WHILE_LOOP::Copy(){
  WHILE_LOOP* Copy = new WHILE_LOOP(Source.Line, Source.Filename.c_str());

  if(Condition) Copy->Condition = (decltype(Condition))Condition->Copy();

  Copy->Statements = CopyList(Statements);

  return Copy;
}
//------------------------------------------------------------------------------

bool WHILE_LOOP::RunAST(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool WHILE_LOOP::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void WHILE_LOOP::Display(){
  DisplayInfo();
  Debug.Print("while(");
    if(Condition) Condition->Display();
  Debug.Print("){\n");
    if(Statements) Statements->Display();
  Debug.Print("}\n");

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void WHILE_LOOP::ValidateMembers(){
  assert(Type == TYPE::WhileLoop);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

