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

#include "CycleDelay.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

CycleDelay::CycleDelay(int line, int filenameIndex):
    AST(line, filenameIndex, Type::CycleDelay)
{
}
//------------------------------------------------------------------------------

CycleDelay::~CycleDelay()
{
    if(left ) delete left;
    if(right) delete right;
    if(delay) delete delay;
}
//------------------------------------------------------------------------------

std::string CycleDelay::print(int indent) const
{
    string result;

    for(int n = 0; n < indent; n++) result += "    ";

    if(left){
        result += "(";
        result += left->print();
        result += ")";
    }

    switch(operation){
        case Token::Type::WaitFor:    result += " #";  break;
        case Token::Type::WaitCycles: result += " ##"; break;

        default: result += "Unknown cycle-delay operation "; break;
    }

    if(delay){
        result += "(";
        result += delay->print();
        result += ") ";
    }
    if(right){
        result += "(";
        result += right->print();
        result += ")";
    }

    return result;
}
//------------------------------------------------------------------------------

