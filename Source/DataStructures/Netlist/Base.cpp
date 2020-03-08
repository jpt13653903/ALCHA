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
#include "AST/Expression/Identifier.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

BASE::BASE(int Line, const string& Filename, const char* Name, TYPE Type){
  this->Line      = Line;
  this->Filename  = Filename;
  this->Name      = Name;
  this->Type      = Type;
  this->Namespace = 0;

  if(!NamespaceStack.empty()) Namespace = NamespaceStack.front();
}
//------------------------------------------------------------------------------

BASE::~BASE(){
  foreach(a, Attributes) delete a->second;
}
//------------------------------------------------------------------------------

bool BASE::ApplyAttributes(
  string&          Name,
  AST::EXPRESSION* Value,
  AST::BASE*       Ast
){
  if(!Value){
    Ast->Error("Invalid attribute expression");
    return false;
  }

  switch(Value->Type){
    case AST::BASE::TYPE::String:
    case AST::BASE::TYPE::Literal:
      break;

    case AST::BASE::TYPE::Array:
      // TODO Make sure that the array only contains strings or literals
      break;

    default:
      Ast->Error("Attribute values must be strings, literals or arrays");
      delete Value;
      return false;
  }
  Attributes[Name] = Value;
  return true;
}
//------------------------------------------------------------------------------

bool BASE::ApplyAttributes(AST::ASSIGNMENT* AttributeList){
  while(AttributeList){
    if(!AttributeList->Left){
      error("Null attribute name");
      return false;
    }
    if(!AttributeList->Right){
      error("Null attribute value");
      return false;
    }
    if(AttributeList->Left->Type != AST::BASE::TYPE::Identifier){
      error("Attribute LHS not an identifier");
      return false;
    }
    assert(AttributeList->Left->Type == AST::BASE::TYPE::Identifier, return false);
    auto AttributeList__Left = (AST::IDENTIFIER*)AttributeList->Left;
    auto a = Attributes.find(AttributeList__Left->Name);
    if(a != Attributes.end()){
      AttributeList->Warning();
      printf("Overwriting attribute %s\n", AttributeList__Left->Name.c_str());
      if(a->second) delete a->second;
    }

    if(!ApplyAttributes(
      AttributeList__Left->Name,
      AttributeList->Right->Evaluate(),
      AttributeList
    )) return false;

    AttributeList = (AST::ASSIGNMENT*)AttributeList->Next;
  }
  return true;
}
//------------------------------------------------------------------------------

string& BASE::HDL_Name(){
  static string Result;
  string SafeName; // Unicode converted to hex

  int n;
  for(n = 0; Name[n]; n++){
    if(Name[n] & 0x80) break;
  }
  if(Name[n]){ // Escape Unicode characters
    char s[16];
    SafeName = "u..";
    for(int n = 0; Name[n]; n++){
      sprintf(s, "%02X", (unsigned)((uint8_t)Name[n]));
      SafeName += s;
    }
  }else{
    SafeName = Name;
  }

  if(Namespace){
    switch(Namespace->Type){
      case TYPE::Module: Result = SafeName; break;
      case TYPE::Group:  Result = Namespace->HDL_Name() + "." + SafeName; break;
      default:;
    }
  }else{
    Result = SafeName;
  }
  return Result;
}
//------------------------------------------------------------------------------

string& BASE::EscapedName(){
  static string Result;

  string Name(HDL_Name());

  for(int n = 0; Name[n]; n++){
    if(Name[n] >= '0' && Name[n] <= '9' && n > 0) continue;
    if(Name[n] >= 'a' && Name[n] <= 'z') continue;
    if(Name[n] >= 'A' && Name[n] <= 'Z') continue;
    if(Name[n] == '_') continue;
    Result = "\\" + Name + " ";
    return Result;
  }
  Result = Name;
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
  Debug.print("Line       = %d\n", Line);

  for(int n = 0; n < Indent; n++) Debug.print(" ");
  Debug.print("Filename   = \"%s\"\n", Filename);

  for(int n = 0; n < Indent; n++) Debug.print(" ");
  Debug.print("Attributes:\n");

  foreach(a, Attributes){
    for(int n = 0; n < Indent; n++) Debug.print(" ");
    Debug.print("  %s = ", a->first.c_str());
    if(a->second) a->second->Display();
    else          Debug.print("{null}");
    Debug.print("\n");
  }
}
//------------------------------------------------------------------------------

AST::EXPRESSION* BASE::GetAttrib(const string& Key){
  auto Value = Attributes.find(Key);
  if(Value != Attributes.end()) return Value->second;
  if(Namespace) return Namespace->GetAttrib(Key);
  return 0;
}
//------------------------------------------------------------------------------

