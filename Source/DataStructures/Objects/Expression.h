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

#ifndef Objects_Expression_h
#define Objects_Expression_h
//------------------------------------------------------------------------------

#include <vector>
//------------------------------------------------------------------------------

#include "Base.h"
#include "Number.h"
#include "AST/Expression.h"
//------------------------------------------------------------------------------

namespace OBJECTS{
  struct EXPRESSION: public BASE{
    enum EXPRESSION_TYPE{
      String,
      Literal,
      Array,
      Object,
      Attribute, // Attribute "Name" of object "ObjectRef"

      VectorConcatenate,
      ArrayConcatenate,

      FunctionCall, // Left is the function name; right is the parameter list
      Slice,
      AccessMember,
      AccessMemberSafe,
      AccessAttribute,

      Increment, // If child is on the left, post-increment
      Decrement, // If child is on the left, post-decrement
      Factorial,

      Negate,
      Bit_NOT,

      AND_Reduce,
      NAND_Reduce,
      OR_Reduce,
      NOR_Reduce,
      XOR_Reduce,
      XNOR_Reduce,
      Logical_NOT,

      Cast,

      Replicate,

      Multiply,
      Divide,
      Modulus,
      Exponential,

      Add,
      Subtract,

      Shift_Left,
      Shift_Right,

      Less,
      Greater,
      Less_Equal,
      Greater_Equal,

      Equal,
      Not_Equal,

      Bit_AND,
      Bit_NAND,
      Bit_OR,
      Bit_NOR,
      Bit_XOR,
      Bit_XNOR,

      Logical_AND,
      Logical_OR,

      Conditional
    } ExpressionType;

    bool Raw; // The literal has been cast to raw bits

    BASE*                    ObjectRef; // Refers to another object
    NUMBER                   Value;     // Only used for numerical literals
    std::string              StrValue;  // Only used for string literals
    std::vector<EXPRESSION*> Elements;  // Used arrays, concatenation, etc.

    // Left and Right operands
    EXPRESSION* Left;
    EXPRESSION* Right;

    EXPRESSION(EXPRESSION_TYPE ExpressionType);
   ~EXPRESSION();

    void Display();
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

