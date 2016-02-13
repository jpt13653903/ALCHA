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

#ifndef AST_HDL_h
#define AST_HDL_h
//------------------------------------------------------------------------------

#include "AST_Assignment.h"
#include "AST_Definition.h"
//------------------------------------------------------------------------------

struct AST_HDL: public AST_Base{
  byte*           Identifier;
  AST_Expression* Files; // Only string constants are allowed
  AST_Assignment* Parameters;
  AST_Definition* Ports;

  AST_HDL(int Line);
 ~AST_HDL();

  void Display();
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

