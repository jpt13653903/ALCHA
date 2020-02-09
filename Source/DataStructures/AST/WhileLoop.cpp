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

using namespace AST;
//------------------------------------------------------------------------------

WHILE_LOOP::WHILE_LOOP(int Line, const char* Filename):
BASE(Line, Filename, TYPE::WhileLoop){
  Condition  = 0;
  Statements = 0;
}
//------------------------------------------------------------------------------

WHILE_LOOP::~WHILE_LOOP(){
  if(Condition ) delete Condition;
  if(Statements) delete Statements;
}
//------------------------------------------------------------------------------

BASE* WHILE_LOOP::Copy(bool CopyNext){
  WHILE_LOOP* Copy = new WHILE_LOOP(Line, Filename.c_str());

  if(Condition)  Copy->Condition  = (decltype(Condition ))Condition ->Copy(CopyNext);
  if(Statements) Copy->Statements = (decltype(Statements))Statements->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

void WHILE_LOOP::Display(){
  DisplayInfo();
  Debug.print("while(");
    if(Condition) Condition->Display();
  Debug.print("){\n");
    if(Statements) Statements->Display();
  Debug.print("}\n");

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------
