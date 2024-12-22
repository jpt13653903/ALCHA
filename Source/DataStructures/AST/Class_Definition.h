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

#ifndef AST_Class_Definition_h
#define AST_Class_Definition_h
//------------------------------------------------------------------------------

#include "Definition.h"
#include "Assignment.h"
//------------------------------------------------------------------------------

namespace AST{
    struct CLASS_DEFINITION: public BASE{
        struct PARENT{ // Link-list node for parent classes
            EXPRESSION* ClassName;  // This class inherits from Parent
            std::list<BASE*> Parameters; // Parent constructor call

            PARENT* Next;

            PARENT();
            PARENT(const PARENT& Parent);
          ~PARENT(); // Also deletes the rest of the list
        };

        ASSIGNMENT* Attributes;

        std::string Identifier;
        DEFINITION* Parameters; // Constructor parameters

        PARENT* Parents;

        BASE* Body;

        CLASS_DEFINITION(int Line, std::string& Filename);
        CLASS_DEFINITION(int Line, const char*  Filename);
      ~CLASS_DEFINITION();

        BASE* Copy() override;

        bool RunAST() override;
        bool GetVerilog(std::string& Body) override;

        void Display() override;

        void ValidateMembers() override;
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

