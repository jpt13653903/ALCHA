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

#include "Alias.h"
//------------------------------------------------------------------------------

using namespace AST;
//------------------------------------------------------------------------------

ALIAS::ALIAS(int Line, const char* Filename): BASE(Line, Filename){
  Type = TYPE::Alias;

  Expression = 0;
}
//------------------------------------------------------------------------------

ALIAS::~ALIAS(){
  if(Expression) delete Expression;
}
//------------------------------------------------------------------------------

void ALIAS::Display(){
  printf("\n%s:%d -- ALIAS (%s):\n", Filename.c_str(), Line, Identifier.c_str());

  Expression->Display();

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------
