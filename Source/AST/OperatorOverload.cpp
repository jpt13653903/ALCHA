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

#include "OperatorOverload.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

OperatorOverload::OperatorOverload(int line, int filenameIndex):
    AST(line, filenameIndex, Type::OperatorOverload)
{
}
//------------------------------------------------------------------------------

OperatorOverload::~OperatorOverload()
{
    if(returnType.parameters) delete returnType.parameters;
    if(attributes           ) delete attributes;
    if(parameters           ) delete parameters;
    if(body                 ) delete body;
}
//------------------------------------------------------------------------------


const char* OperatorOverload::printOperation() const
{
    switch(operation){
        case Token::Type::Elvis:                  return "?:";  break;

        case Token::Type::BitOr:                  return "|";   break;
        case Token::Type::BitNor:                 return "~|";  break;
        case Token::Type::BitXor:                 return "^";   break;
        case Token::Type::BitXnor:                return "~^";  break;
        case Token::Type::BitAnd:                 return "&";   break;
        case Token::Type::BitNand:                return "~&";  break;

        case Token::Type::Equal:                  return "==";  break;
        case Token::Type::NotEqual:               return "!=";  break;
        case Token::Type::Less:                   return "<";   break;
        case Token::Type::Greater:                return ">";   break;
        case Token::Type::LessEqual:              return "<=";  break;
        case Token::Type::GreaterEqual:           return ">=";  break;

        case Token::Type::ShiftLeft:              return "<<";  break;
        case Token::Type::ShiftRight:             return ">>";  break;

        case Token::Type::Add:                    return "+";   break;
        case Token::Type::Subtract:               return "-";   break;
        case Token::Type::Multiply:               return "*";   break;
        case Token::Type::Divide:                 return "/";   break;
        case Token::Type::Modulus:                return "%";   break;
        case Token::Type::Exponential:            return "**";  break;
        case Token::Type::Factorial:              return "!";   break;

        case Token::Type::Replicate:              return "`";   break;
        case Token::Type::Stringify:              return "$";   break;
        case Token::Type::To:                     return "..";  break;

        case Token::Type::BitNot:                 return "~";   break;
        case Token::Type::RawBits:                return ":";   break;
        case Token::Type::Increment:              return "++";  break;
        case Token::Type::Decrement:              return "--";  break;

        case Token::Type::CastOp:                 return "@";   break;

        case Token::Type::Assign:                 return "=";   break;
        case Token::Type::RawAssign:              return ":=";  break;
        case Token::Type::AppendAssign:           return "~=";  break;
        case Token::Type::AddAssign:              return "+=";  break;
        case Token::Type::SubtractAssign:         return "-=";  break;
        case Token::Type::MultiplyAssign:         return "*=";  break;
        case Token::Type::DivideAssign:           return "/=";  break;
        case Token::Type::ExponentialAssign:      return "**="; break;
        case Token::Type::ModulusAssign:          return "%=";  break;
        case Token::Type::AndAssign:              return "&=";  break;
        case Token::Type::OrAssign:               return "|=";  break;
        case Token::Type::XorAssign:              return "^=";  break;
        case Token::Type::ShiftLeftAssign:        return "<<="; break;
        case Token::Type::ShiftRightAssign:       return ">>="; break;

        case Token::Type::SequenceConsecutive:    return "[*";  break;
        case Token::Type::SequenceGoTo:           return "[->"; break;
        case Token::Type::SequenceNonConsecutive: return "[=";  break;
        case Token::Type::AssertImplies:          return "|->"; break;
        case Token::Type::AssertImpliesNext:      return "|=>"; break;
        case Token::Type::Or:                     return "||";  break;
        case Token::Type::And:                    return "&&";  break;
        case Token::Type::Intersect:              return "&&&"; break;

        default: return "Unknown operator"; break;
    }
}
//------------------------------------------------------------------------------

std::string OperatorOverload::print(int indent) const
{
    string result;

    for(int n = 0; n < indent; n++) result += "    ";

    if(isInline) result += "inline ";

    switch(returnType.type){
        case Token::Type::Pin:  result += "pin ";  break;
        case Token::Type::Net:  result += "net ";  break;
        case Token::Type::Void: result += "void "; break;
        case Token::Type::Auto: result += "auto "; break;
        case Token::Type::Byte: result += "byte "; break;
        case Token::Type::Char: result += "char "; break;
        case Token::Type::Num:  result += "num ";  break;
        case Token::Type::Func: result += "func "; break;

        case Token::Type::Identifier:
            if(returnType.typeIdentifier)
                result += returnType.typeIdentifier->print() + " ";
            else
                result += "Invalid typeIdentifier ";
            break;

        default:
            result += "Unknown defType ";
            break;
    }
    if(returnType.parameters){
        bool first = true;
        result += "(";
        auto param = returnType.parameters;
        while(param){
            if(!first) result += ", ";
            first = false;
            result += param->print();
            param   = param->next;
        }
        result += ") ";
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
    result += "operator";
    result += printOperation();
    result += " ";

    bool first = true;
    result += "(";
    auto param = parameters;
    while(param){
        if(!first) result += ", ";
        first = false;
        result += param->print();
        param = param->next;
    }
    result += "){\n";
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

