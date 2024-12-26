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

#include "AST/AST_String.h"
#include "AST/VariableDef.h"
#include "AST/FunctionCall.h"
#include "AST/Literal.h"
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

// AST::AST* Parser::statementBlock()
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

AST::AST* Parser::parameter()
{
    if(token.type == Token::Type::Identifier){
        string identifier = token.data;
        getToken();
        if(token.type == Token::Type::Assign ||
           token.type == Token::Type::RawAssign){
            printError("TODO: Named parameters");
            return 0;
        }else{
            printError("TODO: Expressions");
            return 0;
        }
    }else if(token.type == Token::Type::Literal){
        auto result = new AST::Literal(token.line, astFilenameIndex);
        result->value = token.value;
        getToken();
        return result;

    }else if(token.type == Token::Type::String){
        auto result = new AST::String(token.line, astFilenameIndex);
        result->data = token.data;
        getToken();
        return result;
    }
    return 0;
}
//------------------------------------------------------------------------------

AST::AST* Parser::parameterList()
{
    getToken();

    AST::AST* result  = 0;
    AST::AST* last    = 0;
    AST::AST* current = 0;

    current = parameter();
    result = last = current;

    if(!current) return 0;

    while(token.type == Token::Type::Comma){
        current = parameter();
        if(!current){
            printError("Parameter expected");
            delete result;
            return 0;
        }
        last->next = current;
        last = current;
    }
    if(token.type != Token::Type::CloseRound){
        printError(") expected");
        delete result;
        return 0;
    }
    getToken();

    return result;
}
//------------------------------------------------------------------------------

AST::AST* Parser::identifierStatement()
{
    string identifier = token.data;
    getToken();

    switch(token.type){
        case Token::Type::Colon:
            printError("TODO: Label");
            return 0;

        case Token::Type::OpenRound:{
            auto parameters = parameterList();
            if(token.type == Token::Type::Semicolon){
                auto result = new AST::FunctionCall(token.line, astFilenameIndex);
                result->name = identifier;
                result->parameters = parameters;
                getToken();
                return result;
            }
            if(token.type == Token::Type::OpenAngle){
                printError("TODO: Definition (using defined type)");
                if(parameters) delete parameters;
                return 0;
            }
            if(token.type == Token::Type::Identifier){
                printError("TODO: Definition (using defined type)");
                if(parameters) delete parameters;
                return 0;
            }else{
                printError("Identifier expected");
                if(parameters) delete parameters;
                return 0;
            }
        }

        case Token::Type::OpenAngle:
        case Token::Type::Identifier:
            printError("TODO: Definition (using defined type)");
            return 0;

        case Token::Type::AccessMemberPush:
            printError("TODO: NameSpacePush");
            return 0;

        case Token::Type::Assign:
        case Token::Type::RawAssign:
        case Token::Type::AppendAssign:
        case Token::Type::AddAssign:
        case Token::Type::SubtractAssign:
        case Token::Type::MultiplyAssign:
        case Token::Type::DivideAssign:
        case Token::Type::ModulusAssign:
        case Token::Type::XorAssign:
        case Token::Type::AndAssign:
        case Token::Type::OrAssign:
        case Token::Type::ExponentialAssign:
        case Token::Type::ShiftLeftAssign:
        case Token::Type::ShiftRightAssign:
            printError("TODO: Assign");
            return 0;

        default:
            printError("Unexpected token");
            return 0;
    }
    return 0;
}
//------------------------------------------------------------------------------

AST::AST* Parser::functionDef(Token::Type type, string& identifier)
{
    printError("TODO FunctionDef");
    return 0;
}
//------------------------------------------------------------------------------

