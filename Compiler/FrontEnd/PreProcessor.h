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
//------------------------------------------------------------------------------

class PREPROCESSOR{
 public: // Public types
  enum TYPE{
   tIdentifier,
   tKeyword,
   tLiteral, // Character; float; fixed-point; integer; etc...
   tOperator,
   tString,
  };

  struct TOKEN{
   TYPE Type;
   STRING String;  // Identifier; String; Comment
   STRING Comment; // Concatenation of preceding comments
//   OPERATOR Operator;

  };

 private:
  SCANNER::TOKEN* ppTokens; // Linked list

 public:
  PREPROCESSOR();
 ~PREPROCESSOR();

  bool Open(const char* Filename); // Also runs through the stages 1 to 3
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
