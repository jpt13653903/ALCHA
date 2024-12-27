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

#include "AST/Assignment.h"
#include "AST/AST_String.h"
#include "AST/Concatenate.h"
#include "AST/Expression.h"
#include "AST/FunctionCall.h"
#include "AST/Identifier.h"
#include "AST/Literal.h"
#include "AST/Slice.h"
#include "AST/Stringify.h"
#include "AST/VariableDef.h"
//------------------------------------------------------------------------------

using std::string;
//------------------------------------------------------------------------------

Parser::Parser()
{
}
//------------------------------------------------------------------------------

Parser::~Parser()
{
}
//------------------------------------------------------------------------------

void Parser::printError(const char* message)
{
    if(error) return;
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

// Expression = BitwiseOR ( [ "?" Cast ":" Cast ] | [ "?:" Cast ] );
AST::AST* Parser::expression()
{
    auto condition = bitwiseOr();
    if(!condition) return 0;

    int line = token.line;
    Token::Type operation = token.type;
    switch(token.type){
        case Token::Type::TernaryIf:{
            getToken();
            auto truePart = castExpression();
            if(!truePart){
                printError("Invalid true part in ternary expression");
                delete condition;
                return 0;
            }
            if(token.type != Token::Type::TernaryElse){
                printError(": expected");
                delete condition;
                return 0;
            }
            getToken();
            auto falsePart = castExpression();
            if(!falsePart){
                printError("Invalid false part in ternary expression");
                delete condition;
                delete truePart;
                return 0;
            }
            auto result = new AST::Expression(line, astFilenameIndex);
            result->operation = operation;
            result->condition = condition;
            result->left      = truePart;
            result->right     = falsePart;
            return result;
        }
        case Token::Type::Elvis:{
            getToken();
            auto falsePart = castExpression();
            if(!falsePart){
                printError("Invalid false part in elvis expression");
                delete condition;
                return 0;
            }
            auto result = new AST::Expression(line, astFilenameIndex);
            result->operation = operation;
            result->left      = condition;
            result->right     = falsePart;
            return result;
        }
        default:
            return condition;
    }
    return 0;
}
//------------------------------------------------------------------------------

// BitwiseOR = BitwiseXOR { ( "|" | "~|" ) BitwiseXOR };
AST::AST* Parser::bitwiseOr()
{
    auto left = bitwiseXor();
    if(!left) return 0;

    while(token.type > Token::Type::EndOfFile){
        int line = token.line;
        Token::Type operation = token.type;
        switch(token.type){
            case Token::Type::BitOr:
            case Token::Type::BitNor:{
                getToken();
                auto right = bitwiseXor();
                if(!right){
                    printError("Invalid OR expression");
                    delete left;
                    return 0;
                }
                auto result = new AST::Expression(line, astFilenameIndex);
                result->operation = operation;
                result->left      = left;
                result->right     = right;
                left = result;
                break;
            }
            default:
                return left;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------

// BitwiseXOR = BitwiseAND { ( "^" | "~^" ) BitwiseAND };
AST::AST* Parser::bitwiseXor()
{
    auto left = bitwiseAnd();
    if(!left) return 0;

    while(token.type > Token::Type::EndOfFile){
        int line = token.line;
        Token::Type operation = token.type;
        switch(token.type){
            case Token::Type::BitXor:
            case Token::Type::BitXnor:{
                getToken();
                auto right = bitwiseAnd();
                if(!right){
                    printError("Invalid XOR expression");
                    delete left;
                    return 0;
                }
                auto result = new AST::Expression(line, astFilenameIndex);
                result->operation = operation;
                result->left      = left;
                result->right     = right;
                left = result;
                break;
            }
            default:
                return left;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------

// BitwiseAND = Equality { ( "&" | "~&" ) Equality };
AST::AST* Parser::bitwiseAnd()
{
    auto left = equality();
    if(!left) return 0;

    while(token.type > Token::Type::EndOfFile){
        int line = token.line;
        Token::Type operation = token.type;
        switch(token.type){
            case Token::Type::BitAnd:
            case Token::Type::BitNand:{
                getToken();
                auto right = equality();
                if(!right){
                    printError("Invalid AND expression");
                    delete left;
                    return 0;
                }
                auto result = new AST::Expression(line, astFilenameIndex);
                result->operation = operation;
                result->left      = left;
                result->right     = right;
                left = result;
                break;
            }
            default:
                return left;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------

// Equality = Relational { ( "==" | "!=" ) Relational };
AST::AST* Parser::equality()
{
    auto left = relational();
    if(!left) return 0;

    while(token.type > Token::Type::EndOfFile){
        int line = token.line;
        Token::Type operation = token.type;
        switch(token.type){
            case Token::Type::Equal:
            case Token::Type::NotEqual:{
                getToken();
                auto right = relational();
                if(!right){
                    printError("Invalid equality expression");
                    delete left;
                    return 0;
                }
                auto result = new AST::Expression(line, astFilenameIndex);
                result->operation = operation;
                result->left      = left;
                result->right     = right;
                left = result;
                break;
            }
            default:
                return left;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------

// Relational = Shift { ( "<" | ">" | "<=" | ">=" ) Shift };
AST::AST* Parser::relational()
{
    auto left = shift();
    if(!left) return 0;

    while(token.type > Token::Type::EndOfFile){
        int line = token.line;
        Token::Type operation = token.type;
        switch(token.type){
            case Token::Type::Less:
            case Token::Type::Greater:
            case Token::Type::LessEqual:
            case Token::Type::GreaterEqual:{
                getToken();
                auto right = shift();
                if(!right){
                    printError("Invalid relational expression");
                    delete left;
                    return 0;
                }
                auto result = new AST::Expression(line, astFilenameIndex);
                result->operation = operation;
                result->left      = left;
                result->right     = right;
                left = result;
                break;
            }
            default:
                return left;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------

// Shift = Additive { ( "<<" | ">>") Additive };
AST::AST* Parser::shift()
{
    auto left = additive();
    if(!left) return 0;

    while(token.type > Token::Type::EndOfFile){
        int line = token.line;
        Token::Type operation = token.type;
        switch(token.type){
            case Token::Type::ShiftLeft:
            case Token::Type::ShiftRight:{
                getToken();
                auto right = additive();
                if(!right){
                    printError("Invalid shift expression");
                    delete left;
                    return 0;
                }
                auto result = new AST::Expression(line, astFilenameIndex);
                result->operation = operation;
                result->left      = left;
                result->right     = right;
                left = result;
                break;
            }
            default:
                return left;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------

// Additive = Multiplicative { ( "+" | "-" ) Multiplicative };
AST::AST* Parser::additive()
{
    auto left = multiplicative();
    if(!left) return 0;

    while(token.type > Token::Type::EndOfFile){
        int line = token.line;
        Token::Type operation = token.type;
        switch(token.type){
            case Token::Type::Add:
            case Token::Type::Subtract:{
                getToken();
                auto right = multiplicative();
                if(!right){
                    printError("Invalid additive expression");
                    delete left;
                    return 0;
                }
                auto result = new AST::Expression(line, astFilenameIndex);
                result->operation = operation;
                result->left      = left;
                result->right     = right;
                left = result;
                break;
            }
            default:
                return left;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------

// Multiplicative = Exponential { ( "*" | "/" | "%" ) Exponential };
AST::AST* Parser::multiplicative()
{
    auto left = exponential();
    if(!left) return 0;

    while(token.type > Token::Type::EndOfFile){
        int line = token.line;
        Token::Type operation = token.type;
        switch(token.type){
            case Token::Type::Multiply:
            case Token::Type::Divide:
            case Token::Type::Modulus:{
                getToken();
                auto right = exponential();
                if(!right){
                    printError("Invalid multiplicative expression");
                    delete left;
                    return 0;
                }
                auto result = new AST::Expression(line, astFilenameIndex);
                result->operation = operation;
                result->left      = left;
                result->right     = right;
                left = result;
                break;
            }
            default:
                return left;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------

// Exponential = Replication { "**" Replication };
AST::AST* Parser::exponential()
{
    auto left = replication();
    if(!left) return 0;

    while(token.type > Token::Type::EndOfFile){
        int line = token.line;
        Token::Type operation = token.type;
        switch(token.type){
            case Token::Type::Exponential:{
                getToken();
                auto right = replication();
                if(!right){
                    printError("Invalid exponential expression");
                    delete left;
                    return 0;
                }
                auto result = new AST::Expression(line, astFilenameIndex);
                result->operation = operation;
                result->left      = left;
                result->right     = right;
                left = result;
                break;
            }
            default:
                return left;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------

// Replication = Stringification [ "`" Primary ]; (* Vector replication *)
AST::AST* Parser::replication()
{
    auto left = stringification();
    if(!left) return 0;

    int line = token.line;
    Token::Type operation = token.type;
    switch(token.type){
        case Token::Type::Replicate:{
            getToken();
            auto right = primary();
            if(!right){
                printError("Invalid replicate expression");
                delete left;
                return 0;
            }
            auto result = new AST::Expression(line, astFilenameIndex);
            result->operation = operation;
            result->left      = left;
            result->right     = right;
            return result;
        }
        default:
            return left;
    }
    return 0;
}
//------------------------------------------------------------------------------

// Stringification = [ "$" ] Reduction
//                 | "$(" Reduction [ "," Expression ] ")";
AST::AST* Parser::stringification()
{
    int line = token.line;
    switch(token.type){
        case Token::Type::Stringify:{
            auto expr= reduction();
            if(!expr){
                printError("Invalid stringification");
                return 0;
            }
            auto result = new AST::Stringify(line, astFilenameIndex);
            result->expression = expr;
            return result;
        }
        case Token::Type::StringifyExpression:{
            auto result = new AST::Stringify(line, astFilenameIndex);
            result->expression = reduction();
            if(!result->expression){
                printError("Invalid stringification expression");
                delete result;
                return 0;
            }
            if(token.type == Token::Type::Comma){
                getToken();
                result->format = expression();
                if(!result->format){
                    printError("Invalid stringification format");
                    delete result;
                    return 0;
                }
            }
            return result;
        }
        default:
            return reduction();
    }
    return 0;
}
//------------------------------------------------------------------------------

// Reduction = [ "&" | "~&" | "|" | "~|" | "^" | "~^" | "!" ] Range;
AST::AST* Parser::reduction()
{
    int line = token.line;
    Token::Type operation = token.type;
    switch(token.type){
        case Token::Type::AndReduce:
        case Token::Type::NandReduce:
        case Token::Type::OrReduce:
        case Token::Type::NorReduce:
        case Token::Type::XorReduce:
        case Token::Type::XnorReduce:
        case Token::Type::LogicalNot:{
            auto result = new AST::Expression(line, astFilenameIndex);
            result->operation = operation;
            result->right = range();
            if(!result->right){
                printError("Invalid reduction expression");
                delete result;
                return 0;
            }
            return result;
        }
        default:
            return range();
    }
    return 0;
}
//------------------------------------------------------------------------------

// Range = Unary [ ".." Unary [ ":" Unary ] ];
AST::AST* Parser::range()
{
    auto left = unary();
    if(!left) return 0;

    int line = token.line;
    Token::Type operation = token.type;
    switch(token.type){
        case Token::Type::To:{
            getToken();
            auto right = unary();
            if(!right){
                printError("Invalid range expression");
                delete left;
                return 0;
            }
            auto result = new AST::Expression(line, astFilenameIndex);
            result->operation = operation;
            result->left      = left;
            result->right     = right;
            if(token.type == Token::Type::Step){
                getToken();
                result->stepSize = unary();
                if(!result->stepSize){
                    printError("Invalid range step expression");
                    delete result;
                    return 0;
                }
            }
            return result;
        }
        default:
            return left;
    }
    return 0;
}
//------------------------------------------------------------------------------

// Unary = { "-" | "~" | ":" | "++" | "--" } Postfix;
AST::AST* Parser::unary()
{
    int line = token.line;
    Token::Type operation = token.type;
    switch(token.type){
        case Token::Type::Negate:
        case Token::Type::BitNot:
        case Token::Type::RawBits:
        case Token::Type::Increment:
        case Token::Type::Decrement:{
            auto result = new AST::Expression(line, astFilenameIndex);
            result->operation = operation;
            getToken();
            result->right = postfix();
            if(!result->right){
                printError("Invalid unary expression");
                delete result;
                return 0;
            }
            return result;
        }
        default:
            return postfix();
    }
    return 0;
}
//------------------------------------------------------------------------------

// Postfix = Accessor { "++" | "--" | "!" };
AST::AST* Parser::postfix()
{
    auto left = accessor();
    if(!left) return 0;

    while(token.type > Token::Type::EndOfFile){
        int line = token.line;
        Token::Type operation = token.type;
        switch(token.type){
            case Token::Type::Increment:
            case Token::Type::Decrement:
            case Token::Type::Factorial:{
                getToken();
                auto result = new AST::Expression(line, astFilenameIndex);
                result->operation = operation;
                result->left      = left;
                left = result;
                break;
            }
            default:
                return left;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------

// Accessor = Cast { Array           |  (* Slice *)
//                   ParameterList   |  (* Function call *)
//                   "."  Identifier |  (* Member reference *)
//                   "?." Identifier |  (* Safe member reference *)
//                   "'"  Identifier }; (* Attribute reference *)
AST::AST* Parser::accessor()
{
    auto left = castExpression();
    if(!left) return 0;

    while(token.type > Token::Type::EndOfFile){
        int line = token.line;
        Token::Type operation = token.type;
        switch(token.type){
            case Token::Type::OpenSquare:{
                auto result = new AST::Slice(line, astFilenameIndex);
                result->array = left;
                result->slice = array();
                left = result;
                break;
            }
            case Token::Type::OpenRound:{
                auto result = new AST::FunctionCall(line, astFilenameIndex);
                result->name = left;
                result->parameters = parameterList();
                left = result;
                break;
            }
            case Token::Type::AccessMember:
            case Token::Type::AccessMemberSafe:
            case Token::Type::AccessAttribute:{
                getToken();
                if(token.type != Token::Type::Identifier){
                    printError("Identifier expected");
                    delete left;
                    return 0;
                }
                auto result = new AST::Expression(line, astFilenameIndex);
                result->operation = operation;
                result->left      = left;
                auto right        = new AST::Identifier(line, astFilenameIndex);
                right->name       = token.data;
                result->right     = right;
                getToken();
                left = result;
                break;
            }
            default:
                return left;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------

// Cast = Primary [ "@" ( Identifier | Literal | ( "(" ExpressionList ")" ) ) ];
AST::AST* Parser::castExpression()
{
    auto left = primary();
    if(!left) return 0;

    int line = token.line;
    if(token.type == Token::Type::CastOp){
        getToken();
        auto result = new AST::Expression(line, astFilenameIndex);
        result->operation = Token::Type::CastOp;
        result->left = left;

        switch(token.type){
            case Token::Type::Identifier:{
                auto right    = new AST::Identifier(line, astFilenameIndex);
                right->name   = token.data;
                result->right = right;
                getToken();
                return result;
            }
            case Token::Type::Literal:{
                auto right    = new AST::Literal(line, astFilenameIndex);
                right->value  = token.value;
                result->right = right;
                getToken();
                return result;
            }
            case Token::Type::OpenRound:{
                getToken();
                result->right = expressionList();
                if(token.type != Token::Type::CloseRound){
                    printError(") expected");
                    delete result;
                    return 0;
                }
                return result;
            }
            default:
                printError("Invalid cast expression");
                delete result;
                return 0;
        }
    }
    return left;
}
//------------------------------------------------------------------------------

// Primary = ( ["'"] Identifier ) | Literal | "true" | "false"
//         | VectorConcat | ArrayConcat | AttributeList | Array | String
//         | ( "(" Expression ")" );
AST::AST* Parser::primary()
{
    int line = token.line;
    switch(token.type){
        case Token::Type::AccessAttribute:{
            auto result = new AST::Expression(line, astFilenameIndex);
            getToken();
            if(token.type != Token::Type::Identifier){
                printError("Identifier expected");
                return 0;
            }
            auto right = new AST::Identifier(line, astFilenameIndex);
            right->name = token.data;
            getToken();
            result->right = right;
            return result;
        }
        case Token::Type::Identifier:{
            auto result = new AST::Identifier(line, astFilenameIndex);
            result->name = token.data;
            getToken();
            return result;
        }
        case Token::Type::Literal:{
            auto result = new AST::Literal(line, astFilenameIndex);
            result->value = token.value;
            getToken();
            return result;
        }
        case Token::Type::True:{
            auto result = new AST::Literal(line, astFilenameIndex);
            result->value = 1;
            getToken();
            return result;
        }
        case Token::Type::False:{
            auto result = new AST::Literal(line, astFilenameIndex);
            result->value = 0;
            getToken();
            return result;
        }
        case Token::Type::Concatenate:
        case Token::Type::ArrayConcatenate:{
            return concatenate();
        }
        case Token::Type::OpenAngle:{
            return attributeList();
        }
        case Token::Type::OpenSquare:{
            return array();
        }
        case Token::Type::String:{
            auto result = new AST::String(line, astFilenameIndex);
            result->data = token.data;
            getToken();
            return result;
        }
        case Token::Type::OpenRound:{
            getToken();
            auto result = expression();
            if(!result){
                printError("Expression expected");
                return 0;
            }
            if(token.type != Token::Type::CloseRound){
                printError(") expected");
                delete result;
                return 0;
            }
            return result;
        }
        default:
            return 0;
    }
    return 0;
}
//------------------------------------------------------------------------------

// VectorConcat = ":(" ExpressionList ")";
// ArrayConcat  = ":[" ExpressionList "]";
AST::AST* Parser::concatenate()
{
    auto result = new AST::Concatenate(token.line, astFilenameIndex);
    result->operation = token.type;
    getToken();
    result->members = expressionList();
    if(!result->members){
        printError("Expression list expected");
        return 0;
    }
    if(result->operation == Token::Type::Concatenate &&
       token.type != Token::Type::CloseRound){
        printError(") expected");
        delete result;
        return 0;

    }else if(result->operation == Token::Type::ArrayConcatenate &&
             token.type != Token::Type::CloseSquare){
        printError("] expected");
        delete result;
        return 0;
    }
    getToken();
    return result;
}
//------------------------------------------------------------------------------

// Array = "[" [ ExpressionList ] "]";
AST::AST* Parser::array()
{
    getToken();
    auto result = expressionList();
    if(token.type != Token::Type::CloseSquare){
        printError("] expected");
        return 0;
    }
    getToken();
    return result;
}
//------------------------------------------------------------------------------

// ExpressionList = Expression {"," Expression };
AST::AST* Parser::expressionList()
{
    AST::AST* result = expression();

    if(result){
        auto last    = result;
        auto current = result;

        while(token.type == Token::Type::Comma){
            getToken();
            current = expression();
            if(!current){
                printError("Expression expected");
                delete result;
                return 0;
            }
            last->next = current;
            last = current;
        }
    }
    return result;
}
//------------------------------------------------------------------------------

// ParameterList = "(" [Parameter {"," Parameter } ] ")";
AST::AST* Parser::parameterList()
{
    getToken();

    AST::AST* result = parameter();

    if(result){
        auto last    = result;
        auto current = result;

        while(token.type == Token::Type::Comma){
            getToken();
            current = parameter();
            if(!current){
                printError("Parameter expected");
                delete result;
                return 0;
            }
            last->next = current;
            last = current;
        }
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

// Parameter = [Identifier ("=" | ":=")] Expression;
AST::AST* Parser::parameter()
{
    auto expr = expression();

    if(token.type == Token::Type::Assign ||
       token.type == Token::Type::RawAssign){
        if(expr->type != AST::AST::Type::Identifier){
            printError("Parameter should be an identifier");
            delete expr;
            return 0;
        }
        auto result = new AST::Assignment(token.line, astFilenameIndex);
        result->operation = token.type;
        result->target = expr;
        getToken();
        result->expression = expression();
        if(!result->expression){
            printError("Expression expected");
            delete result;
            return 0;
        }
        return result;
    }
    return expr;
}
//------------------------------------------------------------------------------

// AttributeList = "<" AttributeAssignment { "," AttributeAssignment } ">";
AST::AST* Parser::attributeList()
{
    getToken();

    AST::AST* result = attributeAssignment();

    if(!result){
        printError("Attribute assignment expected");
        delete result;
        return 0;
    }
    auto last    = result;
    auto current = result;

    while(token.type == Token::Type::Comma){
        getToken();
        current = attributeAssignment();
        if(!current){
            printError("Attribute assignment expected");
            delete result;
            return 0;
        }
        last->next = current;
        last = current;
    }
    if(token.type != Token::Type::CloseAngle){
        printError("> expected");
        delete result;
        return 0;
    }
    getToken();

    return result;
}
//------------------------------------------------------------------------------

// AttributeAssignment = Identifier "=" Primary;
AST::AST* Parser::attributeAssignment()
{
    if(token.type != Token::Type::Identifier){
        printError("Identifier expected");
        return 0;
    }
    auto result = new AST::Assignment(token.line, astFilenameIndex);
    result->operation = Token::Type::Assign;

    auto target = new AST::Identifier(token.line, astFilenameIndex);
    target->name = token.data;
    result->target = target;
    getToken();

    if(token.type != Token::Type::Assign){
        printError("= expected");
        delete result;
        return 0;
    }
    getToken();

    result->expression = primary();
    if(!result->expression){
        printError("Primary expression expected");
        delete result;
        return 0;
    }
    return result;
}
//------------------------------------------------------------------------------

AST::AST* Parser::identifierStatement()
{
    AST::AST* left = accessor();
    if(!left){
        printError("Invalid left expression");
        return 0;
    }

    switch(token.type){
        case Token::Type::Colon:
            printError("TODO: Label");
            return 0;

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
        case Token::Type::ShiftRightAssign:{
            auto result = new AST::Assignment(token.line, astFilenameIndex);
            result->operation = token.type;
            result->target = left;
            getToken();
            result->expression = expression();
            if(!result->expression){
                printError("Expression expected");
                delete result;
                return 0;
            }
            if(token.type == Token::Type::Comma){
                getToken();
                return result;
            }else if(token.type == Token::Type::Semicolon){
                // TODO Fence
                getToken();
                return result;
            }else{
                printError(", or ; expected");
                delete result;
                return 0;
            }
        }
        case Token::Type::Increment:
        case Token::Type::Decrement:{
            auto result       = new AST::Assignment(token.line, astFilenameIndex);
            result->operation = token.type;
            result->target    = left;
            getToken();
            if(token.type == Token::Type::Comma){
                getToken();
                return result;
            }else if(token.type == Token::Type::Semicolon){
                // TODO Fence
                getToken();
                return result;
            }else{
                printError(", or ; expected");
                delete result;
                return 0;
            }
            return result;
        }
        case Token::Type::Comma:
            if(left->type != AST::AST::Type::FunctionCall){
                printError("Unexpected ,");
                delete left;
                return 0;
            }
            getToken();
            return left;

        case Token::Type::Semicolon:{
            if(left->type != AST::AST::Type::FunctionCall){
                printError("Unexpected ;");
                delete left;
                return 0;
            }
            // TODO Fence
            getToken();
            return left;
        }
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

AST::AST* Parser::variableDefList(Token::Type type)
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
                isVariableDef = true;
                auto current = new AST::VariableDef(token.line, astFilenameIndex);
                current->defType = type;
                current->name    = identifier;
                if(last) last->next = current;
                else     result     = current;
                last = current;
                getToken();
                break;
            }
            case Token::Type::Semicolon:{
                auto current = new AST::VariableDef(token.line, astFilenameIndex);
                current->defType = type;
                current->name    = identifier;
                if(last) last->next = current;
                else     result     = current;
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
        return variableDefList(type);

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

        case Token::Type::AccessAttribute:
            printError("TODO global attribute");
            return 0;

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

    // Definition can return a list, so run to the end
    while(last->next) last = last->next;

    current = statement();
    while(current){
        last->next = current;
        last = current;
        while(last->next) last = last->next;
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
