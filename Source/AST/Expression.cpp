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

#include "Expression.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Expression::Expression(int line, int filenameIndex):
    AST(line, filenameIndex, Type::Expression)
{
}
//------------------------------------------------------------------------------

Expression::~Expression()
{
    if(left     ) delete left;
    if(right    ) delete right;
    if(condition) delete condition;
}
//------------------------------------------------------------------------------

std::string Expression::print() const
{
    string result;

    result.clear();

    if(operation == Token::Type::TernaryIf){
        result += "(";
        result += condition->print();
        result += ") ? ";
    }
    if(left){
        result += "(";
        result += left->print();
        result += ")";
    }

    switch(operation){
        case Token::Type::TernaryIf:        result += " : ";  break;
        case Token::Type::Elvis:            result += " ?: "; break;

        case Token::Type::BitOr:            result += " | ";  break;
        case Token::Type::BitNor:           result += " ~| "; break;
        case Token::Type::BitXor:           result += " ^ ";  break;
        case Token::Type::BitXnor:          result += " ~^ "; break;
        case Token::Type::BitAnd:           result += " & ";  break;
        case Token::Type::BitNand:          result += " ~& "; break;

        case Token::Type::Equal:            result += " == "; break;
        case Token::Type::NotEqual:         result += " != "; break;
        case Token::Type::Less:             result += " < ";  break;
        case Token::Type::Greater:          result += " > ";  break;
        case Token::Type::LessEqual:        result += " <= "; break;
        case Token::Type::GreaterEqual:     result += " >= "; break;

        case Token::Type::ShiftLeft:        result += " << "; break;
        case Token::Type::ShiftRight:       result += " >> "; break;

        case Token::Type::Add:              result += " + ";  break;
        case Token::Type::Subtract:         result += " - ";  break;
        case Token::Type::Multiply:         result += " * ";  break;
        case Token::Type::Divide:           result += " / ";  break;
        case Token::Type::Modulus:          result += " % ";  break;
        case Token::Type::Exponential:      result += " ** "; break;
        case Token::Type::Factorial:        result += " ! ";  break;

        case Token::Type::Replicate:        result += " ` ";  break;
        case Token::Type::To:               result += " .. "; break;

        case Token::Type::BitNot:           result += " ~ ";  break;
        case Token::Type::RawBits:          result += " : ";  break;
        case Token::Type::Increment:        result += " ++ "; break;
        case Token::Type::Decrement:        result += " -- "; break;

        case Token::Type::AccessMember:     result += " . ";  break;
        case Token::Type::AccessMemberSafe: result += " ?. "; break;
        case Token::Type::AccessMemberPush: result += " .{ "; break;
        case Token::Type::AccessAttribute:  result += " ' ";  break;

        case Token::Type::CastOp:           result += " @ ";  break;

        default: result += "Unknown expression operation "; break;
    }

    if(right){
        result += "(";
        result += right->print();
        result += ")";
    }
    if(operation == Token::Type::To && stepSize){
        result += ":(";
        result += stepSize->print();
        result += ")";
    }

    return result;
}
//------------------------------------------------------------------------------

