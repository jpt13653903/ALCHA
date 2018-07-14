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

#include "AST.h"
//------------------------------------------------------------------------------

AST_Base* AST = 0;
//------------------------------------------------------------------------------

void* AttributesOnDuplicate(const byte* Name, void* Old, void* New){
 delete (STRING*)Old;
 return New;
}
//------------------------------------------------------------------------------

void AttributesDisplay(const byte* Name, void* Data){
 STRING* Value = (STRING*)Data;
 printf(" - %s = \"%s\"\n", Name, Value->String());
}
//------------------------------------------------------------------------------

void AttributesDeleteData(const byte* Name, void* Data){
 delete (STRING*)Data;
}
//------------------------------------------------------------------------------

AST_Base::AST_Base(int Line, const byte* Filename){
 this->Line     = Line;
 this->Filename = IdentifierTree.GetID(Filename);
 this->Next     = 0;
}
//------------------------------------------------------------------------------

AST_Base::~AST_Base(){
 // The list might be too long for a recursive formulation.
 AST_Base* Temp;
 while(Next){
  Temp       = Next;
  Next       = Temp->Next;
  Temp->Next = 0;
  delete Temp;
 }
}
//------------------------------------------------------------------------------
