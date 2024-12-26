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

#include "VariableDef.h"

#include "Symbols/Symbol.h"
#include "Symbols/NameSpace.h"
#include "Symbols/Num.h"
//------------------------------------------------------------------------------

using std::string;
using Symbols::global;
using namespace AST;
//------------------------------------------------------------------------------

VariableDef::VariableDef(int line, int filenameIndex):
    AST(line, filenameIndex, Type::VariableDef)
{
}
//------------------------------------------------------------------------------

VariableDef::~VariableDef()
{
    if(typeIdentifier) delete typeIdentifier;
    if(parameters    ) delete parameters;
    if(attributes    ) delete attributes;
    if(initialiser   ) delete initialiser;
}
//------------------------------------------------------------------------------

bool VariableDef::run()
{
    if(!name.size()){
        printError("Definition with no name");
        return false;
    }
    if(Symbols::global.symbols.contains(name)){
        string s = name + " already exists in the current namespace";
        printError(s.c_str());
        return false;
    }

    switch(type){
        case Token::Type::Pin:
            printError("TODO Pin Definition");
            return false;

        case Token::Type::Net:
            printError("TODO Net Definition");
            return false;

        case Token::Type::Void:
            printError("TODO Void Definition");
            return false;

        case Token::Type::Auto:
            printError("TODO Auto Definition");
            return false;

        case Token::Type::Byte:
            printError("TODO Byte Definition");
            return false;

        case Token::Type::Char:
            printError("TODO Char Definition");
            return false;

        case Token::Type::Num:
            Symbols::global.symbols[name] = new Symbols::Num;
            if(parameters){
                printError("TODO Definition with parameters");
            }
            if(attributes){
                printError("TODO Definition with attributes");
            }
            if(initialiser){
                printError("TODO Definition with initialiser");
            }
            return true;

        case Token::Type::Func:
            printError("TODO Func Definition");
            return false;

        case Token::Type::Identifier:
            printError("TODO TypeIdentifier Definition");
            return false;

        default:
            printError("Unknown type in variable definition");
            return false;
    }
    return true;
}
//------------------------------------------------------------------------------

std::string& VariableDef::print() const
{
    static string result;

    switch(type){
        case Token::Type::Pin:  result = "pin ";  break;
        case Token::Type::Net:  result = "net ";  break;
        case Token::Type::Void: result = "void "; break;
        case Token::Type::Auto: result = "auto "; break;
        case Token::Type::Byte: result = "byte "; break;
        case Token::Type::Char: result = "char "; break;
        case Token::Type::Num:  result = "num ";  break;
        case Token::Type::Func: result = "func "; break;

        case Token::Type::Identifier: result = "TODO: print TypeIdentifier "; break;

        default: result = "Unknown type "; break;
    }
    if(parameters){
        result += "(";
        auto param = parameters;
        while(param){
            result += param->print();
            param   = param->next;
        }
        result += ") ";
    }
    if(attributes){
        result += "<";
        auto attrib = attributes;
        while(attrib){
            result += attrib->print();
            attrib  = attrib->next;
        }
        result += "> ";
    }
    result += name;
    if(initialiser){
        result += " = ";
        result += initialiser->print();
    }

    return result;
}
//------------------------------------------------------------------------------

