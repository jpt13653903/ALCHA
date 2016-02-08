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

TOKEN_TREE::NODE::NODE(const byte* Pattern, TOKEN::TYPE Type){
 Left = Right = 0;

 this->Pattern = (byte*)Pattern;
 this->Type    = Type;
}
//------------------------------------------------------------------------------

TOKEN_TREE::NODE::~NODE(){
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
 // Keep in vine structure until balancing restructures the tree

 NODE* Node = new NODE((byte*)Pattern, Type);
 NODE* Prev = 0;
 NODE* Temp = Root;

 int Compare;

 while(Temp){
  Compare = Temp->Pattern.Compare(Pattern);

  if(Compare > 0){ // New node < Temp
   if(Prev) Prev->Right = Node;
   else     Root        = Node;
   Node->Right = Temp;
   return;

  }else if(Compare < 0){ // New node > Temp
   Prev = Temp;
   Temp = Temp->Right;

  }else{ // Equal => Illegal
   printf("Duplicate token entry: %s\n", Pattern);
   delete Node;
   return;
  }
 }
 if(Prev) Prev->Right = Node;
 else     Root        = Node;
}
//------------------------------------------------------------------------------

/** This balancing operation is based on:
    Quentin F Stout and Bette L Warren,
    "Tree rebalancing in optimal space and time"
    Communications of the ACM, September 1986, Volume 29, Number 9
    http://www.eecs.umich.edu/~qstout/pap/CACM86.pdf */

void TOKEN_TREE::Balance(){
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

 if(Size) Compress(Size);
 Size = Count - Size;

 // Balance the tree
 while(Size > 1){
  Size /= 2;
  Compress(Size);
 }
}
//------------------------------------------------------------------------------

void TOKEN_TREE::Compress(int Count){
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
}
//------------------------------------------------------------------------------

TOKEN::TYPE TOKEN_TREE::Match(const byte* Pattern, int* Count){
 *Count = 0;

 if(!Pattern[0]) return TOKEN::Unknown;

 NODE* Node = Match(Root, Pattern, Count);

 if(Node) return Node->Type;
 return TOKEN::Unknown;
}
//------------------------------------------------------------------------------

TOKEN_TREE::NODE* TOKEN_TREE::Match(
 NODE      * Node,
 const byte* Pattern,
 int       * Count
){
 // Find the first node in the current sub-tree that has the
 // same first byteacter as Pattern
 while(Node){
  if     (Pattern[0] < Node->Pattern[0]) Node = Node->Left;
  else if(Pattern[0] > Node->Pattern[0]) Node = Node->Right;
  else                                   break;
 }
 if(!Node){
  *Count = 0;
  return 0;
 }

 // Check for a match on this node
 int j = 1; // The first byteacters are the same, by the search above
 while(Pattern[j] && Node->Pattern[j]){
  if(Pattern[j] != Node->Pattern[j]) break;
  j++;
 }

 // Check left and right
 int l, r;
 NODE* Left  = Match(Node->Left , Pattern, &l);
 NODE* Right = Match(Node->Right, Pattern, &r);

 if(Node->Pattern[j]){ // Not a match
  j    = 0;
  Node = 0;
 }
 if(l > j){ // Left has a better match
  j    = l;
  Node = Left;
 }
 if(r > j){ // Right has a better match
  j    = r;
  Node = Right;
 }
 *Count = j;
 return Node;
}
//------------------------------------------------------------------------------

TOKEN::TYPE TOKEN_TREE::Find(const byte* Pattern){
 int   Compare;
 NODE* Node = Root;

 while(Node){
  Compare = Node->Pattern.Compare(Pattern);
  if     (Compare > 0) Node = Node->Left;
  else if(Compare < 0) Node = Node->Right;
  else return Node->Type;
 }
 return TOKEN::Unknown;
}
//------------------------------------------------------------------------------
