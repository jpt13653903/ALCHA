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

#include "Engine.h"
#include "Netlist/Namespace/Module.h"
//------------------------------------------------------------------------------

ENGINE* Engine;
//------------------------------------------------------------------------------

using namespace std;
//------------------------------------------------------------------------------

ENGINE::ENGINE(){
  Engine = this; // There is only one instance, created in ALCHA.cpp::main()
}
//------------------------------------------------------------------------------

ENGINE::~ENGINE(){
  while(!AstStack.empty()){
    delete AstStack.top();
    AstStack.pop();
  }
}
//------------------------------------------------------------------------------

bool ENGINE::Run(const char* Filename){
  if(NETLIST::NamespaceStack.empty()){
    NETLIST::NamespaceStack.push_front(&NETLIST::Global);
  }

  AST::BASE* Ast = Parser.Run(Filename);
  if(Ast) AstStack.push(Ast);
  else    return false;

  while(Ast){
    if(!Ast->RunScripting()) return false;
    Ast = Ast->Next;
  }
  return true;
}
//------------------------------------------------------------------------------

