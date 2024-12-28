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

#include "AccessDirectionGroup.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

AccessDirectionGroup::AccessDirectionGroup(int line, int filenameIndex):
    AST(line, filenameIndex, Type::AccessDirectionGroup)
{
}
//------------------------------------------------------------------------------

AccessDirectionGroup::~AccessDirectionGroup()
{
    if(!body) delete body;
}
//------------------------------------------------------------------------------

std::string AccessDirectionGroup::print(int indent) const
{
    string result;

    for(int n = 0; n < indent; n++) result += "    ";

    switch(access){
        case Token::Type::Public:    result += "public ";    break;
        case Token::Type::Private:   result += "private ";   break;
        case Token::Type::Protected: result += "protected "; break;
        default: break;
    }
    switch(direction){
        case Token::Type::Input:  result += "input ";  break;
        case Token::Type::Output: result += "output "; break;
        default: break;
    }
    result += "{\n";
    auto statement = body;
    while(statement){
        result += statement->print(indent+1);
        result += "\n";
        statement = statement->next;
    }
    for(int n = 0; n < indent; n++) result += "    ";
    result += "}";

    return result;
}
//------------------------------------------------------------------------------

