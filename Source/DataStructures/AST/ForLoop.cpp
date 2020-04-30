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

#include "ForLoop.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

FOR_LOOP::FOR_LOOP(int Line, std::string& Filename): FOR_LOOP(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

FOR_LOOP::FOR_LOOP(int Line, const char* Filename): BASE(Line, Filename, TYPE::ForLoop){
  Range      = 0;
  Statements = 0;
}
//------------------------------------------------------------------------------

FOR_LOOP::~FOR_LOOP(){
  if(Range     ) delete Range;
  if(Statements) delete Statements;
}
//------------------------------------------------------------------------------

BASE* FOR_LOOP::Copy(){
  FOR_LOOP* Copy = new FOR_LOOP(Source.Line, Source.Filename.c_str());

  Copy->Identifier = Identifier;

  if(Range) Copy->Range = (decltype(Range))Range->Copy();

  Copy->Statements = CopyList(Statements);

  return Copy;
}
//------------------------------------------------------------------------------

bool FOR_LOOP::RunAST(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool FOR_LOOP::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void FOR_LOOP::Display(){
  DisplayInfo();
  Debug.Print("for(%s in ", Identifier.c_str());
    if(Range) Range->Display();
  Debug.Print("){\n");
    if(Statements) Statements->Display();
  Debug.Print("}\n");

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void FOR_LOOP::ValidateMembers(){
  assert(Type == TYPE::ForLoop);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

