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

#ifndef AST_Expression_Less_Equal_h
#define AST_Expression_Less_Equal_h
//------------------------------------------------------------------------------

#include "../Expression.h"
//------------------------------------------------------------------------------

namespace AST{
  struct LESS_EQUAL: public EXPRESSION{
    LESS_EQUAL(int Line, const std::string& Filename);
    LESS_EQUAL(int Line, const char*        Filename);
   ~LESS_EQUAL();

    BASE* Copy() override;

    bool GetVerilog(std::string& Body) override;
    EXPRESSION* Evaluate() override;

    int     GetWidth    () override;
    NUMBER& GetFullScale() override;
    bool    GetSigned   () override;

    bool HasCircularReference(NETLIST::BASE* Object) override;
    void PopulateUsed() override;

    void Display() override;

    void ValidateMembers() override;
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

