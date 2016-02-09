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

#ifndef TypeDef_h
#define TypeDef_h
//------------------------------------------------------------------------------

#include "MyString.h"
//------------------------------------------------------------------------------

struct TYPE_DEF;
//------------------------------------------------------------------------------

struct SYMBOL{
 STRING    Name;
 TYPE_DEF* Type;

 SYMBOL* Next; // Linked list used for parameters

 SYMBOL(const byte* Name, TYPE_DEF* Type);
~SYMBOL();
};
//------------------------------------------------------------------------------

struct TYPE_DEF{
 bool Synthesiseable; // compile-time otherwise

 enum TYPE{
  // Compile-time
  Integer,
  Rational,
  Float,
  Complex,

  // Synthesiseable
  Signed,
  Unsigned,

  // Structured Types
  Array,
  Class,

  Function
 } Type;

 union{
  struct{ // Float and Complex
   int FloatPrecision;
  };

  struct{ // Signed and Unsigned
   int IntegerBits;
   int FractionBits;
  };

  struct{ // Array
   TYPE_DEF* BaseType;
   int       Size;
  };

  struct{ // Function
   TYPE_DEF* ReturnType; // Funtion
   SYMBOL*   Parameters; // Linked List
  };
 };
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------


