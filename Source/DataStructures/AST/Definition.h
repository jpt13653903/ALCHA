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

namespace Netlist{
    class Base;
    class Synthesisable;
}
//------------------------------------------------------------------------------

namespace AST{
    class Definition: public Base{
        public:
            struct Array{
                Expression* size;
                Array     * next; // Next dimension of the array

                Array();
                Array(const Array& array);
               ~Array();
            };

            struct Identifier{
                std::string identifier;
                Array*      array;      // Null when this is a scalar

                Assignment* initialiser;

                // These are used for function definitions.
                bool        function; // True when this is a function definition
                Definition* parameters; // List of identifiers (calls are duck-typed)
                Base*       functionBody;

                Identifier* next;

                Identifier();
                Identifier(const Identifier& identifier);
               ~Identifier();
            };
        //--------------------------------------------------------------------------

        public:
            enum class Direction{Inferred = 0, Input, Output, Bidirectional} direction;

            std::list<Base*> parameters; // Expression or Assignment
            Assignment* attributes;
            Identifier* identifiers;
        //--------------------------------------------------------------------------

        protected:
            void copyMembers(Definition* copy);
            bool verifyNotDefined(Identifier* identifier);

            void displayParameters ();
            void displayAttributes ();
            void displayIdentifiers();
            void displayDefinition (const char* type);
        //--------------------------------------------------------------------------

        public:
                     Definition(int line, const char* filename, Type definitionType);
            virtual ~Definition();

            bool isDefinition() override;

            void validateMembers() override;
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