AST::AST* Parser::identifierlist(Token::Type type)
{
    bool   isList = false;
    bool   isVariableDef = false; // Can also be a FunctionDef
    string identifier;

    AST::AST* result  = 0;
    AST::AST* last    = 0;

    while(token.type > Token::Type::EndOfFile){
        if(token.type == Token::Type::Identifier){
            identifier = token.data;
            getToken();
        }else{
            printError("Identifier expected");
            return 0;
        }

        while(token.type == Token::Type::OpenSquare){
            printError("TODO ArrayDefinition");
            return 0;
        }

        switch(token.type){
            case Token::Type::Assign:
                isVariableDef = true;
                printError("TODO Initialiser");
                if(result) delete result;
                return 0;

            case Token::Type::Comma:{
                isList = true;
                isVariableDef = false;
                auto current = new AST::VariableDef(token.line, astFilenameIndex);
                current->type = type;
                current->name = identifier;
                if(last) last->next = current;
                else     result = last = current;
                last = current;
                getToken();
                break;
            }

            case Token::Type::Semicolon:{
                isList = true;
                isVariableDef = false;
                auto current = new AST::VariableDef(token.line, astFilenameIndex);
                current->type = type;
                current->name = identifier;
                if(last) last->next = current;
                else     result = last = current;
                last = current;
                getToken();
                return result;
            }

            case Token::Type::OpenRound:{
                if(isList){
                    printError("Unexpected function definition (cannot be part of a list)");
                    if(result) delete result;
                    return 0;
                }
                if(isVariableDef){
                    printError("Unexpected initialiser before function definition");
                    if(result) delete result;
                    return 0;
                }
                auto current = functionDef(type, identifier);
                if(last) last->next = current;
                else     result = last = current;
                last = current;
                return result;
            }

            default:
                printError("Unexpected token");
                if(result) delete result;
                return 0;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------

AST::AST* Parser::definition()
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
            type = token.type;
            getToken();
            break;

        case Token::Type::Identifier:
            printError("TODO TypeIdentifier-style definitions");
            return 0;

        default:
            printError("Unexpected token");
            return 0;
    }

    if(token.type == Token::Type::OpenRound){
        printError("TODO ParameterList");
        return 0;
    }

    if(token.type == Token::Type::OpenAngle){
        printError("TODO AttributeList");
        return 0;
    }

    if(token.type == Token::Type::Identifier){
        return identifierlist(type);

    }else if(token.type == Token::Type::Operator){
        printError("TODO OperatorOverload");
        return 0;

    }else{
        printError("Identifier expected");
        return 0;
    }

    return 0;
}
//------------------------------------------------------------------------------

AST::AST* Parser::statement()
{
    switch(token.type){
        case Token::Type::WaitUntil:
        case Token::Type::WaitOn:
        case Token::Type::WaitFor:
        case Token::Type::WaitCycles:
            printError("TODO Wait");
            return 0;

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
            return 0;

        case Token::Type::Enum:
            printError("TODO: EnumDefinition");
            return 0;

        case Token::Type::Alias:
            printError("TODO: Alias");
            return 0;

        case Token::Type::Import:
            printError("TODO: Import");
            return 0;

        case Token::Type::Struct:
            printError("TODO: Struct");
            return 0;

        case Token::Type::Group:
            printError("TODO: Group");
            return 0;

        case Token::Type::Public:
        case Token::Type::Private:
        case Token::Type::Protected:
        case Token::Type::Input:
        case Token::Type::Output:
            printError("TODO: AccessDirectionGroup");
            return 0;

        case Token::Type::If:
            printError("TODO: IfStatement");
            return 0;

        case Token::Type::For:
            printError("TODO: For");
            return 0;

        case Token::Type::While:
            printError("TODO: While");
            return 0;

        case Token::Type::Loop:
            printError("TODO: Loop");
            return 0;

        case Token::Type::Switch:
            printError("TODO: Switch");
            return 0;

        case Token::Type::Case:
            printError("TODO: Case");
            return 0;

        case Token::Type::Return:
        case Token::Type::Break:
        case Token::Type::Continue:
            printError("TODO: Jump");
            return 0;

        case Token::Type::GoTo:
            printError("TODO: GoTo");
            return 0;

        case Token::Type::RTL:
            printError("TODO: RTL");
            return 0;

        case Token::Type::FSM:
            printError("TODO: FSM");
            return 0;

        case Token::Type::HDL:
            printError("TODO: HDL");
            return 0;

        case Token::Type::Stimulus:
            printError("TODO: Stimulus");
            return 0;

        case Token::Type::Emulate:
            printError("TODO: Emulate");
            return 0;

        case Token::Type::OpenCurly:
            printError("TODO: ForkJoin");
            return 0;

        case Token::Type::Assert:
            printError("TODO: Assert");
            return 0;

        case Token::Type::Semicolon:
            printError("TODO: Fence");
            return 0;

        case Token::Type::EndOfFile:
            return 0;

        default:{
            printError("Unexpected token");
            return 0;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------

AST::AST* Parser::statements()
{
    AST::AST* result  = 0;
    AST::AST* last    = 0;
    AST::AST* current = 0;

    current = statement();
    if(!current) return 0;
    result = last = current;

    current = statement();
    while(current){
        last->next = current;
        last = current;
        current = statement();
    }
    return result;
}
//------------------------------------------------------------------------------

AST::AST* Parser::parse(const char* filename)
{
    info("Running %s", filename);

    error = !scanner.open(filename);
    if(error) return 0;

    astFilenameIndex = AST::filenameBuffer.size();
    AST::filenameBuffer.push_back(filename);

    getToken();
    AST::AST* result = statements();

    if(error){
        if(result) delete result;
        return 0;
    }
    return result;
}
//------------------------------------------------------------------------------
