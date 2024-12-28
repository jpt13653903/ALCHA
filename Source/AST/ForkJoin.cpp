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

#include "ForkJoin.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

ForkJoin::ForkJoin(int line, int filenameIndex):
    AST(line, filenameIndex, Type::ForkJoin)
{
}
//------------------------------------------------------------------------------

ForkJoin::~ForkJoin()
{
    if(!left ) delete left;
    if(!right) delete right;
}
//------------------------------------------------------------------------------

std::string ForkJoin::print(int indent) const
{
    string result;

    for(int n = 0; n < indent; n++) result += "    ";

    result += "{\n";
    auto statement = left;
    while(statement){
        result += statement->print(indent+1);
        result += "\n";
        statement = statement->next;
    }
    for(int n = 0; n < indent; n++) result += "    ";
    result += "}";
    switch(operation){
        case Token::Type::Or:  result += " || ";  break;
        case Token::Type::And: result += " && "; break;
        default: break;
    }
    result += "{\n";
    statement = right;
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

