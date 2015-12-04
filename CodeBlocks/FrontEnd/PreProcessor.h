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

#include "Number.h"
#include "Scanner.h"
#include "Keywords.h"
//------------------------------------------------------------------------------

class PREPROCESSOR{
 public: // Public types
  enum TYPE{
   tIdentifier,
   tKeyword,
   tOperator,
   tString,
   tNumber,
   tFixedPointCast, // Specifies fixed-point format
   tEOF
  };

  struct TOKEN{
   TYPE   Type;
   int    Line;
   STRING File;

   STRING String;  // Identifier; String
   STRING Comment; // Concatenation of preceding comments
   NUMBER Number;
   union{
    OPERATOR_CODE Operator;
    KEYWORD_CODE  Keyword;
    struct{
     int IntegerBits;
     int FractionBits;
    } FixedPointFormat; // Used for tFixedPointCast
   };
  };

 private:
  bool error;
  void Error(const char* Message);

 private:
  STRING Path;

  /// @todo Keep track of stack depth and limit (for circular inclusion)
  ///       Or check for filename in the list and prevent circular inclusion
  ///       - No: the user might want to include the same file twice with
  ///         different parameters.
  struct SCANNER_STACK{
   SCANNER      * Scanner; // The scanner of the current file
   SCANNER_STACK* Next;

   SCANNER_STACK();
  ~SCANNER_STACK();
  }* Scanner_Stack;

  KEYWORDS Keywords;
  SCANNER::PP_TOKEN ppToken; // The current pp-token

  bool GetPPToken(); // Also runs directives

  bool TranslateHex    (NUMBER& Number);
  bool TranslateOctal  (NUMBER& Number);
  bool TranslateBinary (NUMBER& Number);
  bool TranslateDecimal(NUMBER& Number);

  bool TranslateEscapes       (STRING& Result);
  bool TranslateNumber        (NUMBER& Number);
  bool TranslateFixedPointCast(TOKEN * Token );

 public:
  PREPROCESSOR();
 ~PREPROCESSOR();

  bool Open(const char* Filename);

  bool GetToken(TOKEN* Token);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
