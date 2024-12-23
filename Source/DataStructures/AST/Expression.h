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

#ifndef AST_Expression_Expression_h
#define AST_Expression_Expression_h
//------------------------------------------------------------------------------

#include "Base.h"
#include "Number.h"
//------------------------------------------------------------------------------

namespace Netlist{
    class Base;
}
//------------------------------------------------------------------------------

namespace AST{
    class Expression: public Base{
        protected:
            void displayStart();
            void displayEnd  ();

            // turns the current expression into a temporary net object
            Expression* makeObject();
        //--------------------------------------------------------------------------

        public:
            // Left and Right operands
            Expression* left;
            Expression* right;

                     Expression(int line, const char* filename, Type expressionType);
            virtual ~Expression();

            bool isExpression() override;

            bool runAST() override;

            // Evaluates the expression as far as possible, creating new wires if required
            // Returns the instance of the node to use as replacement
            // For example: Node = Node->Evaluate() might change the value of Node
            virtual Expression* evaluate() = 0;

            // Returns the width of the result, if known.  Issues an error if the
            // width is not defined
            // These functions must be called after a call to Evaluate()
            virtual int     getWidth    () = 0;
            virtual Number& getFullScale() = 0;
            virtual bool    getSigned   () = 0;

            // Used for fixed-point scaling...
            // If the scaling is not a power-of-two, it also synthesises a multiplier.
            // The resulting object might be larger that the Width requested in order
            // to prevent loss of precision.
            Expression* scaleWith(Number scale, int width, Number fullScale);

            // Check for circular reference to the netlist object specified
            virtual bool hasCircularReference(Netlist::Base* object) = 0;

            // Populates the "Used" flag so that the back-end can remove unused objects.
            virtual void populateUsed() = 0;

            // If the expression references an object, which in turn references
            // another object of the same width and signedness, the temporary net in
            // the middle is removed.
            virtual Expression* removeTempNet(int width, bool isSigned) = 0;
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

