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

#include "NamespacePush.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

NAMESPACE_PUSH::NAMESPACE_PUSH(int Line, std::string& Filename): NAMESPACE_PUSH(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

NAMESPACE_PUSH::NAMESPACE_PUSH(int Line, const char* Filename): BASE(Line, Filename, TYPE::NamespacePush){
  Namespace  = 0;
  Statements = 0;
}
//------------------------------------------------------------------------------

NAMESPACE_PUSH::~NAMESPACE_PUSH(){
  if(Namespace ) delete Namespace;
  if(Statements) delete Statements;
}
//------------------------------------------------------------------------------

BASE* NAMESPACE_PUSH::Copy(){
  NAMESPACE_PUSH* Copy = new NAMESPACE_PUSH(Source.Line, Source.Filename.c_str());

  if(Namespace ) Copy->Namespace  = (decltype(Namespace ))Namespace ->Copy();
  if(Statements) Copy->Statements = (decltype(Statements))Statements->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool NAMESPACE_PUSH::RunAST(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool NAMESPACE_PUSH::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void NAMESPACE_PUSH::Display(){
  DisplayInfo();
  Debug.Print("NamespacePush:\n ");

  if(Namespace){
    if(Namespace->Left || Namespace->Right) Debug.Print("(");
    Namespace->Display();
    if(Namespace->Left || Namespace->Right) Debug.Print(")");
  }

  Debug.Print(".{\n");
    if(Statements) Statements->Display();
  Debug.Print("}\n");

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void NAMESPACE_PUSH::ValidateMembers(){
  assert(Type == TYPE::NamespacePush);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

