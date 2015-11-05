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
- Removes trailing spaces
- Escape new-lines
- Populates line numbers
*/
//------------------------------------------------------------------------------

#ifndef Scanner_h
#define Scanner_h
//------------------------------------------------------------------------------

#include "FileSystem.h"
//------------------------------------------------------------------------------

class SCANNER{
 public:
  enum TYPE{tByte, tSpace, tNewline};

  struct CHAR{
   unsigned char Char[4]; // UTF-8 string
   int           Line;
   TYPE          Type;
  };

 private:
  FILE_SYSTEM fs;

  int      Line;
  unsigned Index;
  STRING   Buffer;

  // These don't modify Index
  int Space  (); // Single space, returns code length of the space
  int Spaces (); // Scans for multiple spaces, returns the number of bytes
  int Newline(); // Single newline, returns code length of the newline

 public:
  SCANNER();
 ~SCANNER();

  bool Open(const char* Filename);

  bool Get(CHAR& Char);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
