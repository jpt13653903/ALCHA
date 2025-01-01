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

#include "CoverBins.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

CoverBins::CoverBins(int line, int filenameIndex):
    AST(line, filenameIndex, Type::CoverBins)
{
}
//------------------------------------------------------------------------------

CoverBins::~CoverBins()
{
    if(!parameters) delete parameters;
    if(!bins      ) delete bins;
}
//------------------------------------------------------------------------------

CoverBins::Bin::~Bin()
{
    if(!sequence) delete sequence;
    if(!next    ) delete next;
}
//------------------------------------------------------------------------------

std::string CoverBins::print(int indent) const
{
    string result;

    for(int n = 0; n < indent; n++) result += "    ";

    result += "coverbins " + identifier;

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

    result += "{\n";
    auto bin = bins;
    while(bin){
        for(int n = -1; n < indent; n++) result += "    ";
        result += bin->identifier + " = " + bin->sequence->print() + "\n";
        bin = bin->next;
    }
    for(int n = 0; n < indent; n++) result += "    ";
    result += "}";

    return result;
}
//------------------------------------------------------------------------------

