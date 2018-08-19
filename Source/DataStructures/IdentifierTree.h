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
This is a global tree that reduces memory usage on storing identifiers.  The
same memory is reused, rather than creating new instances.

It is based on a left-leaning red-black tree, as described by Robert Sedgewick,
Department of Computer Science, Princeton University, Princeton, NJ 08544     */
//------------------------------------------------------------------------------

#ifndef IdentifierTree_h
#define IdentifierTree_h
//------------------------------------------------------------------------------

#include "General.h"
//------------------------------------------------------------------------------

class IDENTIFIER_TREE{
  private:
    struct NODE{
      bool  Red;

      byte* Name;

      NODE* Left;
      NODE* Right;

      NODE(const byte* Name);
     ~NODE();
    };

    NODE* Root;

    bool  IsRed      (NODE* Node);
    void  ColourFlip (NODE* Node);
    NODE* RotateLeft (NODE* Node);
    NODE* RotateRight(NODE* Node);

    byte* Find  (            const byte* Name);
    NODE* Insert(NODE* Node, const byte* Name);

  public:
    IDENTIFIER_TREE();
   ~IDENTIFIER_TREE();

    byte* GetID(const byte* Name);
};
//------------------------------------------------------------------------------

extern IDENTIFIER_TREE IdentifierTree;
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

