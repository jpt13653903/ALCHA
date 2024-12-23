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

#ifndef AST_Switch_h
#define AST_Switch_h
//------------------------------------------------------------------------------

#include <vector>
#include "Expression.h"
//------------------------------------------------------------------------------

namespace AST{
    struct Switch: public Base{
        struct Case{
            std::vector<Expression*> expressions;
            Base* statements;

            Case* next; // The next case (this can be turned into a BST later...)

            Case();
            Case(const Case& source);
           ~Case();
        };
        Case      * cases;
        Base      * defaultCase;
        Expression* expression;

        Switch(int line, std::string& filename);
        Switch(int line, const char*  filename);
       ~Switch();

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

