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
        auto ObjResult = new OBJECT(Source.Line, Source.Filename);
        ObjResult->ObjectRef = Object;
        // It might be an alias, or evaluate further to a literal
        auto Result = ObjResult->Evaluate();
        if(Result){ // The source reference gets messed up during alias expansion
          Result->Source.Line     = Source.Line;
          Result->Source.Filename = Source.Filename;
        }
        delete this;
        return Result;
      }
      Namespace = Namespace->Namespace;
    }
  }

  // Identifier not found, so try the constants table
  NUMBER Constant;
  if(Constants.GetConstant(Name, &Constant)){
    auto Result = new LITERAL(Source.Line, Source.Filename);
    Result->Value = Constant;
    delete this;
    return Result;
  }

  Error();
  printf("Undefined identifier: \"%s\"\n", Name.c_str());

  delete this;
  return 0;
}
//------------------------------------------------------------------------------

int IDENTIFIER::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

NUMBER& IDENTIFIER::GetFullScale(){
  error("Not yet implemented");
  static NUMBER zero = 0;
  return zero;
}
//------------------------------------------------------------------------------

bool IDENTIFIER::GetSigned(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool IDENTIFIER::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void IDENTIFIER::PopulateUsed(){
  error("Not yet implemented");
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

