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

#ifndef Netlist_Pin_h
#define Netlist_Pin_h
//------------------------------------------------------------------------------

#include "PinComponent.h"
#include "AST/Expression.h"
//------------------------------------------------------------------------------

namespace Netlist{
    struct Pin: public Synthesisable{
        PinComponent* driver;
        PinComponent* enabled;

        Pin(int line, const std::string& filename, const char* name);
       ~Pin();

        AST::Expression* getExpression(int line, const std::string& filename) override;
        bool assign   (AST::Expression* expression) override;
        bool rawAssign(AST::Expression* expression) override;

        bool hasCircularReference(Base* object) override;
        void populateUsed(bool setUsed) override;

        Base* getMember(const std::string& name) override;

        void display(int indent = 0) override;

        void validate() override;
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
