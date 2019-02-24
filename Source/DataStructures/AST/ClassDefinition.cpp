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

using namespace AST;
//------------------------------------------------------------------------------

CLASS_DEFINITION::PARENT::PARENT(){
  ClassName  = 0;
  Parameters = 0;
  Next       = 0;
}
//------------------------------------------------------------------------------

CLASS_DEFINITION::PARENT::~PARENT(){
  if(ClassName ) delete ClassName;
  if(Parameters) delete Parameters;
  if(Next      ) delete Next;
}
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

void CLASS_DEFINITION::Display(){
  DisplayInfo();
  printf("Class Definition (%s):\n", Identifier.c_str());

  if(Attributes){
    printf(" Attributes:\n"); Attributes->Display(); printf("\n");
  }
  if(Parameters){
    printf(" Parameters:\n"); Parameters->Display(); printf("\n");
  }

  if(Parents) printf(" Parents:\n");
  PARENT* Parent = Parents;
  while(Parent){
    printf(" - ");
    if(Parent->ClassName ) Parent->ClassName ->Display(); printf("(");
    if(Parent->Parameters) Parent->Parameters->Display(); printf(")\n");
    Parent = Parent->Next;
  }

  printf(" {\n");
  if(Body) Body->Display();
  printf(" }\n");

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------