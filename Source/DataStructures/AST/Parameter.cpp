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
): BASE(Line, Filename, TYPE::Parameter){
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
  DisplayInfo();
  Debug.print("Parameter (");

  switch(DefinitionType){
    case DEFINITION_TYPE::Auto   : Debug.print("Auto):"     ); break;
    case DEFINITION_TYPE::Pin    : Debug.print("Pin):"      ); break;
    case DEFINITION_TYPE::Net    : Debug.print("Net):"      ); break;
    case DEFINITION_TYPE::Byte   : Debug.print("Byte):"     ); break;
    case DEFINITION_TYPE::Char   : Debug.print("Char):"     ); break;
    case DEFINITION_TYPE::Number : Debug.print("Number):"   ); break;
    case DEFINITION_TYPE::Func   : Debug.print("Function):" ); break;

    case DEFINITION_TYPE::ClassInstance:
      if(ClassName) ClassName->Display();
      else          Debug.print("Class instance with no class name");
      Debug.print("):");
      break;

    default: Debug.print("Invalid definition type:\n");
  }

  Debug.print(" %s", Identifier.c_str());

  int j;
  for(j = 0; j < ArrayDimensions; j++) Debug.print("[]");

  Debug.print("\n");
  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

