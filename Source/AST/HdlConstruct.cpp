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

#include "HdlConstruct.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

HdlConstruct::HdlConstruct(int line, int filenameIndex):
    AST(line, filenameIndex, Type::HdlConstruct)
{
}
//------------------------------------------------------------------------------

HdlConstruct::~HdlConstruct()
{
    if(!attributes) delete attributes;
    if(!parameters) delete parameters;
    if(!portDefs  ) delete portDefs;
}
//------------------------------------------------------------------------------

HdlConstruct::Parameter::~Parameter()
{
    if(!expression) delete expression;
    if(!next      ) delete next;
}
//------------------------------------------------------------------------------

std::string HdlConstruct::print(int indent) const
{
    string result;

    for(int n = 0; n < indent; n++) result += "    ";

    result += "hdl ";

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
    result += identifier;

    result += "(\n";
    auto param = parameters;
    while(param){
        for(int n = -1; n < indent; n++) result += "    ";
        result += param->name;
        switch(param->operation){
            case Token::Type::Assign:    result += " = ";  break;
            case Token::Type::RawAssign: result += " := "; break;

            default: result += " Unsupported assignment operation"; break;
        }
        result += param->expression->print();
        result += "\n";
        param   = param->next;
    }

    for(int n = 0; n < indent; n++) result += "    ";
    result += "){\n";

    auto statement = portDefs;
    while(statement){
        result   += statement->print(indent+1);
        result   += "\n";
        statement = statement->next;
    }
    for(int n = 0; n < indent; n++) result += "    ";
    result += "}";

    return result;
}
//------------------------------------------------------------------------------

