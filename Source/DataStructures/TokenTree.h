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
This is a binary search tree to optimise the scanner.  It facilitates the
greedy match operation.  The tree is only balanced once, so something like
a red-black tree is not required.

It consists of a balanced BST of the first character, each containing a
balanced sub-tree of the next character, etc.                                 */
//------------------------------------------------------------------------------

#ifndef TokenTree_h
#define TokenTree_h
//------------------------------------------------------------------------------

#include "Token.h"
//------------------------------------------------------------------------------

class TOKEN_TREE{
    private:
        struct NODE{
            byte        Char; // The character at the current depth
            TOKEN::TYPE Type; // Unknown => this is not a valid entry

            NODE* Left;
            NODE* Right;

            NODE* Next; // Sub-tree of the next character

            NODE(byte Char);
          ~NODE();
        };

        NODE* Root;

        NODE* Insert(NODE* Root, const byte* Pattern, TOKEN::TYPE Type);

        // Balancing functions
        NODE* Balance   (NODE* Root);
        NODE* Compress  (NODE* Root, int Count);
        void  SubBalance(NODE* Node);

    public:
        TOKEN_TREE();
      ~TOKEN_TREE();

        // Insert all the items, then balance it once.  Do not add more items after.
        void Insert (const char* Pattern, TOKEN::TYPE Type);
        void Balance();

        // Finds the longest match and returns the token type and character count
        TOKEN::TYPE Match(const byte* Pattern, int* Count);

        // Finds an exact match only
        TOKEN::TYPE Find(const byte* Pattern);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

