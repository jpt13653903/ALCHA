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

#include "ClassDefinition.h"
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

  if(Parent.ClassName) ClassName  = (decltype(Parent.ClassName ))Parent.ClassName ->Copy(true);
  if(Parent.Next     ) Next       = new PARENT(*Parent.Next);

  foreach(Parameter, Parent.Parameters){
    if(*Parameter) Parameters.push_back((*Parameter)->Copy(true));
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

CLASS_DEFINITION::CLASS_DEFINITION(int Line, const char* Filename): BASE(Line, Filename, TYPE::ClassDefinition){
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

BASE* CLASS_DEFINITION::Copy(bool CopyNext){
  CLASS_DEFINITION* Copy = new CLASS_DEFINITION(Source.Line, Source.Filename.c_str());

  Copy->Identifier = Identifier;

  if(Parents   ) Copy->Parents    = new PARENT(*Parents);
  if(Body      ) Copy->Body       = (decltype(Body      ))Body      ->Copy(CopyNext);
  if(Attributes) Copy->Attributes = (decltype(Attributes))Attributes->Copy(CopyNext);
  if(Parameters) Copy->Parameters = (decltype(Parameters))Parameters->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

BASE* CLASS_DEFINITION::RunScripting(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

bool CLASS_DEFINITION::GetVerilog(string& Body){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

void CLASS_DEFINITION::Display(){
  DisplayInfo();
  Debug.print("Class Definition (%s):\n", Identifier.c_str());

  if(Attributes){
    Debug.print(" Attributes:\n"); Attributes->Display(); Debug.print("\n");
  }
  if(Parameters){
    Debug.print(" Parameters:\n"); Parameters->Display(); Debug.print("\n");
  }

  if(Parents) Debug.print(" Parents:\n");
  PARENT* Parent = Parents;
  while(Parent){
    Debug.print(" - ");
    if(Parent->ClassName) Parent->ClassName->Display();
    Debug.print("(");
    bool isFirst = true;
    foreach(Parameter, Parent->Parameters){
      if(isFirst) Debug.print(", ");
      isFirst = false;
      (*Parameter)->Display();
    }
    Debug.print(")\n");
    Parent = Parent->Next;
  }

  Debug.print(" {\n");
  if(Body) Body->Display();
  Debug.print(" }\n");

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------
