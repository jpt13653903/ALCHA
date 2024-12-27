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

#include "FunctionCall.h"
#include "Literal.h"
#include "AST_String.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

FunctionCall::FunctionCall(int line, int filenameIndex):
    AST(line, filenameIndex, Type::FunctionCall)
{
}
//------------------------------------------------------------------------------

FunctionCall::~FunctionCall()
{
    if(parameters) delete parameters;
}
//------------------------------------------------------------------------------

std::string FunctionCall::print() const
{
    string result;

    result = name->print() + "(";

    bool first = true;
    AST* parameter = parameters;
    while(parameter){
        if(!first) result += ", ";
        first = false;
        result += parameter->print();
        parameter = parameter->next;
    }
    result += ")";

    return result;
}
//------------------------------------------------------------------------------

