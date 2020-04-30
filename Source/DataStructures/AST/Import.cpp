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

#include "Import.h"
#include "Engine.h"
#include "Netlist/Namespace/Module.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

IMPORT::IMPORT(int Line, string& Filename): IMPORT(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

IMPORT::IMPORT(int Line, const char* Filename): BASE(Line, Filename, TYPE::Import){
  Ast = 0;
}
//------------------------------------------------------------------------------

IMPORT::~IMPORT(){
  if(Ast) delete Ast;
}
//------------------------------------------------------------------------------

BASE* IMPORT::Copy(){
  IMPORT* Copy = new IMPORT(Source.Line, Source.Filename.c_str());
  
  Copy->File      = File;
  Copy->Namespace = Namespace;

  return Copy;
}
//------------------------------------------------------------------------------

bool IMPORT::RunAST(){
  string ImportFilename;
  bool   OwnNamespace = !Namespace.empty();

  NETLIST::MODULE* Module = 0;

  if(OwnNamespace){
    auto Found = NETLIST::NamespaceStack.front()->Symbols.find(Namespace);
    if(Found != NETLIST::NamespaceStack.front()->Symbols.end()){
      Error();
      printf("Symbol \"%s\" already exists in the current namespace\n",
             Namespace.c_str());
      return false;
    }
    Module = new NETLIST::MODULE(Source.Line, Source.Filename, Namespace.c_str());
    NETLIST::NamespaceStack.front()->Symbols[Namespace] = Module;
    NETLIST::NamespaceStack.push_front(Module);
  }

  string& Path = Source.Filename;
  int n;
  for(n = Path.length()-1; n >= 0; n--){
    if(Path[n] == '/') break;
  }
  if(n >= 0) ImportFilename = Path.substr(0, n+1);
  ImportFilename += File;
  ImportFilename += ".alc";
  SimplifyFilename(ImportFilename);
  Debug.Print("\nFilename = %s\n", ImportFilename);

  auto Ast = Engine->RunAST(ImportFilename.c_str());
  if(!Ast) return false;

  if(OwnNamespace){
    Module->Ast = Ast;
    NETLIST::NamespaceStack.pop_front();

  }else{
    this->Ast = Ast;
  }
  return true;
}
//------------------------------------------------------------------------------

bool IMPORT::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void IMPORT::Display(){
  DisplayInfo();
  Debug.Print("import \"%s\"", File.c_str());
  if(Namespace.empty()) Debug.Print("\n");
  else                  Debug.Print(" as %s\n", Namespace.c_str());

  if(Ast ) Ast ->Display();
  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void IMPORT::ValidateMembers(){
  assert(Type == TYPE::Import);

  if(Ast) Ast->Validate();
}
//------------------------------------------------------------------------------

