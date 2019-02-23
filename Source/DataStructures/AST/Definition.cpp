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

#include "Definition.h"
//------------------------------------------------------------------------------

using namespace AST;
//------------------------------------------------------------------------------

DEFINITION::ARRAY::ARRAY(){
  Size = 0;
  Next = 0;
}
//------------------------------------------------------------------------------

DEFINITION::ARRAY::~ARRAY(){
  if(Size) delete Size;
  if(Next) delete Next;
}
//------------------------------------------------------------------------------

DEFINITION::IDENTIFIER::IDENTIFIER(){
  Next        = 0;
  Array       = 0;
  Initialiser = 0;

  Function     = false;
  Parameters   = 0;
  FunctionBody = 0;
}
//------------------------------------------------------------------------------

DEFINITION::IDENTIFIER::~IDENTIFIER(){
  if(Next        ) delete Next;
  if(Array       ) delete Array;
  if(Parameters  ) delete Parameters;
  if(Initialiser ) delete Initialiser;
  if(FunctionBody) delete FunctionBody;
}
//------------------------------------------------------------------------------

DEFINITION::DEFINITION(
  int             Line,
  const char*     Filename,
  DEFINITION_TYPE DefinitionType
): BASE(Line, Filename){
  this->Type           = TYPE::Definition;
  this->DefinitionType = DefinitionType;

  Direction = Inferred;

  ClassName  = 0;
  Parameters = 0;
  Attributes = 0;
}
//------------------------------------------------------------------------------

DEFINITION::~DEFINITION(){
  if(ClassName ) delete ClassName;
  if(Parameters) delete Parameters;
  if(Attributes) delete Attributes;
}
//------------------------------------------------------------------------------

void DEFINITION::Display(){
  printf("\n%s:%d -- DEFINITION (", Filename.c_str(), Line);

  switch(DefinitionType){
    case Pin    : printf("Pin):\n"     ); break;
    case Net    : printf("Net):\n"     ); break;
    case Void   : printf("Void):\n"    ); break;
    case Auto   : printf("Auto):\n"    ); break;
    case Byte   : printf("Byte):"      ); break;
    case Char   : printf("Character):" ); break;
    case Num    : printf("Number):\n"  ); break;
    case Func   : printf("Function):\n"); break;

    case ClassInstance:
      printf("Class instance definition (");
      if(ClassName) ClassName->Display();
      else          printf("Class instance with no class name");
      printf("):\n");
      break;

    default: printf("Invalid definition type:\n");
  }

  printf(" Direction = ");
  switch(Direction){
    case Input : printf("Input\n"); break;
    case Output: printf("Output\n"); break;
    default    : printf("Inferred\n"); break;
  }

  printf(" Parameters: ");
  if(Parameters){
    Parameters->Display();
    printf("\n");
  }else{
    printf("none / default\n");
  }

  printf(" Attributes: ");
  if(Attributes){
    Attributes->Display();
    printf("\n");
  }

  printf(" Identifiers:\n");
  IDENTIFIER* Identifier = Identifiers;
  ARRAY     * Array;
  while(Identifier){
    printf(" - %s", Identifier->Identifier.c_str());
    Array = Identifier->Array;
    while(Array){
      printf("[");
      if(Array->Size) Array->Size->Display();
      printf("]");
      Array = Array->Next;
    }

    if(Identifier->Function){
      printf(" -- function:\n  Parameters: (\n");
      if(Identifier->Parameters) Identifier->Parameters->Display();
      printf(" )\n  Body:{\n");
      if(Identifier->FunctionBody) Identifier->FunctionBody->Display();
      printf("  }\n");
    }
    if(Identifier->Initialiser){
      printf(" -- initialiser:");
      Identifier->Initialiser->Display();
    }

    printf("\n");
    Identifier = Identifier->Next;
  }

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

