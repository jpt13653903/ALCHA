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

#ifndef Objects_Synthesisable_h
#define Objects_Synthesisable_h
//------------------------------------------------------------------------------

#include "Base.h"
#include "Number.h"
#include "AST/Definition.h"
//------------------------------------------------------------------------------

namespace OBJECTS{
  struct SYNTHESISABLE: public BASE{
    bool   Used; // Actually used in an expression somewhere
    bool   Signed;
    int    Width;
    NUMBER FullScale;

    AST::DEFINITION::DIRECTION Direction;

             SYNTHESISABLE(const char* Name, TYPE Type);
    virtual ~SYNTHESISABLE();

    bool ApplyParameters(AST::BASE* Parameter);

    void Display();
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
