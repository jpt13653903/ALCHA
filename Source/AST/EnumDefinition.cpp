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

#include "EnumDefinition.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

EnumDefinition::EnumDefinition(int line, int filenameIndex):
    AST(line, filenameIndex, Type::EnumDefinition)
{
}
//------------------------------------------------------------------------------

EnumDefinition::~EnumDefinition()
{
    if(!attributes) delete attributes;
    if(!members   ) delete members;
}
//------------------------------------------------------------------------------

EnumDefinition::Member::~Member()
{
    if(!initialiser) delete initialiser;
    if(!next       ) delete next;
}
//------------------------------------------------------------------------------

std::string EnumDefinition::print(int indent) const
{
    string result;

    for(int n = 0; n < indent; n++) result += "    ";

    result += "enum ";

    if(attributes){
        bool first = true;
        result += "<";
        auto attrib = attributes;
        while(attrib){
            if(!first) result += ", ";
            first = false;
            result += attrib->print();
            attrib  = attrib->next;
        }
        result += "> ";
    }
    result += name;

    bool first = true;
    result += " { ";
    auto member = members;
    while(member){
        if(!first) result += ", ";
        first = false;
        result += member->name;
        if(member->initialiser)
            result += " = " + member->initialiser->print();
        member  = member->next;
    }
    result += " }";

    return result;
}
//------------------------------------------------------------------------------

