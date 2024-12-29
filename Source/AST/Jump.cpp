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

#include "Jump.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Jump::Jump(int line, int filenameIndex):
    AST(line, filenameIndex, Type::Jump)
{
}
//------------------------------------------------------------------------------

Jump::~Jump()
{
    if(expression) delete expression;
}
//------------------------------------------------------------------------------

std::string Jump::print(int indent) const
{
    string result;

    for(int n = 0; n < indent; n++) result += "    ";

    switch(jumpType){
        case Token::Type::Return:   result += "return";   break;
        case Token::Type::Break:    result += "break";    break;
        case Token::Type::Continue: result += "continue"; break;
        case Token::Type::GoTo:     result += "goto";     break;

        default: result += "Unknown jump type"; break;
    }
    if(expression) result += " " + expression->print();

    return result;
}
//------------------------------------------------------------------------------

