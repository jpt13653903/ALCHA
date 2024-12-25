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

#include "Parser.h"
#include "Utilities.h"
//------------------------------------------------------------------------------

using std::string;
//------------------------------------------------------------------------------

Parser::Parser(){}
//------------------------------------------------------------------------------

Parser::~Parser(){}
//------------------------------------------------------------------------------

void Parser::printError(const char* message)
{
    error = true;
    ::printError(token.line, scanner.getFilename(), message);
    if(token.type != Token::Type::Unknown) printf(
        ANSI_FG_CYAN "  token: "
        ANSI_RESET "%s (%s)\n", token.data.c_str(), token.decodeType()
    );
}
//------------------------------------------------------------------------------

bool Parser::getToken()
{
    if(error) return false;

    if(scanner.getToken(&token))
        return true;

    else if(token.type == Token::Type::EndOfFile)
        return true;

    else
        return false;
}
//------------------------------------------------------------------------------

// bool Parser::statementBlock()
// {
//     if(token.type == Token::Type::OpenCurly){
//         getToken();
//         statements();
//         if(token.type != Token::Type::CloseCurly){
//             printError("} expected");
//             return false;
//         }
//         getToken();
//     }else{
//         if(!statement()){
//             printError("Statement expected");
//             return false;
//         }
//     }
//     return true;
// }
//------------------------------------------------------------------------------

bool Parser::print()
{
    if(token.type != Token::Type::OpenRound){
        printError("( expected");
        return false;
    }
    getToken();

    if(token.type == Token::Type::String){
        printf(ANSI_FG_BRIGHT_BLACK "String: " ANSI_RESET "%s\n",
               token.data.c_str());
    }
    else if(token.type == Token::Type::Literal){
        printf(ANSI_FG_BRIGHT_BLACK "Literal: " ANSI_RESET "%s\n",
               token.value.print().c_str());
    }
    getToken();

    if(token.type != Token::Type::CloseRound){
        printError("( expected");
        return false;
    }
    getToken();

    return true;
}
//------------------------------------------------------------------------------

bool Parser::identifierStatement()
{
    if(token.data == "print"){
        getToken();
        print();

        if(token.type != Token::Type::Semicolon){
            printError("; expected");
            return false;
        }
        getToken();

        return true;
    }else{
        printError("TODO: Label");
        printError("TODO: Definition (using defined type)");
        printError("TODO: FuntionCallStatement");
        printError("TODO: NameSpacePush");
        printError("TODO: Assignment");
        return false;
    }
    return false;
}
//------------------------------------------------------------------------------

bool Parser::functionDef(Token::Type type, string& identifier)
{
    printError("TODO FunctionDef");
    return false;
}
//------------------------------------------------------------------------------

