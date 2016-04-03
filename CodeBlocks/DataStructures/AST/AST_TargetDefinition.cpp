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

#include "AST_TargetDefinition.h"
//------------------------------------------------------------------------------

AST_TargetDefinition::AST_TargetDefinition(int Line, const byte* Filename):
AST_Base(Line, Filename){
 Type = TargetDefinition;

 Attributes.OnDuplicate = AttributesOnDuplicate;
}
//------------------------------------------------------------------------------

AST_TargetDefinition::~AST_TargetDefinition(){
 Attributes.Action(AtributesDeleteData);
}
//------------------------------------------------------------------------------

void AST_TargetDefinition::Display(){
 printf("\n%s:%d -- Target Definition:\n Attributes:\n", Filename, Line);
 Attributes.Action(AttributesDisplay);

 if(Next) Next->Display();
}
//------------------------------------------------------------------------------
