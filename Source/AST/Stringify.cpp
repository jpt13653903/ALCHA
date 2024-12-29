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

#include "Stringify.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Stringify::Stringify(int line, int filenameIndex):
    AST(line, filenameIndex, Type::Stringify)
{
}
//------------------------------------------------------------------------------

Stringify::~Stringify()
{
    if(expression) delete expression;
    if(format    ) delete format;
}
//------------------------------------------------------------------------------

std::string Stringify::print(int indent) const
{
    string result;

    for(int n = 0; n < indent; n++) result += "    ";

    result += "$(";
    if(expression) result += expression->print();
    if(format    ) result += ", " + format->print();
    result += ")";

    return result;
}
//------------------------------------------------------------------------------