bool Parser::identifierlist(Token::Type type)
{
    bool   isList = false;
    bool   isVariableDef = false;
    string identifier;

    while(token.type > Token::Type::EndOfFile){
        if(token.type == Token::Type::
        identifier = token.data;
        getToken();

        while(token.type == Token::Type::OpenSquare){
            printError("TODO ArrayDefinition");
            return false;
        }

        Symbol* symbol = 0;
        switch(token.type){
            case Token::Type::Assign:
                break;

            case Token::Type::Comma:
                break;

            case Token::Type::Semicolon:
                break;

            case Token::Type::OpenRound:
                if(isList){
                    printError("Unexpected function definition (cannot be part of a list)");
                    return false;
                }
                return functionDef(type, identifier);

            default:
                printError("Unexpected token");
                return false;
        }

        switch
    }
    return false;
}
//------------------------------------------------------------------------------

bool Parser::definition()
{
    isInline = false;
    if(token.type == Token::Type::Inline){
        isInline = true;
        getToken();
    }

    Token::Type type;
    switch(token.type){
        case Token::Type::Pin:
        case Token::Type::Net:
        case Token::Type::Void:
        case Token::Type::Auto:
        case Token::Type::Byte:
        case Token::Type::Char:
        case Token::Type::Num:
        case Token::Type::Func:
        case Token::Type::Identifier:
            type = token.type;
            getToken();
            break;

        default:
            printError("Unexpected token");
            return false;
    }

    if(token.type == Token::Type::OpenRound){
        printError("TODO ParameterList");
        return false;
    }

    if(token.type == Token::Type::OpenAngle){
        printError("TODO AttributeList");
        return false;
    }

    if(token.type == Token::Type::Identifier){
        return identifierlist(type);

    }else if(token.type == Token::Type::Operator){
        printError("TODO OperatorOverload");
        return false;

    }else{
        printError("Identifier expected");
        return false;
    }

    return false;
}
//------------------------------------------------------------------------------

bool Parser::statement()
{
    switch(token.type){
        case Token::Type::WaitUntil:
        case Token::Type::WaitOn:
        case Token::Type::WaitFor:
        case Token::Type::WaitCycles:
            printError("TODO Wait");
            return false;

        case Token::Type::Identifier:
            return identifierStatement();

        case Token::Type::Inline:
        case Token::Type::Pin:
        case Token::Type::Net:
        case Token::Type::Void:
        case Token::Type::Auto:
        case Token::Type::Byte:
        case Token::Type::Char:
        case Token::Type::Num:
        case Token::Type::Func:
            return definition();

        case Token::Type::Class:
            printError("TODO: ClassDefinition");
            return false;

        case Token::Type::Enum:
            printError("TODO: EnumDefinition");
            return false;

        case Token::Type::Alias:
            printError("TODO: Alias");
            return false;

        case Token::Type::Import:
            printError("TODO: Import");
            return false;

        case Token::Type::Struct:
            printError("TODO: Struct");
            return false;

        case Token::Type::Group:
            printError("TODO: Group");
            return false;

        case Token::Type::Public:
        case Token::Type::Private:
        case Token::Type::Protected:
        case Token::Type::Input:
        case Token::Type::Output:
            printError("TODO: AccessDirectionGroup");
            return false;

        case Token::Type::If:
            printError("TODO: IfStatement");
            return false;

        case Token::Type::For:
            printError("TODO: For");
            return false;

        case Token::Type::While:
            printError("TODO: While");
            return false;

        case Token::Type::Loop:
            printError("TODO: Loop");
            return false;

        case Token::Type::Switch:
            printError("TODO: Switch");
            return false;

        case Token::Type::Case:
            printError("TODO: Case");
            return false;

        case Token::Type::Return:
        case Token::Type::Break:
        case Token::Type::Continue:
            printError("TODO: Jump");
            return false;

        case Token::Type::GoTo:
            printError("TODO: GoTo");
            return false;

        case Token::Type::RTL:
            printError("TODO: RTL");
            return false;

        case Token::Type::FSM:
            printError("TODO: FSM");
            return false;

        case Token::Type::HDL:
            printError("TODO: HDL");
            return false;

        case Token::Type::Stimulus:
            printError("TODO: Stimulus");
            return false;

        case Token::Type::Emulate:
            printError("TODO: Emulate");
            return false;

        case Token::Type::OpenCurly:
            printError("TODO: ForkJoin");
            return false;

        case Token::Type::Assert:
            printError("TODO: Assert");
            return false;

        case Token::Type::Semicolon:
            getToken();
            return true;

        case Token::Type::EndOfFile:
            return false;

        default:{
            printError("Unexpected token");
            return false;
        }
    }
    return false;
}
//------------------------------------------------------------------------------

bool Parser::statements()
{
    if(!statement()) return false;
    while(statement());
    return !error;
}
//------------------------------------------------------------------------------

bool Parser::run(const char* filename)
{
    info("Running %s", filename);

    error = !scanner.open(filename);
    if(error) return false;

    getToken();
    statements();

    return !error;
}
//------------------------------------------------------------------------------
