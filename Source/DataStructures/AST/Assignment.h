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

#ifndef AST_Assignment_h
#define AST_Assignment_h
//------------------------------------------------------------------------------

#include <list>
//------------------------------------------------------------------------------

#include "Expression.h"
//------------------------------------------------------------------------------

namespace Netlist{
    class Base;
}
//------------------------------------------------------------------------------

namespace AST{
    class Assignment: public Base{
        public:
            // Left and Right operands
            Expression* left;
            Expression* right;

        protected:
            // Populates a list of existing expressions, except when the target is an
            // undefined attribute, in which case the attribute is created first.
            typedef std::list<Netlist::Base*> TargetList;
            bool addLhsObject(Netlist::Base* object, TargetList& list);
            bool getLHS(Expression* node, TargetList& list);

        protected:
            void displayAssignment(const char* theOperator);

        public:
                     Assignment(int line, const char* filename, Type assignmentType);
            virtual ~Assignment();

            bool isAssignment() override;
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

