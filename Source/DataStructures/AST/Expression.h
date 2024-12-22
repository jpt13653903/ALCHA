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

namespace NETLIST{
    class BASE;
}
//------------------------------------------------------------------------------

namespace AST{
    class EXPRESSION: public BASE{
        protected:
            void DisplayStart();
            void DisplayEnd  ();

            // Turns the current expression into a temporary net object
            EXPRESSION* MakeObject();
        //--------------------------------------------------------------------------

        public:
            // Left and Right operands
            EXPRESSION* Left;
            EXPRESSION* Right;

                              EXPRESSION(int Line, const char* Filename, TYPE ExpressionType);
            virtual ~EXPRESSION();

            bool IsExpression() override;

            bool RunAST() override;

            // Evaluates the expression as far as possible, creating new wires if required
            // Returns the instance of the node to use as replacement
            // For example: Node = Node->Evaluate() might change the value of Node
            virtual EXPRESSION* Evaluate() = 0;

            // Returns the width of the result, if known.  Issues an error if the
            // width is not defined
            // These functions must be called after a call to Evaluate()
            virtual int     GetWidth    () = 0;
            virtual NUMBER& GetFullScale() = 0;
            virtual bool    GetSigned   () = 0;

            // Used for fixed-point scaling...
            // If the scaling is not a power-of-two, it also synthesises a multiplier.
            // The resulting object might be larger that the Width requested in order
            // to prevent loss of precision.
            EXPRESSION* ScaleWith(NUMBER Scale, int Width, NUMBER FullScale);

            // Check for circular reference to the netlist object specified
            virtual bool HasCircularReference(NETLIST::BASE* Object) = 0;

            // Populates the "Used" flag so that the back-end can remove unused objects.
            virtual void PopulateUsed() = 0;

            // If the expression references an object, which in turn references
            // another object of the same width and signedness, the temporary net in
            // the middle is removed.
            virtual EXPRESSION* RemoveTempNet(int Width, bool Signed) = 0;
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

