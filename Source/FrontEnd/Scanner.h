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

/** Converts the input file to tokens:
Processing:
- Ignores all spaces and comments
- Keeps track of line numbers
- Translates escape sequences in strings
- Concatenates adjacent strings.

Tokens:
- Keyword (as enumeration)
- Identifier (as UTF-8 string)
- Operator (as enumeration)
- Literal (as mpr)
*/
//------------------------------------------------------------------------------

#ifndef Scanner_h
#define Scanner_h
//------------------------------------------------------------------------------

#include <map>
#include <string>
#include <time.h>
//------------------------------------------------------------------------------

#include "Token.h"
#include "Number.h"
#include "TokenTree.h"
#include "Dictionary.h"
#include "FileWrapper.h"
//------------------------------------------------------------------------------

class SCANNER{
  private:
    bool error;
    void Error(const char* Message);

    int      Line;
    unsigned Index;
    byte*    Buffer;

    bool LineComment  ();
    bool TodoComment  ();
    bool BlockComment ();
    bool NestedComment();
    void WhiteSpace   (); // Ignores spaces, new-lines and comments

    bool Digit   (); // Returns true on decimal digit
    bool NonDigit();

    bool     GetDigit   (unsigned* Digit, unsigned Base);
    unsigned GetExponent(bool    * Sign , TOKEN* Token);
    bool     GetNumber  (TOKEN   * Token, unsigned Base);

    bool Identifier(TOKEN* Token);
    bool Operator  (TOKEN* Token);
    bool Literal   (TOKEN* Token);
    bool String    (TOKEN* Token);

  public: // Public interface
    SCANNER();
   ~SCANNER();

    std::string Filename; // Read-only
    bool Open(const char* Filename);

    bool GetToken(TOKEN* Token);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
