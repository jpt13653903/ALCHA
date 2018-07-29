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
This is a dictionary to store the symbol table.  It is based on DICTIONARY, but
uses the identifier ID (obtained from IdentifierTree) instead of the identifier
string.                                                                       */
//------------------------------------------------------------------------------

#ifndef SymbolTree_h
#define SymbolTree_h
//------------------------------------------------------------------------------

#include "Reference.h"
//------------------------------------------------------------------------------

class SYMBOL_TREE{
  private:
    struct NODE{
      bool  Red;

      const byte* Name;   // Name ID taken from IdentifierTree
      REFERENCE*  Reference; // The object

      NODE* Left;
      NODE* Right;

      NODE(const byte* Name, REFERENCE* Reference);
     ~NODE();
    };

    NODE* Root;

    bool  IsRed      (NODE* Node);
    void  ColourFlip (NODE* Node);
    NODE* RotateLeft (NODE* Node);
    NODE* RotateRight(NODE* Node);

    bool  Duplicate;
    NODE* Insert(NODE* Node, const byte* Name, REFERENCE* Reference);

  public:
    SYMBOL_TREE();
   ~SYMBOL_TREE();

    // Name must be an ID obtained from IdentifierTree
    // Insert returns false on duplicate name
    bool       Insert(const byte* Name, REFERENCE* Reference);
    REFERENCE* Find  (const byte* Name);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

