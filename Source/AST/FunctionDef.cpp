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

#include "FunctionDef.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

FunctionDef::FunctionDef(int line, int filenameIndex):
    AST(line, filenameIndex, Type::FunctionDef)
{
}
//------------------------------------------------------------------------------

FunctionDef::~FunctionDef()
{
    if(returnType.parameters) delete returnType.parameters;
    if(attributes           ) delete attributes;
    if(arrayDefs            ) delete arrayDefs;
    if(parameters           ) delete parameters;
    if(body                 ) delete body;
}
//------------------------------------------------------------------------------

std::string FunctionDef::print(int indent) const
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
    result += identifier;

    auto arrayDef = arrayDefs;
    while(arrayDef){
        result += "[" + arrayDef->print() + "]";
        arrayDef = arrayDef->next;
    }

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

