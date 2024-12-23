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
#include "Utilities.h"
#include "TokenTree.h"
#include "Dictionary.h"
#include "FileWrapper.h"
//------------------------------------------------------------------------------

class Scanner{
    private:
        bool error;
        void printError(const char* message);

        int      line;
        unsigned index;
        byte*    buffer;

        bool lineComment  ();
        bool todoComment  ();
        bool blockComment ();
        bool nestedComment();
        void whiteSpace   (); // Ignores spaces, new-lines and comments

        bool digit   (); // Returns true on decimal digit
        bool nonDigit();

        bool     getDigit   (unsigned* digit, unsigned base);
        unsigned getExponent(bool    * sign , Token* token);
        bool     getNumber  (Token   * token, unsigned base);

        bool getIdentifier(Token* token);
        bool getOperator  (Token* token);
        bool getLiteral   (Token* token);
        bool getString    (Token* token);

    public: // Public interface
        Scanner();
       ~Scanner();

        std::string filename; // Read-only
        bool open(const char* filename);

        bool getToken(Token* token);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
