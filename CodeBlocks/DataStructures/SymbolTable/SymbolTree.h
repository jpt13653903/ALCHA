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

#include "Global.h"
//------------------------------------------------------------------------------

class SYMBOL;
//------------------------------------------------------------------------------

class SYMBOL_TREE{
 private:
  struct NODE{
   bool  Red;

   SYMBOL* Symbol; // The symbol

   NODE* Left;
   NODE* Right;

   NODE(SYMBOL* Symbol);
  ~NODE();
  };

  NODE* Root;

  bool  IsRed      (NODE* Node);
  void  ColourFlip (NODE* Node);
  NODE* RotateLeft (NODE* Node);
  NODE* RotateRight(NODE* Node);

  bool  Duplicate;
  NODE* Insert(NODE* Node, SYMBOL* Symbol);

 public:
  SYMBOL_TREE();
 ~SYMBOL_TREE();

  // Name must be an ID obtained from IdentifierTree
  bool    Insert(SYMBOL*     Symbol); // Returns false on duplicate name
  SYMBOL* Find  (const byte* Name  );
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

