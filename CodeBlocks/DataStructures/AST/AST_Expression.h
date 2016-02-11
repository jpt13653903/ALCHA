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
   Dot,
   Identifier,
   Literal
  } ExpressionType;

  // Pointers so that not all expression nodes have instances, wasting space
  STRING* Name;
  NUMBER* Value;

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

