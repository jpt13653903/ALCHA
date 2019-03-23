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
): BASE(Line, Filename, TYPE::Definition){
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
  DisplayInfo();
  Debug.print("Definition (");

  switch(DefinitionType){
    case Pin    : Debug.print("Pin):\n"     ); break;
    case Net    : Debug.print("Net):\n"     ); break;
    case Void   : Debug.print("Void):\n"    ); break;
    case Auto   : Debug.print("Auto):\n"    ); break;
    case Byte   : Debug.print("Byte):"      ); break;
    case Char   : Debug.print("Character):" ); break;
    case Num    : Debug.print("Number):\n"  ); break;
    case Func   : Debug.print("Function):\n"); break;

    case ClassInstance:
      Debug.print("Class instance definition (");
      if(ClassName) ClassName->Display();
      else          Debug.print("Class instance with no class name");
      Debug.print("):\n");
      break;

    default: Debug.print("Invalid definition type:\n");
  }

  Debug.print(" Direction = ");
  switch(Direction){
    case Input : Debug.print("Input\n"   ); break;
    case Output: Debug.print("Output\n"  ); break;
    default    : Debug.print("Inferred\n"); break;
  }

  Debug.print(" Parameters: ");
  if(Parameters){
    Parameters->Display();
    Debug.print("\n");
  }else{
    Debug.print("none / default\n");
  }

  Debug.print(" Attributes: ");
  if(Attributes){
    Attributes->Display();
    Debug.print("\n");
  }

  Debug.print(" Identifiers:\n");
  IDENTIFIER* Identifier = Identifiers;
  ARRAY     * Array;
  while(Identifier){
    Debug.print(" - %s", Identifier->Identifier.c_str());
    Array = Identifier->Array;
    while(Array){
      Debug.print("[");
      if(Array->Size) Array->Size->Display();
      Debug.print("]");
      Array = Array->Next;
    }

    if(Identifier->Function){
      Debug.print(" -- function:\n  Parameters: (\n");
      if(Identifier->Parameters) Identifier->Parameters->Display();
      Debug.print(" )\n  Body:{\n");
      if(Identifier->FunctionBody) Identifier->FunctionBody->Display();
      Debug.print("  }\n");
    }
    if(Identifier->Initialiser){
      Debug.print(" -- initialiser:");
      Identifier->Initialiser->Display();
    }

    Debug.print("\n");
    Identifier = Identifier->Next;
  }

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------

