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

#include "AST_Group.h"
//------------------------------------------------------------------------------

AST_Group::AST_Group(int Line): AST_Base(Line){
 Type = Group;

 Attributes.OnDuplicate = AttributesOnDuplicate;

 Identifier = 0;
 Body       = 0;
}
//------------------------------------------------------------------------------

AST_Group::~AST_Group(){
 Attributes.Action(AtributesDeleteData);

 if(Body) delete Body;
}
//------------------------------------------------------------------------------

void AST_Group::Display(){
 if(Identifier) printf("\nLine %d -- Group (%s):\n", Line, Identifier);
 else           printf("\nLine %d -- Group:\n"     , Line);

 printf(" Attributes:\n");
  Attributes.Action(AttributesDisplay);

 printf(" {\n");
 if(Body) Body->Display();
 printf(" }\n");

 if(Next) Next->Display();
}
//------------------------------------------------------------------------------
