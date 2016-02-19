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

#ifndef Token_h
#define Token_h
//------------------------------------------------------------------------------

#include "Number.h"
#include "MyString.h"
//------------------------------------------------------------------------------

struct TOKEN{
 enum TYPE{
  Unknown = 0, // also used to designate errors, etc.

  // White-space ...............................................................
  Space,   // These are only used in the scanner
  Newline,

  // Keywords ..................................................................
  Target,

  Void,
  Auto,
  Pin,
  Sig,
  Clk,

  Int,
  Rat,
  Float,
  Complex,

  In,
  Out,
  Signed,

  Class,

  If,
  Else,
  For,
  While,
  Loop,
  Switch,
  Case,
  Default,

  Import,
  As,

  Return,
  Break,
  Continue,

  RTL,
  FSM,
  HDL,

  // Operators .................................................................
  Increment, // ++
  Decrement, // --
  FP_Cast,   // '
  To,        // ->
  Step,      // @
  Dot,       // .
  Dot_Curly, // .{

  Bit_AND,  //  &
  Bit_NAND, // ~&
  Bit_OR,   //  |
  Bit_NOR,  // ~|
  Bit_XOR,  //  ^
  Bit_XNOR, // ~^
  Bit_NOT,  // ~

  Bit_Repeat, // \                                                             "

  Add,      // +
  Subtract, // -
  Multiply, // *
  Divide,   // /
  Modulus,  // %

  Shift_Left,  // <<
  Shift_Right, // >>

  Less,          // <
  Greater,       // >
  Less_Equal,    // <=
  Greater_Equal, // >=
  Equal,         // ==
  Not_Equal,      // !=

  Logical_NOT, // !
  Logical_AND, // &&
  Logical_OR,  // ||

  Conditional, // ?

  Assign,             //   =
  Raw_Assign,         //  :=
  Append_Assign,      //  @=
  Add_Assign,         //  +=
  Subtract_Assign,    //  -=
  Multiply_Assign,    //  *=
  Divide_Assign,      //  /=
  Modulus_Assign,     //  %=
  AND_Assign,         //  &=
  OR_Assign,          //  |=
  XOR_Assign,         //  ^=
  Shift_Left_Assign,  // <<=
  Shift_Right_Assign, // >>=

  // Punctuators ...............................................................
  OpenRound,   // (
  CloseRound,  // )

  OpenSquare,  // [
  CloseSquare, // ]

  OpenCurly,   // {
  CloseCurly,  // }

  Comma,       // ,
  Colon,       // :
  Semicolon,   // ;

  // Other types ...............................................................
  Identifier,
  Literal,
  String,

  // Aliases ...................................................................
  OpenAngle         = Less,     // <
  CloseAngle        = Greater,  // >
  Negate            = Subtract, // -
  Bit_Concatenate   = Colon,    // :
  Array_Concatenate = Bit_NOT   // ~

 } Type;

 int    Line;  // The line number
 byte*  ID;    // The identifier ID
 STRING Data;  // The string contents; otherwise the original token characters
 NUMBER Value; // Literal value

 TOKEN();
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
