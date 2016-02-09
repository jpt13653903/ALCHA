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

#include "TokenTree.h"
//------------------------------------------------------------------------------

TOKEN_TREE::NODE::NODE(byte Char){
 Left = Right = Next = 0;

 this->Char = Char;
 this->Type = TOKEN::Unknown;
}
//------------------------------------------------------------------------------

TOKEN_TREE::NODE::~NODE(){
 if(Next ) delete Next;
 if(Left ) delete Left;
 if(Right) delete Right;
}
//------------------------------------------------------------------------------

TOKEN_TREE::TOKEN_TREE(){
 Root = 0;
}
//------------------------------------------------------------------------------

TOKEN_TREE::~TOKEN_TREE(){
 if(Root) delete Root;
}
//------------------------------------------------------------------------------

void TOKEN_TREE::Add(const char* Pattern, TOKEN::TYPE Type){
 Root = Add(Root, (byte*)Pattern, Type);
}
//------------------------------------------------------------------------------

TOKEN_TREE::NODE* TOKEN_TREE::Add(
 NODE*       Root,
 const byte* Pattern,
 TOKEN::TYPE Type
){
 if(!Pattern[0]) return Root;

 // Keep in vine structure until balancing restructures the tree

 NODE* Node;
 NODE* Prev = 0;
 NODE* Temp = Root;

 while(Temp){
  if(*Pattern < Temp->Char){
   Node        = new NODE(*Pattern);
   Node->Right = Temp;
   if(Pattern[1]) Node->Next = Add(Node->Next, Pattern+1, Type);
   else           Node->Type = Type;
   if(Prev) Prev->Right = Node;
   else     Root        = Node;
   return Root;

  }else if(*Pattern > Temp->Char){
   Prev = Temp;
   Temp = Temp->Right;

  }else{
   if(Pattern[1]){
    Temp->Next = Add(Temp->Next, Pattern+1, Type);
   }else{
    if(Temp->Type) printf("Duplicate token entry: ...%s = %d\n", Pattern, Type);
    else           Temp->Type = Type;
   }
   return Root;
  }
 }
 Node = new NODE(*Pattern);
 if(Pattern[1]) Node->Next = Add(Node->Next, Pattern+1, Type);
 else           Node->Type = Type;
 if(Prev) Prev->Right = Node;
 else     Root        = Node;

 return Root;
}
//------------------------------------------------------------------------------

/** This balancing operation is based on:
    Quentin F Stout and Bette L Warren,
    "Tree rebalancing in optimal space and time"
    Communications of the ACM, September 1986, Volume 29, Number 9
    http://www.eecs.umich.edu/~qstout/pap/CACM86.pdf */

void TOKEN_TREE::Balance(){
 Root = Balance(Root);
}
//------------------------------------------------------------------------------

TOKEN_TREE::NODE* TOKEN_TREE::Balance(NODE* Root){
 if(!Root) return 0;

 // Count the items in the vine
 int   Count = 0;
 NODE* Node  = Root;
 while(Node){
  Count++;
  Node = Node->Right;
 }

 // Create the deepest leaves
 int j    = 0x8000;
 int Size = Count + 1;
 while(j > Size) j >>= 1; // j = 2^floor(log2(Count + 1))
 Size -= j;

 if(Size) Root = Compress(Root, Size);
 Size = Count - Size;

 // Balance the tree
 while(Size > 1){
  Size /= 2;
  Root  = Compress(Root, Size);
 }

 SubBalance(Root);

 return Root;
}
//------------------------------------------------------------------------------

TOKEN_TREE::NODE* TOKEN_TREE::Compress(NODE* Root, int Count){
 NODE* Temp  = Root->Right;
 Root->Right = Temp->Left;
 Temp->Left  = Root;
 Root        = Temp;

 int   j;
 NODE* Node = Root;

 for(j = 1; j < Count; j++){
  Temp               = Node->Right->Right;
  Node->Right->Right = Temp->Left;
  Temp->Left         = Node->Right;
  Node->Right        = Temp;
  Node               = Temp;
 }
 return Root;
}
//------------------------------------------------------------------------------

void TOKEN_TREE::SubBalance(NODE* Node){
 if(Node->Next) Node->Next = Balance(Node->Next);

 if(Node->Left ) SubBalance(Node->Left );
 if(Node->Right) SubBalance(Node->Right);
}
//------------------------------------------------------------------------------

TOKEN::TYPE TOKEN_TREE::Match(const byte* Pattern, int* Count){
 int         N    = 0;
 TOKEN::TYPE Type = TOKEN::Unknown;

 *Count = 0;

 NODE* Node = Root;

 while(Node){
  if(*Pattern < Node->Char){
   Node = Node->Left;

  }else if(*Pattern > Node->Char){
   Node = Node->Right;

  }else{
   N++;
   if(Node->Type){ // Keep track of the best option
    *Count = N;
    Type   = Node->Type;
   }
   if(Pattern[1]){
    Pattern++;
    Node = Node->Next;
   }else{
    return Type;
   }
  }
 }
 return Type;
}
//------------------------------------------------------------------------------

TOKEN::TYPE TOKEN_TREE::Find(const byte* Pattern){
 NODE* Node = Root;

 while(Node){
  if(*Pattern < Node->Char){
   Node = Node->Left;

  }else if(*Pattern > Node->Char){
   Node = Node->Right;

  }else{
   if(Pattern[1]){
    Pattern++;
    Node = Node->Next;
   }else{
    return Node->Type;
   }
  }
 }
 return TOKEN::Unknown;
}
//------------------------------------------------------------------------------
