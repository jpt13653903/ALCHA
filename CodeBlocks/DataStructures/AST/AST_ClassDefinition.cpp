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

#include "AST_ClassDefinition.h"
//------------------------------------------------------------------------------

AST_ClassDefinition::AST_ClassDefinition(int Line): AST_Base(Line){
 Type = ClassDefinition;

 Attributes.OnDuplicate = AttributesOnDuplicate;

 Identifier = 0;
 Parameters = 0;

 Parent           = 0;
 ParentParameters = 0;

 Body = 0;
}
//------------------------------------------------------------------------------

AST_ClassDefinition::~AST_ClassDefinition(){
 Attributes.Action(AtributesDeleteData);

 if(Body            ) delete Body;
 if(Parameters      ) delete Parameters;
 if(ParentParameters) delete ParentParameters;
}
//------------------------------------------------------------------------------

void AST_ClassDefinition::Display(){
 printf("\nLine %d -- Class Definition (%s):\n", Line, Identifier);
 printf(" Attributes:\n");
  Attributes.Action(AttributesDisplay);

 printf(" Parameters:\n  "); if(Parameters) Parameters->Display(); printf("\n");

 if(Parent){
  printf(" Parent (%s):\n  ", Parent);
  if(ParentParameters) ParentParameters->Display(); printf("\n");
 }
 printf(" {\n");
 if(Body) Body->Display();
 printf(" }\n");

 if(Next) Next->Display();
}
//------------------------------------------------------------------------------
