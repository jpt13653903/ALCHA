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

#include "Switch.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

SWITCH::CASE::CASE(){
  Next       = 0;
  Statements = 0;
}
//------------------------------------------------------------------------------

SWITCH::CASE::CASE(const CASE& Case){
  if(Case.Next) Next = new CASE(*Case.Next);

  Statements = CopyList(Case.Statements);

  foreach(Element, Case.Expressions){
    Expressions.push_back((EXPRESSION*)(*Element)->Copy());
  }
}
//------------------------------------------------------------------------------

SWITCH::CASE::~CASE(){
  if(Next       ) delete Next;
  if(Statements ) delete Statements;

  foreach(Element, Expressions) delete *Element;
}
//------------------------------------------------------------------------------

SWITCH::SWITCH(int Line, std::string& Filename): SWITCH(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

SWITCH::SWITCH(int Line, const char* Filename): BASE(Line, Filename, TYPE::Switch){
  Cases      = 0;
  Default    = 0;
  Expression = 0;
}
//------------------------------------------------------------------------------

SWITCH::~SWITCH(){
  if(Cases     ) delete Cases;
  if(Default   ) delete Default;
  if(Expression) delete Expression;
}
//------------------------------------------------------------------------------

BASE* SWITCH::Copy(){
  SWITCH* Copy = new SWITCH(Source.Line, Source.Filename.c_str());

  if(Cases     ) Copy->Cases      = new CASE(*Cases);
  if(Expression) Copy->Expression = (decltype(Expression))Expression->Copy();

  Copy->Default = CopyList(Default);

  return Copy;
}
//------------------------------------------------------------------------------

bool SWITCH::RunAST(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool SWITCH::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void SWITCH::Display(){
  DisplayInfo();
  Debug.Print("switch(");
    if(Expression) Expression->Display();
  Debug.Print("){\n");
    CASE* Temp = Cases;
    while(Temp){
      Debug.Print(" case(");
        bool isFirst = true;
        foreach(Expression, Temp->Expressions){
          if(!isFirst) Debug.Print(", ");
          (*Expression)->Display();
          isFirst = false;
        }
      Debug.Print("){\n");
        if(Temp->Statements) Temp->Statements->Display();
      Debug.Print(" }\n");
      Temp = Temp->Next;
    }
    Debug.Print(" default{\n");
      if(Default) Default->Display();
    Debug.Print(" }\n");
  Debug.Print("}\n");

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void SWITCH::ValidateMembers(){
  assert(Type == TYPE::Switch);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

