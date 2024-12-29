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

#ifndef AST_ControlStatement_h
#define AST_ControlStatement_h
//------------------------------------------------------------------------------

#include "AST.h"
#include "Token.h"

#include <string>
//------------------------------------------------------------------------------

namespace AST{
    struct ControlStatement: public AST{
        Token::Type statementType = Token::Type::Unknown;
        AST*        expression = 0;
        AST*        body       = 0;
        AST*        elseBody   = 0; // If statements

        ControlStatement(int line, int filenameIndex);
       ~ControlStatement();

        std::string print(int indent = 0) const override;
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
