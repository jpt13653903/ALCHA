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

#ifndef AST_Expression_Identifier_h
#define AST_Expression_Identifier_h
//------------------------------------------------------------------------------

#include "../Expression.h"
#include "Constants.h"
//------------------------------------------------------------------------------

namespace AST{
    struct Identifier: public Expression{
        std::string name;

        Identifier(int line, const std::string& filename);
        Identifier(int line, const char*        filename);
       ~Identifier();

        Base* copy() override;

        bool getVerilog(std::string& body) override;

        // Evaluates to an Object
        Expression* evaluate() override;

        int     getWidth    () override;
        Number& getFullScale() override;
        bool    getSigned   () override;

        bool hasCircularReference(Netlist::Base* object) override;
        void populateUsed() override;

        Expression* removeTempNet(int width, bool isSigned) override;

        void display() override;

        void validateMembers() override;
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

