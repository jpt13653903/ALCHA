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

#include "CoverGroup.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

CoverGroup::CoverGroup(int line, int filenameIndex):
    AST(line, filenameIndex, Type::CoverGroup)
{
}
//------------------------------------------------------------------------------

CoverGroup::~CoverGroup()
{
    if(!parameters) delete parameters;
    if(!bins      ) delete bins;
}
//------------------------------------------------------------------------------

CoverGroup::Bin::~Bin()
{
    if(!identifier) delete identifier;
    if(!parameters) delete parameters;
    if(!next      ) delete next;
}
//------------------------------------------------------------------------------

std::string CoverGroup::print(int indent) const
{
    string result;

    for(int n = 0; n < indent; n++) result += "    ";

    result += "covergroup ";

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
        result += ") ";
    }
    result += identifier;

    result += "{\n";
    auto bin = bins;
    while(bin){
        result += bin->identifier->print(indent+1);
        bool first = true;
        result += "(";
        auto param = parameters;
        while(param){
            if(!first) result += ", ";
            first = false;
            result += param->print();
            param   = param->next;
        }
        result += ")\n";
        bin = bin->next;
    }
    for(int n = 0; n < indent; n++) result += "    ";
    result += "}";

    return result;
}
//------------------------------------------------------------------------------

