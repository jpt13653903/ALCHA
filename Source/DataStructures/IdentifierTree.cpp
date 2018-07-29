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

#include "IdentifierTree.h"
//------------------------------------------------------------------------------

IDENTIFIER_TREE IdentifierTree;
//------------------------------------------------------------------------------

IDENTIFIER_TREE::NODE::NODE(const byte* Name){
  Red  = true;
  Left = Right = 0;

  int j;
  for(j = 0; Name[j]; j++);
  this->Name = new byte[j+1];
  for(j = 0; Name[j]; j++) this->Name[j] = Name[j];
  this->Name[j] = 0;
}
//------------------------------------------------------------------------------

IDENTIFIER_TREE::NODE::~NODE(){
  delete[] Name;

  if(Left ) delete Left;
  if(Right) delete Right;
}
//------------------------------------------------------------------------------

IDENTIFIER_TREE::IDENTIFIER_TREE(){
  Root = 0;
}
//------------------------------------------------------------------------------

IDENTIFIER_TREE::~IDENTIFIER_TREE(){
  if(Root) delete Root;
}
//------------------------------------------------------------------------------

bool IDENTIFIER_TREE::IsRed(NODE* Node){
  if(!Node) return false;
  return Node->Red;
}
//------------------------------------------------------------------------------

void IDENTIFIER_TREE::ColourFlip(NODE* Node){
  Node       ->Red = !Node       ->Red;
  Node->Left ->Red = !Node->Left ->Red;
  Node->Right->Red = !Node->Right->Red;
}
//------------------------------------------------------------------------------

IDENTIFIER_TREE::NODE* IDENTIFIER_TREE::RotateLeft(NODE* Node){
  NODE* Temp  = Node->Right;
  Node->Right = Temp->Left;
  Temp->Left  = Node;
  Temp->Red   = Node->Red;
  Node->Red   = true;
  return Temp;
}
//------------------------------------------------------------------------------

IDENTIFIER_TREE::NODE* IDENTIFIER_TREE::RotateRight(NODE* Node){
  NODE* Temp  = Node->Left;
  Node->Left  = Temp->Right;
  Temp->Right = Node;
  Temp->Red   = Node->Red;
  Node->Red   = true;
  return Temp;
}
//------------------------------------------------------------------------------

byte* IDENTIFIER_TREE::GetID(const byte* Name){
  byte* ID = Find(Name);
  if(!ID){
    Root = Insert(Root, Name);
    ID   = Find  (Name);
  }
  return ID;
}
//------------------------------------------------------------------------------

byte* IDENTIFIER_TREE::Find(const byte* Name){
  int   j;
  NODE* Node = Root;

  while(Node){
    if(Name[0] < Node->Name[0]){
      Node = Node->Left;

    }else if(Name[0] > Node->Name[0]){
      Node = Node->Right;

    }else{
      for(j = 1; Name[j]; j++){
        if(Name[j] < Node->Name[j]){
          Node = Node->Left;
          break;

        }else if(Name[j] > Node->Name[j]){
          Node = Node->Right;
          break;
        }
      }
      if(!Name[j]){
        if(!Node->Name[j]) return Node->Name;
        else               Node = Node->Left;
      }
    }
  }
  return 0;
}
//------------------------------------------------------------------------------

IDENTIFIER_TREE::NODE* IDENTIFIER_TREE::Insert(NODE* Node, const byte* Name){
  if(!Node) return new NODE(Name);

  int j;
  for(j = 0; Name[j]; j++){
    if(Name[j] < Node->Name[j]){
      Node->Left = Insert(Node->Left, Name);
      break;

    }else if(Name[j] > Node->Name[j]){
      Node->Right = Insert(Node->Right, Name);
      break;
    }
  }
  if(!Name[j]){
    if(!Node->Name[j]) return Node; // Duplicate
    else               Node->Left = Insert(Node->Left, Name);
  }

  if(IsRed(Node->Right) && !IsRed(Node->Left      )) Node = RotateLeft (Node);
  if(IsRed(Node->Left ) &&  IsRed(Node->Left->Left)) Node = RotateRight(Node);
  if(IsRed(Node->Left ) &&  IsRed(Node->Right     ))        ColourFlip (Node);

  return Node;
}
//------------------------------------------------------------------------------
