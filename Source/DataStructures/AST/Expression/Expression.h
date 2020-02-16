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

namespace NETLIST{
  class BASE;
}
//------------------------------------------------------------------------------

namespace AST{
  class EXPRESSION: public BASE{
    protected:
      void DisplayStart();
      void DisplayEnd  ();

    public:
      std::string    Name;      // Used for identifiers
      NUMBER         Value;     // Only used for numerical literals
      std::string    StrValue;  // Only used for string literals
      NETLIST::BASE* ObjectRef; // Used for "Object" type

      // Left and Right operands
      EXPRESSION* Left;
      BASE*       Right; // Can be expression or assignment (for function calls)

      EXPRESSION(int Line, const char* Filename, TYPE ExpressionType);
      virtual ~EXPRESSION();

      // These functions work on the linked list stored in Right
      BASE* Element(int n);
      int   ElementCount();

      // Returns a copy of this instance
      virtual BASE* Copy(bool CopyNext) = 0;

      virtual void Display() = 0;
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

