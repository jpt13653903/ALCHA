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

#include "../Base.h"
#include "Number.h"
//------------------------------------------------------------------------------

namespace AST{
  class EXPRESSION: public BASE{
    protected:
      void DisplayStart();
      void DisplayEnd  ();

    public:
      // Left and Right operands
      EXPRESSION* Left;
      BASE*       Right; // Can be expression or assignment (for function calls)

      EXPRESSION(int Line, const char* Filename, TYPE ExpressionType);
      virtual ~EXPRESSION();

      virtual EXPRESSION* Evaluate() = 0;
      virtual EXPRESSION* Simplify() = 0;
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

