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

#include "Class_Instance.h"

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

CLASS_INSTANCE::CLASS_INSTANCE(
  int     Line,
  string& Filename
): CLASS_INSTANCE(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

CLASS_INSTANCE::CLASS_INSTANCE(
  int             Line,
  const char*     Filename
): DEFINITION(Line, Filename, TYPE::Class_Instance){
  ClassName = 0;
}
//------------------------------------------------------------------------------

CLASS_INSTANCE::~CLASS_INSTANCE(){
  if(ClassName) delete ClassName;
}
//------------------------------------------------------------------------------

BASE* CLASS_INSTANCE::Copy(bool CopyNext){
  CLASS_INSTANCE* Copy = new CLASS_INSTANCE(Source.Line, Source.Filename.c_str());

  Copy->Direction = Direction;

  if(ClassName  ) Copy->ClassName   = (decltype(ClassName ))ClassName ->Copy(CopyNext);
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

bool CLASS_INSTANCE::RunAST(){
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

    error("Not yet implemented");

    Identifier = Identifier->Next;
  }
  return true;
}
//------------------------------------------------------------------------------

bool CLASS_INSTANCE::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void CLASS_INSTANCE::Display(){
  DisplayInfo();
  Debug.Print("Definition (");

  Debug.Print("Class instance definition (");
  if(ClassName) ClassName->Display();
  else          Debug.Print("Class instance with no class name");
  Debug.Print("):\n");

  void DisplayParameters ();
  void DisplayAttributes ();
  void DisplayIdentifiers();

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void CLASS_INSTANCE::ValidateMembers(){
  assert(Type == TYPE::Class_Instance);

  if(ClassName) ClassName->Validate();

  DEFINITION::ValidateMembers();
}
//------------------------------------------------------------------------------

