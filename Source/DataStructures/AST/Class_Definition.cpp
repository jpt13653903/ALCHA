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

#include "Class_Definition.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

CLASS_DEFINITION::PARENT::PARENT(){
  ClassName  = 0;
  Next       = 0;
}
//------------------------------------------------------------------------------

CLASS_DEFINITION::PARENT::PARENT(const PARENT& Parent){
  ClassName  = 0;
  Next       = 0;

  if(Parent.ClassName) ClassName  = (decltype(Parent.ClassName))Parent.ClassName->Copy();
  if(Parent.Next     ) Next       = new PARENT(*Parent.Next);

  foreach(Parameter, Parent.Parameters){
    if(*Parameter) Parameters.push_back((*Parameter)->Copy());
  }
}
//------------------------------------------------------------------------------

CLASS_DEFINITION::PARENT::~PARENT(){
  if(ClassName ) delete ClassName;
  if(Next      ) delete Next;

  foreach(Parameter, Parameters){
    if(*Parameter) delete *Parameter;
  }
}
//------------------------------------------------------------------------------

CLASS_DEFINITION::CLASS_DEFINITION(int Line, std::string& Filename): CLASS_DEFINITION(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

CLASS_DEFINITION::CLASS_DEFINITION(int Line, const char* Filename): BASE(Line, Filename, TYPE::Class_Definition){
  Attributes = 0;
  Parameters = 0;

  Parents = 0;

  Body = 0;
}
//------------------------------------------------------------------------------

CLASS_DEFINITION::~CLASS_DEFINITION(){
  if(Body      ) delete Body;
  if(Parents   ) delete Parents;
  if(Attributes) delete Attributes;
  if(Parameters) delete Parameters;
}
//------------------------------------------------------------------------------

BASE* CLASS_DEFINITION::Copy(){
  CLASS_DEFINITION* Copy = new CLASS_DEFINITION(Source.Line, Source.Filename.c_str());

  Copy->Identifier = Identifier;

  if(Parents) Copy->Parents = new PARENT(*Parents);

  Copy->Body       = (decltype(Body      ))CopyList(Body);
  Copy->Attributes = (decltype(Attributes))CopyList(Attributes);
  Copy->Parameters = (decltype(Parameters))CopyList(Parameters);

  return Copy;
}
//------------------------------------------------------------------------------

bool CLASS_DEFINITION::RunAST(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool CLASS_DEFINITION::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void CLASS_DEFINITION::Display(){
  DisplayInfo();
  Debug.Print("Class Definition (%s):\n", Identifier.c_str());

  if(Attributes){
    Debug.Print(" Attributes:\n"); Attributes->Display(); Debug.Print("\n");
  }
  if(Parameters){
    Debug.Print(" Parameters:\n"); Parameters->Display(); Debug.Print("\n");
  }

  if(Parents) Debug.Print(" Parents:\n");
  PARENT* Parent = Parents;
  while(Parent){
    Debug.Print(" - ");
    if(Parent->ClassName) Parent->ClassName->Display();
    Debug.Print("(");
    bool isFirst = true;
    foreach(Parameter, Parent->Parameters){
      if(isFirst) Debug.Print(", ");
      isFirst = false;
      (*Parameter)->Display();
    }
    Debug.Print(")\n");
    Parent = Parent->Next;
  }

  Debug.Print(" {\n");
  if(Body) Body->Display();
  Debug.Print(" }\n");

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void CLASS_DEFINITION::ValidateMembers(){
  assert(Type == TYPE::Class_Definition);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

