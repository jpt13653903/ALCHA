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

#include "Assignment.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Assignment::Assignment(int line, int filenameIndex):
    AST(line, filenameIndex, Type::Assignment)
{
}
//------------------------------------------------------------------------------

Assignment::~Assignment()
{
    if(target    ) delete target;
    if(expression) delete expression;
}
//------------------------------------------------------------------------------

std::string Assignment::print(int indent) const
{
    string result;

    for(int n = 0; n < indent; n++) result += "    ";

    result += target->print();
    switch(operation){
        case Token::Type::Assign:            result += " = ";   break;
        case Token::Type::RawAssign:         result += " := ";  break;
        case Token::Type::AppendAssign:      result += " ~= ";  break;
        case Token::Type::AddAssign:         result += " += ";  break;
        case Token::Type::SubtractAssign:    result += " -= ";  break;
        case Token::Type::MultiplyAssign:    result += " *= ";  break;
        case Token::Type::DivideAssign:      result += " /= ";  break;
        case Token::Type::ExponentialAssign: result += " **= "; break;
        case Token::Type::ModulusAssign:     result += " %= ";  break;
        case Token::Type::AndAssign:         result += " &= ";  break;
        case Token::Type::OrAssign:          result += " |= ";  break;
        case Token::Type::XorAssign:         result += " ^= ";  break;
        case Token::Type::ShiftLeftAssign:   result += " <<= "; break;
        case Token::Type::ShiftRightAssign:  result += " >>= "; break;

        case Token::Type::Increment: result += "++"; break;
        case Token::Type::Decrement: result += "--"; break;

        default: result += "Unknown assignment type "; break;
    }
    if(expression) result += expression->print();

    return result;
}
//------------------------------------------------------------------------------

