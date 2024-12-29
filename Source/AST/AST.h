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

#ifndef AST_AST_h
#define AST_AST_h
//------------------------------------------------------------------------------

#include "General.h"
#include "Type.h"

#include <string>
#include <vector>
//------------------------------------------------------------------------------

namespace AST{
    extern std::vector<std::string> filenameBuffer;

    struct AST{
        Type type;

        int line          = 0;
        int filenameIndex = 0;

        AST* next = 0;

        AST(int line, int filenameIndex, Type type);
        virtual ~AST();

        const char* decodeType() const;
        virtual std::string print(int indent = 0) const = 0;
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

