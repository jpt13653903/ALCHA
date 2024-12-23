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

#ifndef Netlist_NameSpace_h
#define Netlist_NameSpace_h
//------------------------------------------------------------------------------

#include <map>
#include <list>
//------------------------------------------------------------------------------

#include "Base.h"
//------------------------------------------------------------------------------

namespace Netlist{
    class NameSpace: public Base{
        public:
            AST::Base* ast;
            std::map<std::string, Base*> symbols;

                     NameSpace(int line, const std::string& filename,
                               const char* name, Type namespaceType);
            virtual ~NameSpace(); // Also cleans up the children

            AST::Expression* getExpression(int line, const std::string& filename) override;
            bool assign   (AST::Expression* expression) override;
            bool rawAssign(AST::Expression* expression) override;

            bool hasCircularReference(Base* object) override;

            bool isNameSpace() override;

            Base* getMember(const std::string& name) override;

            void validate() override;
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

