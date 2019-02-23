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

#include "Parameter.h"
//------------------------------------------------------------------------------

using namespace AST;
//------------------------------------------------------------------------------

PARAMETER::PARAMETER(
  int             Line,
  const char*     Filename,
  DEFINITION_TYPE DefinitionType
): BASE(Line, Filename){
  this->Type           = TYPE::Parameter;
  this->DefinitionType = DefinitionType;

  ClassName       = 0;
  ArrayDimensions = 0;
}
//------------------------------------------------------------------------------

PARAMETER::~PARAMETER(){
  if(ClassName) delete ClassName;
}
//------------------------------------------------------------------------------

void PARAMETER::Display(){
  printf(" Line %d -- PARAMETER (", Line);

  switch(DefinitionType){
    case Auto   : printf("Auto):"     ); break;
    case Pin    : printf("Pin):"      ); break;
    case Net    : printf("Net):"      ); break;
    case Byte   : printf("Byte):"     ); break;
    case Char   : printf("Char):"     ); break;
    case Number : printf("Number):"   ); break;
    case Func   : printf("Function):" ); break;

    case ClassInstance:
      if(ClassName) ClassName->Display();
      else          printf("Class instance with no class name");
      printf("):");
      break;

    default: printf("Invalid definition type:\n");
  }

  printf(" %s", Identifier.c_str());

  int j;
  for(j = 0; j < ArrayDimensions; j++) printf("[]");

  printf("\n");
  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

