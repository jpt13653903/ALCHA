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

#ifndef Objects_Pin_h
#define Objects_Pin_h
//------------------------------------------------------------------------------

#include "AST/Definition.h"
#include "Synthesisable.h"
#include "Expression.h"
//------------------------------------------------------------------------------

namespace OBJECTS{
  struct PIN: public SYNTHESISABLE{
    EXPRESSION* Driver;
    EXPRESSION* Enabled;

    AST::DEFINITION::DIRECTION Direction;

    PIN(const char* Name);
   ~PIN();

    void Display();
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
