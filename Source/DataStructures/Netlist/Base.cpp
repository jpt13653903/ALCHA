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

#include "Base.h"
#include "Module.h"
#include "Expression.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

BASE::BASE(const char* Name, TYPE Type){
  this->Type      = Type;
  this->Name      = Name;
  this->Namespace = 0;

  if(!NamespaceStack.empty()) Namespace = NamespaceStack.front();
}
//------------------------------------------------------------------------------

BASE::~BASE(){
  for(auto a = Attributes.begin(); a != Attributes.end(); a++){
    delete a->second;
  }
}
//------------------------------------------------------------------------------

string& BASE::HDL_Name(){
  static string Result;

  if(!Namespace) return Name;

  Result.clear();

  switch(Namespace->Type){
    case TYPE::Module:
      return Name;

    case TYPE::Group:
      Result = Namespace->HDL_Name() + "_" + Name;
      break;

    default:
      break;
  }
  return Result;
}
//------------------------------------------------------------------------------

void BASE::DisplayLongName(){
  if(Namespace != (NAMESPACE*)&Global){
    Namespace->DisplayLongName();
    Debug.print("::");
  }
  Debug.print("%s", Name.c_str());
}
//------------------------------------------------------------------------------

void BASE::DisplayAttributes(int Indent){
  for(int n = 0; n < Indent; n++) Debug.print(" ");
  Debug.print("Attributes:\n");

  for(auto a = Attributes.begin(); a != Attributes.end(); a++){
    for(int n = 0; n < Indent; n++) Debug.print(" ");
    Debug.print("  %s = ", a->first.c_str());
    if(a->second) a->second->Display();
    else          Debug.print("{null}");
    Debug.print("\n");
  }
}
//------------------------------------------------------------------------------

EXPRESSION* BASE::GetAttrib(const string& Key){
  auto Value = Attributes.find(Key);
  if(Value != Attributes.end()) return Value->second;
  if(Namespace) return Namespace->GetAttrib(Key);
  return 0;
}
//------------------------------------------------------------------------------

