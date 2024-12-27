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

#include "Concatenate.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Concatenate::Concatenate(int line, int filenameIndex):
    AST(line, filenameIndex, Type::Concatenate)
{
}
//------------------------------------------------------------------------------

Concatenate::~Concatenate()
{
    if(members) delete members;
}
//------------------------------------------------------------------------------

std::string Concatenate::print() const
{
    string result;

    switch(operation){
        case Token::Type::Concatenate:      result = ":("; break;
        case Token::Type::ArrayConcatenate: result = ":["; break;

        default: result = "Unknown concatenate operation "; break;
    }

    bool first = false;
    AST* member = members;
    while(member){
        if(!first) result += ", ";
        first = false;

        result += member->print();

        member = member->next;
    }

    switch(operation){
        case Token::Type::Concatenate:      result = ")"; break;
        case Token::Type::ArrayConcatenate: result = "]"; break;

        default: result = "Unknown concatenate operation "; break;
    }

    return result;
}
//------------------------------------------------------------------------------

