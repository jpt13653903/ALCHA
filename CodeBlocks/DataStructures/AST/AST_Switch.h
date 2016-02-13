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

#ifndef AST_Switch_h
#define AST_Switch_h
//------------------------------------------------------------------------------

#include "AST_Expression.h"
//------------------------------------------------------------------------------

struct AST_Switch: public AST_Base{
  struct CASE{
   AST_Expression* Expressions; // List through the AST_Base::Next pointer
   AST_Base      * Statements;

   CASE* Next; // The next case (this can be turned into a BST later...)

   CASE();
  ~CASE();
  };
  CASE          * Cases;
  AST_Base      * Default;
  AST_Expression* Expression;

  AST_Switch(int Line);
 ~AST_Switch();

  void Display();
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

