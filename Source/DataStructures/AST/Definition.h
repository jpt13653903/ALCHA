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

#ifndef AST_Definition_h
#define AST_Definition_h
//------------------------------------------------------------------------------

#include <list>
#include "Assignment.h"
//------------------------------------------------------------------------------

namespace NETLIST{
  class BASE;
  class SYNTHESISABLE;
}
//------------------------------------------------------------------------------

namespace AST{
  class DEFINITION: public BASE{
    public:
      struct ARRAY{
        EXPRESSION* Size;
        ARRAY     * Next; // Next dimension of the array

        ARRAY();
        ARRAY(const ARRAY& Array);
       ~ARRAY();
      };

      struct IDENTIFIER{
        std::string Identifier;
        ARRAY*      Array;      // Null when this is a scalar

        ASSIGNMENT* Initialiser;

        // These are used for function definitions.
        bool        Function; // True when this is a function definition
        DEFINITION* Parameters; // List of identifiers (calls are duck-typed)
        BASE*       FunctionBody;

        IDENTIFIER* Next;

        IDENTIFIER();
        IDENTIFIER(const IDENTIFIER& Identifier);
       ~IDENTIFIER();
      };
    //--------------------------------------------------------------------------

    public:
      enum class DIRECTION{Inferred = 0, Input, Output, Bidirectional} Direction;

      std::list<BASE*> Parameters; // Expression or Assignment
      ASSIGNMENT* Attributes;
      IDENTIFIER* Identifiers;
    //--------------------------------------------------------------------------

    protected:
      void CopyMembers(DEFINITION* Copy);
      bool VerifyNotDefined(IDENTIFIER* Identifier);

      void DisplayParameters ();
      void DisplayAttributes ();
      void DisplayIdentifiers();
      void DisplayDefinition (const char* Type);
    //--------------------------------------------------------------------------

    public:
               DEFINITION(int Line, const char* Filename, TYPE DefinitionType);
      virtual ~DEFINITION();

      bool IsDefinition() override;

      void ValidateMembers() override;
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
