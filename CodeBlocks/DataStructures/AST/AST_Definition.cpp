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
 Array = 0;
 Next  = 0;
}
//------------------------------------------------------------------------------

AST_Definition::IDENTIFIER::~IDENTIFIER(){
 if(Array) delete Array;
 if(Next ) delete Next;
}
//------------------------------------------------------------------------------

AST_Definition::AST_Definition(int Line, DEFINITION_TYPE DefinitionType):
AST_Base(Line){
 this->Type           = Definition;
 this->DefinitionType = DefinitionType;

 Signed    = false;
 Direction = Bidirectional;

 IntegerBits  = 0;
 FractionBits = 0;

 Attributes.OnDuplicate = AttributesOnDuplicate;
}
//------------------------------------------------------------------------------

AST_Definition::~AST_Definition(){
 if(IntegerBits ) delete IntegerBits;
 if(FractionBits) delete FractionBits;

 Attributes.Action(AtributesDeleteData);
}
//------------------------------------------------------------------------------

void AST_Definition::Display(){
 printf("\nLine %d -- ", Line);

 switch(DefinitionType){
  case Pin    : printf("Pin definition:\n"); break;
  case Sig    : printf("Signal definition:\n"); break;
  case Clk    : printf("Clock definition:\n"); break;
  case Int    : printf("Integer definition:\n"); break;
  case Rat    : printf("Rational definition:\n"); break;
  case Float  : printf("Float definition:\n"); break;
  case Complex: printf("Complex definition:\n"); break;
  default     : printf("Invalid definition type:\n"); break;
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
 printf("  Integer bits  = ");
 if(IntegerBits){
  IntegerBits->Display();
  printf("\n");
 }else{
  printf("1\n");
 }
 printf("  Fraction bits = ");
 if(FractionBits){
  FractionBits->Display();
  printf("\n");
 }else{
  printf("0\n");
 }

 printf(" Attributes:\n");
 Attributes.Action(AttributesDisplay);

 printf(" Identifier:\n");
 IDENTIFIER* Identifier = Identifiers;
 ARRAY     * Array;
 while(Identifier){
  printf(" - %s", Identifier->Identifier.String());
  Array = Identifier->Array;
  while(Array){
   printf("[");
   if(Array->Size) Array->Size->Display();
   printf("]");
   Array = Array->Next;
  }
  printf("\n");
  Identifier = Identifier->Next;
 }

 if(Next) Next->Display();
}
//------------------------------------------------------------------------------

