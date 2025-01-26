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

#include "NameSpace.h"
//------------------------------------------------------------------------------

using std::string;
using namespace Objects;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

NameSpace::NameSpace(): Object(Type::NameSpace)
{
}
//------------------------------------------------------------------------------

NameSpace::~NameSpace()
{
    for(auto& [name, symbol]: symbols){
        if(symbol) delete(symbol);
    }
}
//------------------------------------------------------------------------------

std::string& NameSpace::print() const
{
    static string result;

    result  = ANSI_FG_BRIGHT_BLACK;
    result += decodeType();
    result += ": " ANSI_RESET "[\n";
    for(auto& [name, symbol]: symbols){
        result += "    " + symbol->print();
    }
    result += "]";

    return result;
}
//------------------------------------------------------------------------------

