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
}
//------------------------------------------------------------------------------

AST::BASE* ENGINE::RunScripting(const char* Filename){
  AST::BASE* Ast = Parser.Run(Filename);
  if(!Ast) return 0;

  auto Node = Ast;
  while(Node){
    Node = Node->RunScripting();
    if(!Node){
      delete Ast;
      return 0;
    }
    Node = Node->Next;
  }
  return Ast;
}
//------------------------------------------------------------------------------

bool ENGINE::Run(const char* Filename){
  assert(NETLIST::NamespaceStack.empty());

  NETLIST::NamespaceStack.push_front(&NETLIST::Global);

  NETLIST::Global.Ast = RunScripting(Filename);

  #ifdef DEBUG
    Debug.print(ANSI_FG_GREEN "\nDisplaying Global AST ");
    Debug.print(Filename);
    Debug.print(" -------------------------------------\n\n" ANSI_RESET);
    if(NETLIST::Global.Ast) NETLIST::Global.Ast->Display();
    else                    Debug.print("AST is empty\n");
  #endif

  return NETLIST::Global.Ast;
}
//------------------------------------------------------------------------------

