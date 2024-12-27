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

#ifndef AST_String_h
#define AST_String_h
//------------------------------------------------------------------------------

#include "AST.h"
#include <string>
//------------------------------------------------------------------------------

namespace AST{
    struct String: public AST{
        std::string data;

        String(int line, int filenameIndex);
       ~String();

        std::string print() const override;
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

