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
      // width is not defined (like an uncast literal, for instance)
      virtual int     GetWidth    () = 0;
      virtual NUMBER& GetFullScale() = 0;
      virtual bool    GetSigned   () = 0;

      // Used for fixed-point scaling...
      // If the scaling is not a power-of-two, it also synthesises a multiplier.
      EXPRESSION* ScaleWith(NUMBER& Scale, int Width, NUMBER& FullScale);

      // Check for circular reference to the netlist object specified
      virtual bool HasCircularReference(NETLIST::BASE* Object) = 0;
      virtual void PopulateUsed() = 0;
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

