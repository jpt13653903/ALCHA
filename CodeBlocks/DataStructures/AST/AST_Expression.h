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

#include "AST.h"
#include "Number.h"
//------------------------------------------------------------------------------

struct AST_Expression: public AST_Base{
  enum EXPRESSION_TYPE{
   String,
   Literal,
   Identifier,

   FunctionCall, // Left is the function name; right is the parameter list
   Slice,
   Dot,
   SliceAll,
   ArrayConcatenate,
   Range, // Left = from; Right = to; Right->Next = step

   Increment, // If child is on the left, post-increment
   Decrement,  // If child is on the left, post-increment

   Negate,
   Bit_NOT,

   AND_Reduce,
   NAND_Reduce,
   OR_Reduce,
   NOR_Reduce,
   XOR_Reduce,
   XNOR_Reduce,
   Logical_NOT,

   FP_Cast,

   Concatenate,
   Replicate,

   Multiply,
   Divide,
   Modulus,

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

  // Pointers so that not all expression nodes have instances, wasting space
  byte  * Name;     // ID obtained from IdentifierTree
  NUMBER* Value;    // Only used for numerical literals
  STRING* StrValue; // Only used for string literals

  // Left and Right operands
  AST_Expression* Left;
  AST_Expression* Right;

  AST_Expression(int Line, EXPRESSION_TYPE ExpressionType);
 ~AST_Expression();

  void Display();
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

