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

/** Runs the following phases:
1) Run the preprocessor
   - Include files
   - Expand macros
   - Handle #if blocks
   - etc.
2) Translate escape sequences in strings
3) Remove new-lines (the comment of the newline is concatenated with the
   comment of the next pp-token)
4) Concatenate adjacent strings (the associated comments are also concatenated)
5) Translate pp-tokens to tokens
*/
//------------------------------------------------------------------------------

#ifndef PreProcessor_h
#define PreProcessor_h
//------------------------------------------------------------------------------

#include "Scanner.h"
#include "Keyword.h"
//------------------------------------------------------------------------------

class PREPROCESSOR{
 public: // Public types
  enum TYPE{
   tIdentifier,
   tKeyword,
   tOperator,
   tString,
   tFixedPoint, // Includes integers and character literals
   tFloat,       // Normal 80-bit floating point
   tEOF
  };

  struct FIXED_POINT{
   unsigned  IntegerBits;
   unsigned  FractionBits; // 0 => integer
   unsigned* Bits; // As many elements as the number of bits require, stored
                   // in little-endian
   FIXED_POINT();
  ~FIXED_POINT();

   bool Init(unsigned IntegerBits, unsigned FractionBits);
  };

  struct TOKEN{
   TYPE        Type;
   int         Line;
   STRING      String;  // Identifier; String
   STRING      Comment; // Concatenation of preceding comments
   FIXED_POINT FixedPoint;
   union{
    OPERATOR_CODE Operator;
    KEYWORD_CODE  Keyword;
    long double   Float;
   };
  };

 private:
  KEYWORD  Keywords;
  SCANNER* Scanner; // The scanner of the current file
  SCANNER::TOKEN ppToken;

 public:
  PREPROCESSOR();
 ~PREPROCESSOR();

  bool Open(const char* Filename);

  bool GetToken(TOKEN* Token);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
