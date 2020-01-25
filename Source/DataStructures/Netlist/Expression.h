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

#ifndef Netlist_Expression_h
#define Netlist_Expression_h
//------------------------------------------------------------------------------

#include <vector>
//------------------------------------------------------------------------------

#include "Base.h"
#include "Number.h"
#include "AST/Expression.h"
//------------------------------------------------------------------------------

namespace NETLIST{
  struct EXPRESSION{
    enum EXPRESSION_TYPE{
      String,
      Literal,
      Array,
      Object,

      VectorConcatenate,
      ArrayConcatenate,

      FunctionCall, // Left is the function name; right is the parameter list
      Slice,

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

    // The line and filename that resulted in this node, used for error reporting
    int         Line;
    std::string Filename;

    // The resulting fixed-point type of the expression (if appropriate)
    // If Width is not 0, the contents is in binary representation.
    // If this is a literal, Value is real-world when Width is 0, else Value
    // is the binary representation (but still in rational form).
    bool   Signed;
    int    Width; // 0 => not applicable or not defined
    NUMBER FullScale;

    BASE*                    ObjectRef; // Refers to another object
    NUMBER                   Value;     // Only used for numerical literals
    std::string              StrValue;  // Only used for string literals
    std::vector<EXPRESSION*> Elements;  // Used for arrays, concatenation, etc.

    // Left and Right operands
    EXPRESSION* Left;
    EXPRESSION* Right;

    EXPRESSION(int Line, const std::string& Filename, EXPRESSION_TYPE ExpressionType);
    EXPRESSION(EXPRESSION* Expression); // Makes a copy
   ~EXPRESSION(); // Also deletes Left, Right and Elements

    void Display();
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

