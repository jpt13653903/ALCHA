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
//------------------------------------------------------------------------------

using std::string;
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
    if(defList       ) delete defList;
}
//------------------------------------------------------------------------------

VariableDef::Definition::~Definition()
{
    if(arrayDefs  ) delete arrayDefs;
    if(initialiser) delete initialiser;
    if(next       ) delete next;
}
//------------------------------------------------------------------------------

std::string VariableDef::print(int indent) const
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

        default: result += "Unknown defType "; break;
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

    bool first = true;
    auto def = defList;
    while(def){
        if(!first) result += ", ";
        first = false;

        result += def->name;

        auto arrayDef = def->arrayDefs;
        while(arrayDef){
            result += "[" + arrayDef->print() + "]";
            arrayDef = arrayDef->next;
        }
        if(def->initialiser){
            switch(def->initType){
                case Token::Type::Assign:    result += " = ";  break;
                case Token::Type::RawAssign: result += " := "; break;

                default: result += " Invalid initialiser type "; break;
            }
            result += def->initialiser->print();
        }
        def = def->next;
    }

    return result;
}
//------------------------------------------------------------------------------

