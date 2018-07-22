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

#include "AST_Parameter.h"
//------------------------------------------------------------------------------

AST_Parameter::AST_Parameter(
 int             Line,
 const byte*     Filename,
 DEFINITION_TYPE DefinitionType
): AST_Base(Line, Filename){
 this->Type           = Parameter;
 this->DefinitionType = DefinitionType;

 ClassName       = 0;
 Identifier      = 0;
 ArrayDimensions = 0;
}
//------------------------------------------------------------------------------

AST_Parameter::~AST_Parameter(){
 if(ClassName) delete ClassName;
}
//------------------------------------------------------------------------------

void AST_Parameter::Display(){
 printf(" Line %d -- Parameter (", Line);

 switch(DefinitionType){
  case Auto   : printf("Auto):"     ); break;
  case Pin    : printf("Pin):"      ); break;
  case Net    : printf("Net):"      ); break;
  case Clk    : printf("Clock):"    ); break;
  case Byte   : printf("Byte):"     ); break;
  case Char   : printf("Character):"); break;
  case Int    : printf("Integer):"  ); break;
  case Rat    : printf("Rational):" ); break;
  case Float  : printf("Float):"    ); break;
  case Complex: printf("Complex):"  ); break;
  case Func   : printf("Function):" ); break;

  case ClassInstance:
   if(ClassName) ClassName->Display();
   else          printf("Class instance with no class name");
   printf("):");
   break;

  default: printf("Invalid definition type:\n");
 }

 printf(" %s", Identifier);

 int j;
 for(j = 0; j < ArrayDimensions; j++) printf("[]");

 printf("\n");
 if(Next) Next->Display();
}
//------------------------------------------------------------------------------
