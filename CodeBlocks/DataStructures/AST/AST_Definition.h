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

#ifndef AST_Definition_h
#define AST_Definition_h
//------------------------------------------------------------------------------

#include "AST_Expression.h"
//------------------------------------------------------------------------------

struct AST_Definition: public AST_Base{
  enum DEFINITION_TYPE{
   Pin, Sig, Clk,
   Int, Rat, Float, Complex
  } DefinitionType;

  struct ARRAY{
   AST_Expression* Size;
   ARRAY         * Next;

   ARRAY();
  ~ARRAY();
  };

  struct IDENTIFIER{
   STRING Identifier;
   ARRAY* Array; // Null when this is a scalar

   IDENTIFIER* Next;

   IDENTIFIER();
  ~IDENTIFIER();
  };

  enum DIRECTION{Bidirectional = 0, In, Out} Direction;
  bool Signed;

  // Expressions for fixed-point casts
  AST_Expression* IntegerBits;
  AST_Expression* FractionBits;

  DICTIONARY Attributes;

  IDENTIFIER* Identifiers;

  AST_Definition(int Line, DEFINITION_TYPE DefinitionType);
 ~AST_Definition();

  void Display();
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
