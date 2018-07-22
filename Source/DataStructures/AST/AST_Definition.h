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

#include "AST_Parameter.h"
#include "AST_Assignment.h"
//------------------------------------------------------------------------------

struct AST_Definition: public AST_Base{
  enum DEFINITION_TYPE{
   Void, Auto, // Used for functions only
   Pin, Net, Clk,
   Byte, Char, Int, Rat, Float, Complex,
   Func, // Function pointer
   ClassInstance
  } DefinitionType;
  AST_Expression* ClassName; // For class instances

  struct ARRAY{
   AST_Expression* Size;
   ARRAY         * Next; // Next dimension of the array

   ARRAY();
  ~ARRAY();
  };

  struct IDENTIFIER{
   byte * Identifier; // ID obtained via IdentifierTree
   ARRAY* Array;      // Null when this is a scalar

   AST_Assignment* Initialiser;

   // These are used for function definitions.
   bool           Function; // True when this is a function definition
   AST_Parameter* Parameters; // List of identifiers (calls are duck-typed)
   AST_Base     * FunctionBody;

   IDENTIFIER* Next;

   IDENTIFIER();
  ~IDENTIFIER();
  };

  enum DIRECTION{Bidirectional = 0, In, Out} Direction;

  // Expression for fixed-point casts
  AST_Expression::EXPRESSION_TYPE FormatType; // Basic vs. Scaled
  AST_Expression*                 Format; // The "Right" of the fixed-point cast

  DICTIONARY Attributes;

  IDENTIFIER* Identifiers;

  AST_Definition(
   int             Line,
   const byte*     Filename,
   DEFINITION_TYPE DefinitionType);
 ~AST_Definition();

  void Display();
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------