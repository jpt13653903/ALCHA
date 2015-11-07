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

#include "Tree.h"
//------------------------------------------------------------------------------

TREE_NODE::TREE_NODE(){
 Red  = true;
 Left = Right = 0;
}
//------------------------------------------------------------------------------

TREE_NODE::~TREE_NODE(){
 if(Left ) delete Left;
 if(Right) delete Right;
}
//------------------------------------------------------------------------------

TREE::TREE(){
 Root = 0;
}
//------------------------------------------------------------------------------

TREE::~TREE(){
 delete Root;
}
//------------------------------------------------------------------------------

bool TREE::IsRed(TREE_NODE* N){
 if(N) return N->Red;
 return false;
}
//------------------------------------------------------------------------------

TREE_NODE* TREE::FixUp(TREE_NODE* N){
 if(IsRed(N->Right) && !IsRed(N->Left      )) N = RotateLeft (N);
 if(IsRed(N->Left ) &&  IsRed(N->Left->Left)) N = RotateRight(N);
 if(IsRed(N->Left ) &&  IsRed(N->Right     ))     FlipColours(N);

 return N;
}
//------------------------------------------------------------------------------

TREE_NODE* TREE::RotateLeft(TREE_NODE* N){
 TREE_NODE* t   = N->Right;
 N->Right  = t->Left;
 t->Left   = N;
 t->Red = N->Red;
 N->Red = true;

 return t;
}
//------------------------------------------------------------------------------

TREE_NODE* TREE::RotateRight(TREE_NODE* N){
 TREE_NODE* t   = N->Left;
 N->Left   = t->Right;
 t->Right  = N;
 t->Red = N->Red;
 N->Red = true;

 return t;
}
//------------------------------------------------------------------------------

void TREE::FlipColours(TREE_NODE* N){
 N       ->Red = !N       ->Red;
 N->Left ->Red = !N->Left ->Red;
 N->Right->Red = !N->Right->Red;
}
//------------------------------------------------------------------------------

TREE_NODE* TREE::Insert(TREE_NODE* N, TREE_NODE* Data){
 if(!N){
  return Data;
 }

 int result = Data->Compare(N);
 // Duplicate: don't insert anything
 if(!result) return N;

 if(result < 0){
  N->Left  = Insert(N->Left , Data);

 }else{
  N->Right = Insert(N->Right, Data);
 }

 return FixUp(N);
}
//------------------------------------------------------------------------------

void TREE::Insert(TREE_NODE* Data){
 Root = Insert(Root, Data);
 Root->Red = false;
}
//------------------------------------------------------------------------------

TREE_NODE* TREE::Find(TREE_NODE* N, TREE_NODE* Key){
 int   result;
 TREE_NODE* Temp = 0;

 while(N){
  result = Key->Compare(N);
  if     (result < 0) N = N->Left;
  else if(result > 0) N = N->Right;
  else{
   Temp = N;       // Store the current "best match" and
   N    = N->Left; // carry on searching the left sub-tree
  }
 }
 return Temp;
}
//------------------------------------------------------------------------------

TREE_NODE* TREE::Find(TREE_NODE* Key){
 return Find(Root, Key);
}
//------------------------------------------------------------------------------

void TREE::Clear(){
 if(Root) delete Root;
 Root = 0;
}
//------------------------------------------------------------------------------
