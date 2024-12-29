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

#include "ClockedConstruct.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

ClockedConstruct::ClockedConstruct(int line, int filenameIndex):
    AST(line, filenameIndex, Type::ClockedConstruct)
{
}
//------------------------------------------------------------------------------

ClockedConstruct::~ClockedConstruct()
{
    if(!attributes) delete attributes;
    if(!parameters) delete parameters;
    if(!body      ) delete body;
}
//------------------------------------------------------------------------------

std::string ClockedConstruct::print(int indent) const
{
    string result;

    for(int n = 0; n < indent; n++) result += "    ";

    switch(construct){
        case Token::Type::RTL: result += "rtl "; break;
        case Token::Type::FSM: result += "fsm "; break;
        default: result += "Unknown clocked construct ";
    }

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
    if(parameters){
        bool first = true;
        result += "(";
        auto param = parameters;
        while(param){
            if(!first) result += ", ";
            first = false;
            result += param->print();
            param   = param->next;
        }
        result += ")";
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

