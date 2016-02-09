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

#include "Dictionary.h"
//------------------------------------------------------------------------------

DICTIONARY::NODE::NODE(const byte* Name, void* Data){
 Red  = true;
 Left = Right = 0;

 int j;
 for(j = 0; Name[j]; j++);
 this->Name = new byte[j+1];
 for(j = 0; Name[j]; j++) this->Name[j] = Name[j];
 this->Name[j] = 0;

 this->Data = Data;
}
//------------------------------------------------------------------------------

DICTIONARY::NODE::~NODE(){
 delete[] Name;

 if(Left ) delete Left;
 if(Right) delete Right;
}
//------------------------------------------------------------------------------

DICTIONARY::DICTIONARY(){
 Root = 0;
}
//------------------------------------------------------------------------------

DICTIONARY::~DICTIONARY(){
 if(Root) delete Root;
}
//------------------------------------------------------------------------------

bool DICTIONARY::IsRed(NODE* Node){
 if(!Node) return false;
 return Node->Red;
}
//------------------------------------------------------------------------------

void DICTIONARY::ColourFlip(NODE* Node){
 Node       ->Red = !Node       ->Red;
 Node->Left ->Red = !Node->Left ->Red;
 Node->Right->Red = !Node->Right->Red;
}
//------------------------------------------------------------------------------

DICTIONARY::NODE* DICTIONARY::RotateLeft(NODE* Node){
 NODE* Temp  = Node->Right;
 Node->Right = Temp->Left;
 Temp->Left  = Node;
 Temp->Red   = Node->Red;
 Node->Red   = true;
 return Temp;
}
//------------------------------------------------------------------------------

DICTIONARY::NODE* DICTIONARY::RotateRight(NODE* Node){
 NODE* Temp  = Node->Left;
 Node->Left  = Temp->Right;
 Temp->Right = Node;
 Temp->Red   = Node->Red;
 Node->Red   = true;
 return Temp;
}
//------------------------------------------------------------------------------

void DICTIONARY::Insert(const byte* Name, void* Data){
 Root = Insert(Root, Name, Data);
}
//------------------------------------------------------------------------------

DICTIONARY::NODE* DICTIONARY::Insert(NODE* Node, const byte* Name, void* Data){
 if(!Node) return new NODE(Name, Data);

 int j;
 for(j = 0; Name[j]; j++){
  if(Name[j] < Node->Name[j]){
   Node->Left = Insert(Node->Left, Name, Data);
   break;

  }else if(Name[j] > Node->Name[j]){
   Node->Right = Insert(Node->Right, Name, Data);
   break;
  }
 }
 if(Name[j] == Node->Name[j]) return Node; // Duplicates are ignored

 if(IsRed(Node->Right) && !IsRed(Node->Left      )) Node = RotateLeft (Node);
 if(IsRed(Node->Left ) &&  IsRed(Node->Left->Left)) Node = RotateRight(Node);
 if(IsRed(Node->Left ) &&  IsRed(Node->Right     ))        ColourFlip (Node);

 return Node;
}
//------------------------------------------------------------------------------

void* DICTIONARY::Find(const byte* Name){
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
   if(Name[j] == Node->Name[j]) return Node->Data;
  }
 }
 return 0;
}
//------------------------------------------------------------------------------

void DICTIONARY::Action(DICTIONARY_ACTION Function){
 if(Root) Action(Root, Function);
}
//------------------------------------------------------------------------------

void DICTIONARY::Action(NODE* Node, DICTIONARY_ACTION Function){
 if(Node->Left ) Action(Node->Left , Function);
 Function(Node->Name, Node->Data);
 if(Node->Right) Action(Node->Right, Function);
}
//------------------------------------------------------------------------------
