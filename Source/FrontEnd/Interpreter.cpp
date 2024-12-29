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

#include "Interpreter.h"
#include "Utilities.h"
//------------------------------------------------------------------------------

#include "AST.h"
#include "AST_String.h"
#include "Assignment.h"
#include "Expression.h"
#include "FunctionCall.h"
#include "Identifier.h"
#include "Literal.h"
#include "VariableDef.h"
//------------------------------------------------------------------------------

#include "Symbols/Num.h"
//------------------------------------------------------------------------------

using std::string;
//------------------------------------------------------------------------------

Interpreter::Interpreter()
{
}
//------------------------------------------------------------------------------

Interpreter::~Interpreter()
{
}
//------------------------------------------------------------------------------

void Interpreter::printError(const char* message)
{
    error = true;
    ::printError(ast->line, AST::filenameBuffer[ast->filenameIndex], message);
}
//------------------------------------------------------------------------------

bool Interpreter::variableDef()
{
    auto ast = (AST::VariableDef*)this->ast;
    auto def = ast->defList;
    while(def){
        if(!def->name.size()){
            printError("Definition with no name");
            return false;
        }
        if(global.symbols.contains(def->name)){
            string s = def->name + " already exists in the current namespace";
            printError(s.c_str());
            return false;
        }

        switch(ast->defType){
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
                global.symbols[def->name] = new Symbols::Num;
                if(ast->parameters){
                    printError("TODO Definition with parameters");
                }
                if(ast->attributes){
                    printError("TODO Definition with attributes");
                }
                if(def->initialiser){
                    printError("TODO Definition with initialiser");
                }
                break;

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
        def = def->next;
    }
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::functionDef()
{
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::operatorOverload()
{
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::assignment()
{
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::functionCall()
{
    auto ast = (AST::FunctionCall*)this->ast;
    if(ast->name->type == AST::AST::Type::Identifier &&
       ((AST::Identifier*)ast->name)->name == "print"){
        if(ast->parameters){
            if(ast->parameters->type == AST::AST::Type::String){
                printf(ANSI_FG_BRIGHT_BLACK "String: " ANSI_RESET "%s\n",
                       ((AST::String*)ast->parameters)->data.c_str());
            }
            else if(ast->parameters->type == AST::AST::Type::Literal){
                printf(ANSI_FG_BRIGHT_BLACK "Literal: " ANSI_RESET "%s\n",
                       ((AST::Literal*)ast->parameters)->value.print().c_str());
            }
        }else{
            printf("\n");
        }
    }
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::run(AST::AST* ast)
{
    this->ast = ast;

    while(this->ast){
        switch(this->ast->type){
            case AST::AST::Type::VariableDef:
                if(!variableDef()) return false;
                break;

            case AST::AST::Type::FunctionDef:
                if(!functionDef()) return false;
                break;

            case AST::AST::Type::OperatorOverload:
                if(!operatorOverload()) return false;
                break;

            case AST::AST::Type::Assignment:
                if(!assignment()) return false;
                break;

            case AST::AST::Type::FunctionCall:
                if(!functionCall()) return false;
                break;

            default:
                printError("Unknown AST type");
                return false;
        }
        this->ast = this->ast->next;
    }

    if(error) return false;
    return true;
}
//------------------------------------------------------------------------------
