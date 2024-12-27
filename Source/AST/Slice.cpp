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

#include "Slice.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Slice::Slice(int line, int filenameIndex):
    AST(line, filenameIndex, Type::Slice)
{
}
//------------------------------------------------------------------------------

Slice::~Slice()
{
    if(array) delete array;
    if(slice) delete slice;
}
//------------------------------------------------------------------------------

std::string Slice::print() const
{
    string result;

    result.clear();

    if(array){
        result += "(";
        result += array->print();
        result += ")[";
    }

    bool first = true;
    AST* element = slice;
    while(element){
        if(!first) result += ", ";
        first = false;

        result += element->print();

        element = element->next;
    }
    result += "]";

    return result;
}
//------------------------------------------------------------------------------

