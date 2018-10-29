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

#include "AST_Definition.h"
//------------------------------------------------------------------------------

AST_Definition::ARRAY::ARRAY(){
  Size = 0;
  Next = 0;
}
//------------------------------------------------------------------------------

AST_Definition::ARRAY::~ARRAY(){
  if(Size) delete Size;
  if(Next) delete Next;
}
//------------------------------------------------------------------------------

AST_Definition::IDENTIFIER::IDENTIFIER(){
  Next        = 0;
  Array       = 0;
  Identifier  = 0;
  Initialiser = 0;

  Function     = false;
  Parameters   = 0;
  FunctionBody = 0;
}
//------------------------------------------------------------------------------

AST_Definition::IDENTIFIER::~IDENTIFIER(){
  if(Next        ) delete Next;
  if(Array       ) delete Array;
  if(Parameters  ) delete Parameters;
  if(Initialiser ) delete Initialiser;
  if(FunctionBody) delete FunctionBody;
}
//------------------------------------------------------------------------------

AST_Definition::AST_Definition(
  int             Line,
  const byte*     Filename,
  DEFINITION_TYPE DefinitionType
): AST_Base(Line, Filename){
  this->Type           = Definition;
  this->DefinitionType = DefinitionType;

  Direction = Inferred;

  ClassName = 0;
  Format    = 0;

  FormatType = AST_Expression::Cast;

  Attributes.OnDuplicate = AttributesOnDuplicate;
}
//------------------------------------------------------------------------------

AST_Definition::~AST_Definition(){
  if(ClassName) delete ClassName;
  if(Format   ) delete Format;

  Attributes.Action(AttributesDeleteData);
}
//------------------------------------------------------------------------------

void AST_Definition::Display(){
  printf("\n%s:%d -- Definition (", Filename, Line);

  switch(DefinitionType){
    case Void   : printf("Void):\n"    ); break;
    case Auto   : printf("Auto):\n"    ); break;
    case Pin    : printf("Pin):\n"     ); break;
    case Net    : printf("Net):\n"     ); break;
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

  printf(" Format: ");
  if(Format){
    Format->Display();
    printf("\n");
  }else{
    printf("1-bit\n");
  }

  if(Attributes.GetCount()){
    printf(" Attributes:\n");
    Attributes.Action(AttributesDisplay);
  }

  printf(" Identifiers:\n");
  IDENTIFIER* Identifier = Identifiers;
  ARRAY     * Array;
  while(Identifier){
    printf(" - %s", Identifier->Identifier);
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

