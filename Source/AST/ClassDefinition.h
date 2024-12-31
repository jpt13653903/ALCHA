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

#ifndef AST_ClassDefinition_h
#define AST_ClassDefinition_h
//------------------------------------------------------------------------------

#include "AST.h"

#include <string>
//------------------------------------------------------------------------------

namespace AST{
    struct ClassDefinition: public AST{
        AST*        attributes = 0;
        std::string identifier;
        AST*        parameters = 0;
        AST*        body       = 0;

        struct Parent{
            AST*    typeIdentifier = 0;
            AST*    parameters     = 0;
            Parent* next           = 0;

            ~Parent();
        }* parents = 0;

        ClassDefinition(int line, int filenameIndex);
       ~ClassDefinition();

        std::string print(int indent = 0) const override;
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

