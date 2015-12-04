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

#include "Operators.h"
#include "FileSystem.h"
//------------------------------------------------------------------------------

class SCANNER{
 public: // Public types
  enum TYPE{
   tSpace,     // Used in CHAR only
   tNewline,
   tDirective, // Newline {Space} "#" {Space} Identifier; store only ident.
   tIdentifier,
   tNumber,
   tFixedPointCast,
   tCharacter,
   tString,
   tOperator,
   tOther,
   tEOF,
  };

  struct PP_TOKEN{ // Preprocessing token
   TYPE      Type;
   STRING    Body;
   int       Line;
   bool      PrecedingSpace; // Token preceded by comment, space or newline
   STRING    Comment;        // Concatenation of any preceding comments
   PP_TOKEN* Next;           // Used when storing macro bodies

   PP_TOKEN();
  };

 private: // General private members
  bool error;
  void Error(const char* Message);

 private: // Phase 1 members
  struct CHAR{
   unsigned char Char[4]; // UTF-8 string
   int           Line;
   TYPE          Type; // tSpace, tNewline, tOther or tEOF
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

  bool Number        (STRING& Body);
  bool FixedPointCast(STRING& Body);
  bool String        (STRING& Body);
  bool Comment       (STRING& Body);
  bool Character     (STRING& Body);
  bool Identifier    (STRING& Body);
  bool Operator      (STRING& Body);

 public: // Public interface
  SCANNER();

  STRING Filename; // Read-only
  bool   Open(const char* Filename);

  bool GetToken(PP_TOKEN* ppToken);

  // Scans through the code to find the next directive: used to implement
  // skipping over #if bodies
  bool GetDirective(PP_TOKEN* ppToken);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
