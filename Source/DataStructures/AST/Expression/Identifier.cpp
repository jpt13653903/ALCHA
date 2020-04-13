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

#include "Identifier.h"
#include "Object.h"
#include "Literal.h"

#include "Netlist/Alias.h"
#include "Netlist/Namespace/Module.h"
#include "Netlist/Synthesisable.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

IDENTIFIER::IDENTIFIER(int Line, const string& Filename): IDENTIFIER(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

IDENTIFIER::IDENTIFIER(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Identifier){
}
//------------------------------------------------------------------------------

IDENTIFIER::~IDENTIFIER(){
}
//------------------------------------------------------------------------------

BASE* IDENTIFIER::Copy(){
  IDENTIFIER* Copy = new IDENTIFIER(Source.Line, Source.Filename.c_str());

  Copy->Name = Name;

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool IDENTIFIER::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* IDENTIFIER::Evaluate(){
  foreach(NamespaceIterator, NETLIST::NamespaceStack){
    auto Namespace = *NamespaceIterator;
    while(Namespace){
      auto Object = Namespace->GetMember(Name);
      if(Object){
        auto Result = new OBJECT(Source.Line, Source.Filename);
        Result->ObjectRef = Object;
        delete this;
        // It might be an alias, or evaluate further to a literal
        return Result->Evaluate();
      }
      Namespace = Namespace->Namespace;
    }
  }
  Error();
  printf("Undefined identifier: \"%s\"\n", Name.c_str());

  delete this;
  return 0;
}
//------------------------------------------------------------------------------

void IDENTIFIER::Display(){
  DisplayStart();

  if(Name.empty()) error ("(Identifier node has no name)");
  else             Debug.Print("%s", Name.c_str());

  DisplayEnd();
}
//------------------------------------------------------------------------------

void IDENTIFIER::ValidateMembers(){
  assert(Type == TYPE::Identifier);

  assert(!Next);
  assert(!Prev);
  
  assert(!Left );
  assert(!Right);
}
//------------------------------------------------------------------------------

