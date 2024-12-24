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

#ifndef AST_ClassDefinition_h
#define AST_ClassDefinition_h
//------------------------------------------------------------------------------

#include "Definition.h"
#include "Assignment.h"
//------------------------------------------------------------------------------

namespace AST{
    struct ClassDefinition: public Base{
        struct Parent{ // Link-list node for parent classes
            Expression* className;  // This class inherits from Parent
            std::list<Base*> parameters; // Parent constructor call

            Parent* next;

            Parent();
            Parent(const Parent& parent);
           ~Parent(); // Also deletes the rest of the list
        };

        Assignment* attributes;

        std::string identifier;
        Definition* parameters; // Constructor parameters

        Parent* parents;

        Base* body;

        ClassDefinition(int line, std::string& filename);
        ClassDefinition(int line, const char*  filename);
       ~ClassDefinition();

        Base* copy() override;

        bool runAST() override;
        bool getVerilog(std::string& body) override;

        void display() override;

        void validateMembers() override;
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

