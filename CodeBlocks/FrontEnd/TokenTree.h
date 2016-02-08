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

/** This is a binary search tree to optimise the scanner.  It facilitates the
    greedy match operation.  The tree is only balanced once, so something like
    a red-black tree is not required. */
//------------------------------------------------------------------------------

#ifndef TokenTree_h
#define TokenTree_h
//------------------------------------------------------------------------------

#include "Token.h"
//------------------------------------------------------------------------------

class TOKEN_TREE{
 private:
  struct NODE{
   STRING      Pattern; // The characters
   TOKEN::TYPE Type;

   NODE* Left;
   NODE* Right;

   NODE(const byte* Pattern, TOKEN::TYPE Type);
  ~NODE();
  };

  NODE* Root;

  // Balancing functions
  void Compress(int Count);

  // Finds the node with the longest match to pattern
  NODE* Match(NODE* Node, const byte* Pattern, int* Count);

 public:
  TOKEN_TREE();
 ~TOKEN_TREE();

  // Add all the items, then balance it once.  Do not add more items after.
  void Add(const char* Pattern, TOKEN::TYPE Type);
  void Balance();

  // Finds the longest match and returns the token type and character count
  // Also checks for Identifier, Literal and String starting sequences
  TOKEN::TYPE Match(const byte* Pattern, int* Count);

  // Finds an exact match only
  TOKEN::TYPE Find(const byte* Pattern);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

