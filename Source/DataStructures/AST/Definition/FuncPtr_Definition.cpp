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

#include "FuncPtr_Definition.h"

#include "Netlist/Namespace/Module.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

FUNCPTR_DEFINITION::FUNCPTR_DEFINITION(
  int     Line,
  string& Filename
): FUNCPTR_DEFINITION(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

FUNCPTR_DEFINITION::FUNCPTR_DEFINITION(
  int             Line,
  const char*     Filename
): DEFINITION(Line, Filename, TYPE::FuncPtr_Definition){
}
//------------------------------------------------------------------------------

FUNCPTR_DEFINITION::~FUNCPTR_DEFINITION(){
}
//------------------------------------------------------------------------------

BASE* FUNCPTR_DEFINITION::Copy(){
  FUNCPTR_DEFINITION* Copy = new FUNCPTR_DEFINITION(Source.Line, Source.Filename.c_str());

  CopyMembers(Copy);

  return Copy;
}
//------------------------------------------------------------------------------

bool FUNCPTR_DEFINITION::RunAST(){
  auto Identifier = Identifiers;

  while(Identifier){
    if(!VerifyNotDefined(Identifier)) return false;

    if(Identifier->Function){
      error("Not yet implemented");
      Identifier = Identifier->Next;
      continue;
    }

    error("Not yet implemented");

    Identifier = Identifier->Next;
  }
  return true;
}
//------------------------------------------------------------------------------

bool FUNCPTR_DEFINITION::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void FUNCPTR_DEFINITION::Display(){
  DisplayDefinition("Function");
}
//------------------------------------------------------------------------------

void FUNCPTR_DEFINITION::ValidateMembers(){
  assert(Type == TYPE::FuncPtr_Definition);

  assert(Parameters.empty());

  DEFINITION::ValidateMembers();
}
//------------------------------------------------------------------------------

