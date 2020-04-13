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

#ifndef AST_Assignment_OR_Assign_h
#define AST_Assignment_OR_Assign_h
//------------------------------------------------------------------------------

#include "../Assignment.h"
//------------------------------------------------------------------------------

namespace AST{
  struct OR_ASSIGN: public ASSIGNMENT{
    OR_ASSIGN(int Line, std::string Filename);
    OR_ASSIGN(int Line, const char* Filename);
   ~OR_ASSIGN();

    bool RunAST() override;
    bool GetVerilog(std::string& Body) override;

    void Display() override;

    void ValidateMembers() override;

    BASE* Copy() override;
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

