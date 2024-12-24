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

Parser::Parser()
{
    error = false;
}
//------------------------------------------------------------------------------

Parser::~Parser(){}
//------------------------------------------------------------------------------

void Parser::printError(const char* message)
{
    error = true;
    ::printError(token.line, scanner.getFilename(), message);
    if(token.type != Token::Type::Unknown) printf(
        ANSI_FG_CYAN "  token: "
        ANSI_RESET "%s\n", token.data.c_str()
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
               token.value.getString().c_str());
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

bool Parser::statement()
{
    if(token.type == Token::Type::Identifier){
        if(token.data == "print"){
            getToken();
            print();

            if(token.type != Token::Type::Semicolon){
                printError("; expected");
                return false;
            }
            getToken();

            return true;
        }
    }
    // wait();

    // if(label                ()) return true;
    // if(definition           ()) return true;
    // if(classDefinition      ()) return true;
    // if(enumDefinition       ()) return true;

    // if(alias                ()) return true;
    // if(import               ()) return true;
    // if(struct               ()) return true;
    // if(group                ()) return true;
    // if(accessDirectionGroup ()) return true;

    // if(ifStatement          ()) return true;
    // if(for                  ()) return true;
    // if(while                ()) return true;
    // if(loop                 ()) return true;
    // if(switch               ()) return true;
    // if(case                 ()) return true;
    // if(jump                 ()) return true;
    // if(goTo                 ()) return true;

    // if(functionCallStatement()) return true;
    // if(namespacePush        ()) return true;
    // if(assignment           ()) return true;

    // if(rtl                  ()) return true;
    // if(fsm                  ()) return true;
    // if(hdl                  ()) return true;

    // if(stimulus             ()) return true;
    // if(emulate              ()) return true;
    // if(forkJoin             ()) return true;
    // if(assert               ()) return true;

    // if(token->type == Token::Type::Semicolon) return true;

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
