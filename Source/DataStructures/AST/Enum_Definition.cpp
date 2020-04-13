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

#include "Enum_Definition.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

ENUM_DEFINITION::VALUE::VALUE(){
  Next = 0;
}
//------------------------------------------------------------------------------

ENUM_DEFINITION::VALUE::VALUE(const VALUE& Value){
  Next = 0;

  if(Value.Next) Next = new VALUE(*Value.Next);
}
//------------------------------------------------------------------------------

ENUM_DEFINITION::VALUE::~VALUE(){
  if(Next) delete Next;
}
//------------------------------------------------------------------------------

ENUM_DEFINITION::ENUM_DEFINITION(int Line, std::string& Filename):
ENUM_DEFINITION(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

ENUM_DEFINITION::ENUM_DEFINITION(int Line, const char* Filename):
BASE(Line, Filename, TYPE::Enum_Definition){
  Values = 0;
}
//------------------------------------------------------------------------------

ENUM_DEFINITION::~ENUM_DEFINITION(){
  if(Values) delete Values;
}
//------------------------------------------------------------------------------

BASE* ENUM_DEFINITION::Copy(){
  ENUM_DEFINITION* Copy = new ENUM_DEFINITION(Source.Line, Source.Filename.c_str());

  Copy->Identifier = Identifier;

  if(Values) Copy->Values = new VALUE(*Values);

  return Copy;
}
//------------------------------------------------------------------------------

bool ENUM_DEFINITION::RunAST(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool ENUM_DEFINITION::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void ENUM_DEFINITION::Display(){
  DisplayInfo();
  Debug.Print("Enum Definition (%s):\n", Identifier.c_str());

  Debug.Print(" Values: ");
  VALUE* Value = Values;
  while(Value){
    Debug.Print("%s", Value->Identifier.c_str());
    Value = Value->Next;
    if(Value) Debug.Print(", ");
    else      Debug.Print("\n");
  }

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void ENUM_DEFINITION::ValidateMembers(){
  assert(Type == TYPE::Enum_Definition);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

