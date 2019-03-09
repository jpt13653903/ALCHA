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
#include "Namespace.h"
#include "Expression.h"
//------------------------------------------------------------------------------

using namespace OBJECTS;
//------------------------------------------------------------------------------

BASE::BASE(const char* Name, TYPE Type){
  this->Name = Name;
  this->Type = Type;

  if(!Stack.empty()) this->Namespace = Stack.front();
  else               this->Namespace = 0;
}
//------------------------------------------------------------------------------

BASE::~BASE(){
  for(auto a = Attributes.begin(); a != Attributes.end(); a++){
    delete a->second;
  }
}
//------------------------------------------------------------------------------

void BASE::DisplayLongName(BASE* Node){
  if(!Node) return;
  if(Node->Namespace != &Global){
    DisplayLongName(Node->Namespace);
    printf("::");
  }
  printf("%s", Node->Name.c_str());
}
//------------------------------------------------------------------------------

void BASE::DisplayAttributes(int Indent){
  for(int n = 0; n < Indent; n++) printf(" ");
  printf("Attributes:\n");

  for(auto a = Attributes.begin(); a != Attributes.end(); a++){
    for(int n = 0; n < Indent; n++) printf(" ");
    printf("  %s = ", a->first.c_str());
    if(a->second) a->second->Display();
    else          printf("{null}");
    printf("\n");
  }
}
//------------------------------------------------------------------------------


