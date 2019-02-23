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

#include "EnumDefinition.h"
//------------------------------------------------------------------------------

using namespace AST;
//------------------------------------------------------------------------------

ENUM_DEFINITION::VALUE::VALUE(){
  Next = 0;
}
//------------------------------------------------------------------------------

ENUM_DEFINITION::VALUE::~VALUE(){
  if(Next) delete Next;
}
//------------------------------------------------------------------------------

ENUM_DEFINITION::ENUM_DEFINITION(int Line, const char* Filename):
BASE(Line, Filename, TYPE::EnumDefinition){
  Values = 0;
}
//------------------------------------------------------------------------------

ENUM_DEFINITION::~ENUM_DEFINITION(){
  if(Values) delete Values;
}
//------------------------------------------------------------------------------

void ENUM_DEFINITION::Display(){
  printf("\n%s:%d -- Enum Definition (%s):\n",
         Filename.c_str(),
         Line,
         Identifier.c_str());

  printf(" Values: ");
  VALUE* Value = Values;
  while(Value){
    printf("%s", Value->Identifier.c_str());
    Value = Value->Next;
    if(Value) printf(", ");
    else      printf("\n");
  }

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------
