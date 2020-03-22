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
}
//------------------------------------------------------------------------------

IMPORT::~IMPORT(){
}
//------------------------------------------------------------------------------

BASE* IMPORT::Copy(bool CopyNext){
  IMPORT* Copy = new IMPORT(Source.Line, Source.Filename.c_str());
  
  Copy->File      = File;
  Copy->Namespace = Namespace;

  if(CopyNext && Next){
    assert(false);
    // Copy->Next = Next->Copy(CopyNext);
  }

  return Copy;
}
//------------------------------------------------------------------------------

BASE* IMPORT::RunScripting(){
  string ImportFilename;
  bool   OwnNamespace = !Namespace.empty();

  NETLIST::MODULE* Module = 0;

  if(OwnNamespace){
    auto Found = NETLIST::NamespaceStack.front()->Symbols.find(Namespace);
    if(Found != NETLIST::NamespaceStack.front()->Symbols.end()){
      Error();
      printf("Symbol \"%s\" already exists in the current namespace\n",
             Namespace.c_str());
      return 0;
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
  Debug.print("\nFilename = %s\n", ImportFilename);

  auto Ast = Engine->RunScripting(ImportFilename.c_str());
  if(!Ast) return 0;

  if(OwnNamespace){
    Module->Ast = Ast;
    NETLIST::NamespaceStack.pop_front();

  }else{ // Inject into the current namespace, after the "import" statement
    // TODO: Inject in place of the import node instead of after
    //       But what to return when OwnNamespace is true???
    auto AstTail = Ast;
    while(AstTail->Next) AstTail = AstTail->Next;
    AstTail->Next = Next;
    AstTail->Next->Prev = AstTail;
    Next = Ast;
    Ast->Prev = this;
    return AstTail;
  }
  return this;
}
//------------------------------------------------------------------------------

bool IMPORT::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void IMPORT::Display(){
  DisplayInfo();
  Debug.print("import \"%s\"", File.c_str());
  if(Namespace.empty()) Debug.print("\n");
  else                  Debug.print(" as %s\n", Namespace.c_str());

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void IMPORT::ValidateMembers(){
  assert(Type == TYPE::Import);
}
//------------------------------------------------------------------------------

