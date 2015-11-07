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

/**
 An implementation of Left-Leaning Red-Black Tree, as described by
 Robert Sedgewick, Department of Computer Science, Princeton University,
 Princeton, NJ 08544
*/
//------------------------------------------------------------------------------

#ifndef Tree_h
#define Tree_h
//------------------------------------------------------------------------------

struct TREE_NODE{
 bool       Red;
 TREE_NODE* Left;
 TREE_NODE* Right;

          TREE_NODE();
 virtual ~TREE_NODE();

 // Returns <0 for Left < Right; 0 for Equal; >0 for Left > Right
 virtual int Compare(TREE_NODE* Right) = 0;
};
//------------------------------------------------------------------------------

class TREE{
 private:
  TREE_NODE* Root;

  inline bool       IsRed(TREE_NODE* N);
  inline TREE_NODE* FixUp(TREE_NODE* N);

  // Insert functions
  TREE_NODE* Insert     (TREE_NODE* N, TREE_NODE* Data);
  TREE_NODE* RotateLeft (TREE_NODE* N);
  TREE_NODE* RotateRight(TREE_NODE* N);
  void       FlipColours(TREE_NODE* N);

  // Find function
  TREE_NODE* Find(TREE_NODE* N, TREE_NODE* Key);

 public:
           TREE();
  virtual ~TREE();

  void       Insert(TREE_NODE* Data);
  TREE_NODE* Find  (TREE_NODE* Key);
  void       Clear ();
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
