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
This is a dictionary to store arbitrary data with a string ID.  It is based
on a left-leaning red-black tree, as described by Robert Sedgewick,
Department of Computer Science, Princeton University, Princeton, NJ 08544     */
//------------------------------------------------------------------------------

#ifndef Dictionary_h
#define Dictionary_h
//------------------------------------------------------------------------------

#include "Global.h"
//------------------------------------------------------------------------------

typedef void (*DICTIONARY_ACTION)(const byte* Name, void* Data);
//------------------------------------------------------------------------------

class DICTIONARY{
 private:
  struct NODE{
   bool  Red;

   byte* Name; // This memory is internally managed
   void* Data; // This is arbitrary data and not deleted by this structure

   NODE* Left;
   NODE* Right;

   NODE(const byte* Name, void* Data);
  ~NODE();
  };

  NODE* Root;

  bool  IsRed      (NODE* Node);
  void  ColourFlip (NODE* Node);
  NODE* RotateLeft (NODE* Node);
  NODE* RotateRight(NODE* Node);

  NODE* Insert(NODE* Node, const byte* Name, void* Data);
  void  Action(NODE* Node, DICTIONARY_ACTION Function);

 public:
  DICTIONARY();
 ~DICTIONARY();

  void  Insert(const byte* Name, void* Data);
  void* Find  (const byte* Name);

  // This calls the given function for every node, in order
  void Action(DICTIONARY_ACTION Function);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

