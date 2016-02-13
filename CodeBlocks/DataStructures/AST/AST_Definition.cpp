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
 Identifier = 0;
 Array      = 0;
 Next       = 0;

 Function     = false;
 Parameters   = 0;
 FunctionBody = 0;
}
//------------------------------------------------------------------------------

AST_Definition::IDENTIFIER::~IDENTIFIER(){
 if(Array) delete Array;
 if(Next ) delete Next;

 if(Parameters  ) delete Parameters;
 if(FunctionBody) delete FunctionBody;
}
//------------------------------------------------------------------------------

AST_Definition::AST_Definition(int Line, DEFINITION_TYPE DefinitionType):
AST_Base(Line){
 this->Type           = Definition;
 this->DefinitionType = DefinitionType;

 Signed    = false;
 Direction = Bidirectional;

 ClassName = 0;
 Format    = 0;

 Attributes.OnDuplicate = AttributesOnDuplicate;
}
//------------------------------------------------------------------------------

AST_Definition::~AST_Definition(){
 if(ClassName) delete ClassName;
 if(Format   ) delete Format;

 Attributes.Action(AtributesDeleteData);
}
//------------------------------------------------------------------------------

void AST_Definition::Display(){
 printf("\nLine %d -- Definition (", Line);

 switch(DefinitionType){
  case Void   : printf("Void):\n"    ); break;
  case Pin    : printf("Pin):\n"     ); break;
  case Sig    : printf("Signal):\n"  ); break;
  case Clk    : printf("Clock):\n"   ); break;
  case Int    : printf("Integer):\n" ); break;
  case Rat    : printf("Rational):\n"); break;
  case Float  : printf("Float):\n"   ); break;
  case Complex: printf("Complex):\n" ); break;

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
  case In : printf("Input\n"); break;
  case Out: printf("Output\n"); break;
  default : printf("Bidirectional\n"); break;
 }

 printf(" Format:\n");
 if(Signed) printf("  Signed\n");
 else       printf("  Unsigned\n");
 printf("  Format = ");
 if(Format){
  Format->Display();
  printf("\n");
 }else{
  printf("1-bit\n");
 }

 printf(" Attributes:\n");
 Attributes.Action(AttributesDisplay);

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
   printf(" -- function:\n  Parameters: (");
   if(Identifier->Parameters) Identifier->Parameters->Display();
   printf(")\n  Body:{\n");
   if(Identifier->FunctionBody) Identifier->FunctionBody->Display();
   printf("  }\n");
  }

  printf("\n");
  Identifier = Identifier->Next;
 }

 if(Next) Next->Display();
}
//------------------------------------------------------------------------------

