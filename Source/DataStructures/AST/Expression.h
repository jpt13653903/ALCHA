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

#ifndef AST_Expression_h
#define AST_Expression_h
//------------------------------------------------------------------------------

#include "Base.h"
#include "Number.h"
//------------------------------------------------------------------------------

namespace AST{
  struct EXPRESSION: public BASE{
    enum EXPRESSION_TYPE{
      String,
      Literal,
      Array,
      Identifier,

      VectorConcatenate,
      ArrayConcatenate,

      FunctionCall, // Left is the function name; right is the parameter list
      Slice,
      AccessMember,
      AccessMemberSafe,
      AccessAttribute,
      Range, // Left = from; Right = to; Right->Next = step

      Increment, // If child is on the left, post-increment
      Decrement, // If child is on the left, post-decrement
      Factorial,

      Negate,
      Bit_NOT,
      Raw,     // Unary operator to cast to "raw bits", or "unsigned int"

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

    std::string  Name;
    NUMBER     * Value;    // Only used for numerical literals
    std::string* StrValue; // Only used for string literals

    // Left and Right operands
    EXPRESSION* Left;
    BASE*       Right; // Can be expression or assignment (for function calls)

    EXPRESSION(
      int             Line,
      const char*     Filename,
      EXPRESSION_TYPE ExpressionType
    );
   ~EXPRESSION();

    void Display();
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

