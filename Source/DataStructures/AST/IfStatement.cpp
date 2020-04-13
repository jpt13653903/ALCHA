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

#include "IfStatement.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

IF_STATEMENT::IF_STATEMENT(int Line, std::string& Filename): IF_STATEMENT(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

IF_STATEMENT::IF_STATEMENT(int Line, const char* Filename): BASE(Line, Filename, TYPE::IfStatement){
  Condition       = 0;
  TrueStatements  = 0;
  FalseStatements = 0;
}
//------------------------------------------------------------------------------

IF_STATEMENT::~IF_STATEMENT(){
  if(Condition      ) delete Condition;
  if(TrueStatements ) delete TrueStatements;
  if(FalseStatements) delete FalseStatements;
}
//------------------------------------------------------------------------------

BASE* IF_STATEMENT::Copy(){
  IF_STATEMENT* Copy = new IF_STATEMENT(Source.Line, Source.Filename.c_str());

  if(Condition) Copy->Condition = (decltype(Condition))Condition->Copy();

  Copy->TrueStatements  = CopyList(TrueStatements );
  Copy->FalseStatements = CopyList(FalseStatements);

  return Copy;
}
//------------------------------------------------------------------------------

bool IF_STATEMENT::RunAST(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool IF_STATEMENT::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void IF_STATEMENT::Display(){
  DisplayInfo();
  Debug.Print("if(");
    if(Condition) Condition->Display();
  Debug.Print("){\n");
    if(TrueStatements) TrueStatements->Display();
  Debug.Print("}else{\n");
    if(FalseStatements) FalseStatements->Display();
  Debug.Print("}\n");

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void IF_STATEMENT::ValidateMembers(){
  assert(Type == TYPE::IfStatement);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

