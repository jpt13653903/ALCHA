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

#include "Symbol.h"
#include "SymbolTree.h"
//------------------------------------------------------------------------------

SYMBOL_TREE::NODE::NODE(SYMBOL* Symbol){
 Red  = true;
 Left = Right = 0;

 this->Symbol = Symbol;
}
//------------------------------------------------------------------------------

SYMBOL_TREE::NODE::~NODE(){
 if(Left ) delete Left;
 if(Right) delete Right;
}
//------------------------------------------------------------------------------

SYMBOL_TREE::SYMBOL_TREE(){
 Root = 0;
}
//------------------------------------------------------------------------------

SYMBOL_TREE::~SYMBOL_TREE(){
 if(Root) delete Root;
}
//------------------------------------------------------------------------------

bool SYMBOL_TREE::IsRed(NODE* Node){
 if(!Node) return false;
 return Node->Red;
}
//------------------------------------------------------------------------------

void SYMBOL_TREE::ColourFlip(NODE* Node){
 Node       ->Red = !Node       ->Red;
 Node->Left ->Red = !Node->Left ->Red;
 Node->Right->Red = !Node->Right->Red;
}
//------------------------------------------------------------------------------

SYMBOL_TREE::NODE* SYMBOL_TREE::RotateLeft(NODE* Node){
 NODE* Temp  = Node->Right;
 Node->Right = Temp->Left;
 Temp->Left  = Node;
 Temp->Red   = Node->Red;
 Node->Red   = true;
 return Temp;
}
//------------------------------------------------------------------------------

SYMBOL_TREE::NODE* SYMBOL_TREE::RotateRight(NODE* Node){
 NODE* Temp  = Node->Left;
 Node->Left  = Temp->Right;
 Temp->Right = Node;
 Temp->Red   = Node->Red;
 Node->Red   = true;
 return Temp;
}
//------------------------------------------------------------------------------

bool SYMBOL_TREE::Insert(SYMBOL* Symbol){
 Duplicate = false;
 Root = Insert(Root, Symbol);
 return !Duplicate;
}
//------------------------------------------------------------------------------

SYMBOL_TREE::NODE* SYMBOL_TREE::Insert(NODE* Node, SYMBOL* Symbol){
 if(!Node) return new NODE(Symbol);

 if(Symbol->Name < Node->Symbol->Name){
  Node->Left = Insert(Node->Left, Symbol);

 }else if(Symbol->Name > Node->Symbol->Name){
  Node->Right = Insert(Node->Right, Symbol);

 }else{ // Duplicate
  Duplicate = true;
  return Node;
 }

 if(IsRed(Node->Right) && !IsRed(Node->Left      )) Node = RotateLeft (Node);
 if(IsRed(Node->Left ) &&  IsRed(Node->Left->Left)) Node = RotateRight(Node);
 if(IsRed(Node->Left ) &&  IsRed(Node->Right     ))        ColourFlip (Node);

 return Node;
}
//------------------------------------------------------------------------------

SYMBOL* SYMBOL_TREE::Find(const byte* Name){
 NODE* Node = Root;

 while(Node){
       if(Name < Node->Symbol->Name) Node = Node->Left;
  else if(Name > Node->Symbol->Name) Node = Node->Right;
  else                               return Node->Symbol;
 }
 return 0;
}
//------------------------------------------------------------------------------
