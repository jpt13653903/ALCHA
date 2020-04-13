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

#include "Pin_Definition.h"

#include "Netlist/Byte.h"
#include "Netlist/Character.h"
#include "Netlist/Synthesisable/Pin.h"
#include "Netlist/Namespace/Module.h"
#include "Netlist/Synthesisable/Net.h"
#include "Netlist/Num.h"
#include "Netlist/Synthesisable.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

PIN_DEFINITION::PIN_DEFINITION(
  int     Line,
  string& Filename
): PIN_DEFINITION(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

PIN_DEFINITION::PIN_DEFINITION(
  int             Line,
  const char*     Filename
): DEFINITION(Line, Filename, TYPE::Pin_Definition){
}
//------------------------------------------------------------------------------

PIN_DEFINITION::~PIN_DEFINITION(){
}
//------------------------------------------------------------------------------

BASE* PIN_DEFINITION::Copy(bool CopyNext){
  PIN_DEFINITION* Copy = new PIN_DEFINITION(Source.Line, Source.Filename.c_str());

  Copy->Direction = Direction;

  if(Attributes ) Copy->Attributes  = (decltype(Attributes))Attributes->Copy(CopyNext);
  if(Identifiers) Copy->Identifiers = new IDENTIFIER(*Identifiers);

  if(CopyNext && Next){
    assert(false);
    // Copy->Next = Next->Copy(CopyNext);
  }

  foreach(Parameter, Parameters){
    if(*Parameter) Copy->Parameters.push_back((*Parameter)->Copy(CopyNext));
  }

  return Copy;
}
//------------------------------------------------------------------------------

bool PIN_DEFINITION::RunAST(){
  auto Identifier = Identifiers;

  while(Identifier){
    auto Symbol = NETLIST::NamespaceStack.front()->Symbols.find(Identifier->Identifier);
    if(Symbol != NETLIST::NamespaceStack.front()->Symbols.end()){
      Error();
      printf("Symbol \"%s\" already defined in the current namespace\n",
             Identifier->Identifier.c_str());
      return false;
    }

    if(Identifier->Function){
      error("Not yet implemented");
      Identifier = Identifier->Next;
      continue;
    }
    if(Identifier->Parameters) error("Not yet implemented");

    auto Pin = new NETLIST::PIN(Source.Line, Source.Filename, Identifier->Identifier.c_str());
    Pin->Direction = Direction;
    if(!Pin->ApplyParameters(Parameters)) Error("Invalid parameters");
    if(!Pin->ApplyAttributes(Attributes)) Error("Invalid attributes");
    NETLIST::NamespaceStack.front()->Symbols[Pin->Name] = Pin;
    if(Identifier->Initialiser){
      if(!Identifier->Initialiser->RunAST()) return false;
    }

    Identifier = Identifier->Next;
  }
  return true;
}
//------------------------------------------------------------------------------

bool PIN_DEFINITION::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void PIN_DEFINITION::Display(){
  DisplayDefinition("Pin");
}
//------------------------------------------------------------------------------

void PIN_DEFINITION::ValidateMembers(){
  assert(Type == TYPE::Pin_Definition);

  DEFINITION::ValidateMembers();
}
//------------------------------------------------------------------------------

