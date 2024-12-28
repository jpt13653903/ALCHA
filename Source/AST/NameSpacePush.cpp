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

#include "NameSpacePush.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

NameSpacePush::NameSpacePush(int line, int filenameIndex):
    AST(line, filenameIndex, Type::NameSpacePush)
{
}
//------------------------------------------------------------------------------

NameSpacePush::~NameSpacePush()
{
    if(nameSpace) delete nameSpace;
    if(body     ) delete body;
}
//------------------------------------------------------------------------------

std::string NameSpacePush::print(int indent) const
{
    string result;

    for(int n = 0; n < indent; n++) result += "    ";

    result += nameSpace->print();
    result += ".{\n";
    auto statement = body;
    while(statement){
        result += statement->print(indent+1);
        result += "\n";
        statement = statement->next;
    }
    result += "}";

    return result;
}
//------------------------------------------------------------------------------

