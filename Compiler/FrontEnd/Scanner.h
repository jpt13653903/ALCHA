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

/** Scans in two phases:
1) GetChar()
   - Removes trailing spaces
   - Escape new-lines
   - Populates line numbers
2) GetToken()
   - Removes leading spaces outside strings and comments
   - Concatenates spaces outside strings and comments
   - Concatenates newlines outside strings and comments
   - Converts character stream to preprocessing token
*/
//------------------------------------------------------------------------------

#ifndef Scanner_h
#define Scanner_h
//------------------------------------------------------------------------------

 # include "FileSystem.h"
//------------------------------------------------------------------------------

class SCANNER{
 public: // Public types
  enum TYPE{
   tSpace,
   tNewline,
   tComment,
   tDirective, // Newline {Space} "#" {Space} Identifier; store only ident.
   tIdentifier,
   tNumber,
   tCharacter,
   tString,
   tPunctuator,
   tOther,
   tEOF,
  };

  struct TOKEN{ // Preprocessing token
   TYPE   Type;
   STRING Token;
   int    Line;
   TOKEN* Next;

   TOKEN();
  };

 private: // General private members
  bool   error;
  STRING Filename;

  void  Error(const char* Message);

 private: // Phase 1 members
  struct CHAR{
   unsigned char Char[4]; // UTF-8 string
   int           Line;
   TYPE          Type;
  } Char;

  FILE_SYSTEM fs;

  int      Line;
  unsigned Index;
  STRING   Buffer;

  // These don't modify Index
  int Space  (); // Single space, returns code length of the space
  int Spaces (); // Scans for multiple spaces, returns the number of bytes
  int Newline(); // Single newline, returns code length of the newline

  bool GetChar();

 private: // Phase 2 members
  bool PrevIsNewline; // Used to detect directives

  inline bool Digit();
  inline bool HexDigit();
  inline bool NonDigit();

  bool Number    (STRING& Token);
  bool String    (STRING& Token);
  bool Comment   (STRING& Token);
  bool Character (STRING& Token);
  bool Identifier(STRING& Token);
  bool Punctuator(STRING& Token);

 public: // Public interface
  SCANNER();
 ~SCANNER();

  bool Open(const char* Filename);

  // Allocates memory and return a new token; null on error or end-of-file
  TOKEN* GetToken    ();
  TOKEN* GetDirective(); // Scans through the code to find the next directive:
                         // used to implement skipping over #if bodies
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
