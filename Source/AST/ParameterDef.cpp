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

#include "ParameterDef.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

ParameterDef::ParameterDef(int line, int filenameIndex):
    AST(line, filenameIndex, Type::ParameterDef)
{
}
//------------------------------------------------------------------------------

ParameterDef::~ParameterDef()
{
    if(!typeIdentifier) delete typeIdentifier;
    if(!parameters    ) delete parameters;
    if(!initialiser   ) delete initialiser;
}
//------------------------------------------------------------------------------

std::string ParameterDef::print(int indent) const
{
    string result;

    for(int n = 0; n < indent; n++) result += "    ";

    switch(defType){
        case Token::Type::Pin:  result += "pin ";  break;
        case Token::Type::Net:  result += "net ";  break;
        case Token::Type::Void: result += "void "; break;
        case Token::Type::Auto: result += "auto "; break;
        case Token::Type::Byte: result += "byte "; break;
        case Token::Type::Char: result += "char "; break;
        case Token::Type::Num:  result += "num ";  break;
        case Token::Type::Func: result += "func "; break;

        case Token::Type::Identifier:
            if(typeIdentifier) result += typeIdentifier->print() + " ";
            else               result += "Invalid typeIdentifier ";
            break;

        default:
            break;
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
        result += ") ";
    }
    result += identifier;

    for(int n = 0; n < arrayDimentions; n++){
        result += "[]";
    }
    if(initialiser){
        result += " = ";
        result += initialiser->print();
    }
    return result;
}
//------------------------------------------------------------------------------

