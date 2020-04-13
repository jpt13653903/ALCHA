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

#include "Num_Definition.h"

#include "Netlist/Namespace/Module.h"
#include "Netlist/Num.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

NUM_DEFINITION::NUM_DEFINITION(
  int     Line,
  string& Filename
): NUM_DEFINITION(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

NUM_DEFINITION::NUM_DEFINITION(
  int             Line,
  const char*     Filename
): DEFINITION(Line, Filename, TYPE::Num_Definition){
}
//------------------------------------------------------------------------------

NUM_DEFINITION::~NUM_DEFINITION(){
}
//------------------------------------------------------------------------------

BASE* NUM_DEFINITION::Copy(){
  NUM_DEFINITION* Copy = new NUM_DEFINITION(Source.Line, Source.Filename.c_str());

  CopyMembers(Copy);

  return Copy;
}
//------------------------------------------------------------------------------

bool NUM_DEFINITION::RunAST(){
  auto Identifier = Identifiers;

  while(Identifier){
    if(!VerifyNotDefined(Identifier)) return false;

    if(Identifier->Function){
      error("Not yet implemented");
      Identifier = Identifier->Next;
      continue;
    }

    auto Number = new NETLIST::NUM(Source.Line, Source.Filename, Identifier->Identifier.c_str());
    if(!Number->ApplyAttributes(Attributes)) Error("Invalid attributes");
    NETLIST::NamespaceStack.front()->Symbols[Number->Name] = Number;
    if(Identifier->Initialiser){
      if(!Identifier->Initialiser->RunAST()) return false;
    }

    Identifier = Identifier->Next;
  }
  return true;
}
//------------------------------------------------------------------------------

bool NUM_DEFINITION::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void NUM_DEFINITION::Display(){
  DisplayDefinition("Number");
}
//------------------------------------------------------------------------------

void NUM_DEFINITION::ValidateMembers(){
  assert(Type == TYPE::Num_Definition);

  DEFINITION::ValidateMembers();
}
//------------------------------------------------------------------------------

