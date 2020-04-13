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

#include <vector>
#include "Expression.h"
//------------------------------------------------------------------------------

namespace AST{
  struct SWITCH: public BASE{
    struct CASE{
      std::vector<EXPRESSION*> Expressions;
      BASE* Statements;

      CASE* Next; // The next case (this can be turned into a BST later...)

      CASE();
      CASE(const CASE& Case);
     ~CASE();
    };
    CASE      * Cases;
    BASE      * Default;
    EXPRESSION* Expression;

    SWITCH(int Line, std::string& Filename);
    SWITCH(int Line, const char*  Filename);
   ~SWITCH();

    BASE* Copy() override;

    bool RunAST() override;
    bool GetVerilog(std::string& Body) override;

    void Display() override;

    void ValidateMembers() override;
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

