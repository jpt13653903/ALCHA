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
//------------------------------------------------------------------------------

using std::list;
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

void Parser::getToken()
{
    if(error) return;

    if(!scanner.getToken(&token)) return;

    #ifdef DEBUG
        logger.print(ANSI_FG_BRIGHT_BLACK);
        logger.print(scanner.getFilename());
        logger.print(":" ANSI_FG_CYAN "");
        logger.print("%05d", token.line);
        logger.print("  \t" ANSI_RESET);
        switch(token.type){
            case Token::Type::Identifier: logger.print("identifier\t"                 ); break;
            case Token::Type::Literal   : logger.print("literal   \t"                 ); break;
            case Token::Type::String    : logger.print("string    \t\""               ); break;
            default                     : logger.print("token %d  \t", (int)token.type); break;
        }
        logger.print(token.data.c_str());
        switch(token.type){
            case Token::Type::Literal: logger.print(" = %s", token.value.display()); break;
            case Token::Type::String : logger.print("\""); break;
            default                  : break;
        }
        logger.print("\n");
    #endif
}
//------------------------------------------------------------------------------

AST::Assignment* Parser::attributeAssignment()
{
    AST::Assignment* node;

    if(token.type != Token::Type::Identifier){
        printError("Attribute expected");
        return 0;
    }
    node = new AST::Assign(token.line, scanner.getFilename());
    node->left = identifier();
    if(!node->left){
        printError("Identifier expected");
        delete node;
        return 0;
    }

    if(token.type != Token::Type::Assign){
        printError("'=' expected");
        delete node;
        return 0;
    }
    getToken();

    node->right = primary();
    if(!node->right){
        printError("Attribute value expected");
        delete node;
        return 0;
    }

    return node;
}
//------------------------------------------------------------------------------

AST::Assignment* Parser::attributeList()
{
    AST::Assignment* head = 0;
    AST::Assignment* tail = 0;
    AST::Assignment* node;

    if(token.type != Token::Type::OpenAngle) return 0;
    getToken();

    while(token.type != Token::Type::Unknown){
        node = attributeAssignment();
        if(!node){
            printError("Attribute assignment expected");
            if(head) delete head;
            return 0;
        }
        if(tail){
            tail->next = node;
            node->prev = tail;
        }else{
            head = node;
        }
        tail = node;

        if(token.type == Token::Type::CloseAngle){
            getToken();
            return head;
        }
        if(token.type != Token::Type::Comma){
            printError("',' or '>' expected");
            delete head;
            return 0;
        }
        getToken();
    }
    printError("Incomplete attribute list");
    if(head) delete head;
    return 0;
}
//------------------------------------------------------------------------------

AST::ClassDefinition* Parser::classDefinition()
{
    if(token.type != Token::Type::Class) return 0;
    AST::ClassDefinition* node = new AST::ClassDefinition(
        token.line, scanner.getFilename()
    );
    getToken();

    node->attributes = attributeList();

    if(token.type != Token::Type::Identifier){
        printError("Identifier expected");
        delete node;
        return 0;
    }
    node->identifier = token.data;
    getToken();

    if(token.type == Token::Type::OpenRound){
        getToken();

        node->parameters = defParameterList();

        if(token.type != Token::Type::CloseRound){
            printError(") expected");
            delete node;
            return 0;
        }
        getToken();
    }

    if(token.type == Token::Type::Colon){
        AST::ClassDefinition::Parent* parent;
        AST::ClassDefinition::Parent* lastParent = 0;
        do{
            getToken();

            parent = new AST::ClassDefinition::Parent;
            if(lastParent) lastParent->next = parent;
            else           node->parents    = parent;
            lastParent = parent;

            parent->className = typeIdentifier();
            if(!parent->className){
                printError("Type identifier expected");
                delete node;
                return 0;
            }

            parameterList(parent->parameters);
        }while(token.type == Token::Type::Comma);
    }
    if(token.type != Token::Type::OpenCurly){
        printError("{ expected");
        delete node;
        return 0;
    }
    getToken();

    node->body = statements();

    if(token.type != Token::Type::CloseCurly){
        printError("} expected");
        delete node;
        return 0;
    }
    getToken();

    return node;
}
//------------------------------------------------------------------------------

AST::EnumDefinition* Parser::enumDefinition()
{
    if(token.type != Token::Type::Enum) return 0;
    AST::EnumDefinition* node = new AST::EnumDefinition(
        token.line, scanner.getFilename()
    );
    getToken();

    if(token.type != Token::Type::Identifier){
        printError("Identifier expected");
        delete node;
        return 0;
    }
    node->identifier = token.data;
    getToken();

    if(token.type != Token::Type::OpenCurly){
        printError("{ expected");
        delete node;
        return 0;
    }

    AST::EnumDefinition::Value* value;
    AST::EnumDefinition::Value* lastValue = 0;

    do{
        getToken();

        value = new AST::EnumDefinition::Value;
        if(lastValue) lastValue->next = value;
        else          node->values    = value;
        lastValue = value;

        if(token.type != Token::Type::Identifier){
            printError("Identifier expected");
            delete node;
            return 0;
        }
        value->identifier = token.data;
        getToken();
    }while(token.type == Token::Type::Comma);

    if(token.type != Token::Type::CloseCurly){
        printError("} expected");
        delete node;
        return 0;
    }
    getToken();

    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::string()
{
    if(token.type != Token::Type::String) return 0;

    AST::String* node = new AST::String(token.line, scanner.getFilename());

    node->value = token.data;

    getToken();
    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::literal()
{
    switch(token.type){
        case Token::Type::True   : token.value = 1; break;
        case Token::Type::False  : token.value = 0; break;
        case Token::Type::Literal:                  break;
        default: return 0;
    }

    AST::Literal* node = new AST::Literal(token.line, scanner.getFilename());

    node->value = token.value;

    getToken();
    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::identifier()
{
    if(token.type != Token::Type::Identifier) return 0;

    AST::Identifier* node = new AST::Identifier(token.line, scanner.getFilename());
    node->name = token.data;

    getToken();
    return node;
}
//------------------------------------------------------------------------------

bool Parser::expressionList(std::vector<AST::Expression*>& list)
{
    AST::Expression* node;

    node = expression();
    if(!node) return false;
    list.push_back(node);

    while(token.type == Token::Type::Comma){
        getToken();
        node = expression();
        if(!node){
            printError("Expression expected");
            for(auto element: list) delete element;
            list.clear();
            return false;
        }
        list.push_back(node);
    }
    return true;
}
//------------------------------------------------------------------------------

AST::Base* Parser::parameter()
{
    AST::Assignment* node;
    AST::Expression* expr = expression();
    if(!expr) return 0;

    if(expr->type == AST::Base::Type::Identifier){
        switch(token.type){
            case Token::Type::Assign:
                node = new AST::Assign(token.line, scanner.getFilename());
                break;

            case Token::Type::RawAssign:
                node = new AST::RawAssign(token.line, scanner.getFilename());
                break;

            default:
                return expr;
        }
        getToken();

        node->left  = expr;
        node->right = expression();

        if(!node->right){
            printError("Expression expected");
            delete node;
            return 0;
        }
        return node;
    }
    return expr;
}
//------------------------------------------------------------------------------

bool Parser::parameterList(list<AST::Base*>& parameters)
{
    AST::Base* node;

    if(token.type != Token::Type::OpenRound) return 0;
    getToken();
    if(token.type == Token::Type::CloseRound){
        getToken();
        return true; // The caller checks for OpenRound if required
    }

    while(token.type != Token::Type::Unknown){
        node = parameter();
        if(!node){
            printError("Parameter assignment or expression expected");
            return false;
        }
        parameters.push_back(node);

        if(token.type == Token::Type::CloseRound){
            getToken();
            return true;
        }
        if(token.type != Token::Type::Comma){
            printError("',' or ')' expected");
            return false;
        }
        getToken();
    }
    printError("Incomplete parameter list");
    return false;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::array()
{
    AST::Array* node;

    if(token.type == Token::Type::OpenSquare){
        getToken();

        node = new AST::Array(token.line, scanner.getFilename());
        expressionList(node->elements);

        if(token.type != Token::Type::CloseSquare){
            printError("] expected");
            delete node;
            return 0;
        }
        getToken();
        return node;
    }
    return 0;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::arrayConcat()
{
    AST::ArrayConcatenate* node;

    if(token.type == Token::Type::ArrayConcatenate){
        getToken();

        node = new AST::ArrayConcatenate(token.line, scanner.getFilename());
        expressionList(node->elements);

        if(token.type != Token::Type::CloseSquare){
            printError("] expected");
            delete node;
            return 0;
        }
        getToken();
        return node;
    }
    return 0;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::vectorConcat()
{
    AST::VectorConcatenate* node;

    if(token.type == Token::Type::Concatenate){
        getToken();

        node = new AST::VectorConcatenate(token.line, scanner.getFilename());
        expressionList(node->elements);

        if(token.type != Token::Type::CloseRound){
            printError(") expected");
            delete node;
            return 0;
        }
        getToken();
        return node;
    }
    return 0;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::primary()
{
    AST::Expression* node;
    bool globalAttribute = false;

    if(token.type == Token::Type::AccessAttribute){
        globalAttribute = true;
        getToken();
    }

    node = identifier();
    if(node){
        if(globalAttribute){
            AST::Expression* temp = new AST::AccessAttribute(token.line, scanner.getFilename());
            temp->right = node;
            node = temp;
        }
        return node;
    }
    if(globalAttribute){
        printError("Identifier expected");
        return 0;
    }

    node = literal     (); if(node) return node;
    node = array       (); if(node) return node;
    node = string      (); if(node) return node;
    node = vectorConcat(); if(node) return node;
    node = arrayConcat (); if(node) return node;

    if(token.type == Token::Type::OpenRound){
        getToken();

        node = expression();
        if(!node){
            printError("Expression expected");
            return 0;
        }

        if(token.type != Token::Type::CloseRound){
            printError(") expected");
            delete node;
            return 0;
        }
        getToken();
        return node;
    }
    return 0;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::castEpr(AST::Expression* node)
{
    AST::Expression* temp;

    if(token.type == Token::Type::CastOp){
        temp = new AST::Cast(token.line, scanner.getFilename());
        getToken();
        temp->left = node;
        node = temp;

        node->right = identifier(); if(node->right) return node;
        node->right = literal   (); if(node->right) return node;

        if(token.type != Token::Type::OpenRound){
            printError("Cast expression specifier expected");
            delete node;
            return 0;
        }
        getToken();

        node->right = expression();
        if(!node->right){
            printError("Expression expected");
            delete node;
            return 0;
        }
        if(token.type == Token::Type::Comma){
            getToken();
            node->right->next = expression();
            if(!node->right->next){
                printError("Expression expected");
                delete node;
                return 0;
            }
            node->right->next->prev = node->right;
        }
        if(token.type != Token::Type::CloseRound){
            printError(") expected");
            delete node;
            return 0;
        }
        getToken();
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::cast()
{
    AST::Expression* node;
    node = primary();
    if(!node) return 0;

    return castEpr(node);
}
//------------------------------------------------------------------------------

AST::Expression* Parser::postfix()
{
    AST::Expression* node = cast();
    if(!node) return 0;

    AST::Expression* temp;

    while(token.type != Token::Type::Unknown){
        if(token.type == Token::Type::OpenSquare){ // array slice
            temp = new AST::Slice(token.line, scanner.getFilename());

            temp->left  = node;
            temp->right = array();
            node = temp;

            if(!node->right){
                printError("Slice array expected");
                delete node;
                return 0;
            }

        }else if(token.type == Token::Type::OpenRound){ // function call
            temp = new AST::FunctionCall(token.line, scanner.getFilename());

            temp->left  = node;
            parameterList(((AST::FunctionCall*)temp)->parameters);
            node = temp;

        }else if(token.type == Token::Type::AccessMember){
            temp = new AST::AccessMember(token.line, scanner.getFilename());
            getToken();

            temp->left  = node;
            temp->right = identifier();
            node = temp;

            if(!node->right){
                printError("Identifier expected");
                delete node;
                return 0;
            }

        }else if(token.type == Token::Type::AccessMemberSafe){
            temp = new AST::AccessMemberSafe(token.line, scanner.getFilename());
            getToken();

            temp->left  = node;
            temp->right = identifier();
            node = temp;

            if(!node->right){
                printError("Identifier expected");
                delete node;
                return 0;
            }

        }else if(token.type == Token::Type::AccessAttribute){
            temp = new AST::AccessAttribute(token.line, scanner.getFilename());
            getToken();

            temp->left  = node;
            temp->right = identifier();
            node = temp;

            if(!node->right){
                printError("Identifier expected");
                delete node;
                return 0;
            }

        }else if(token.type == Token::Type::Increment){
            temp = new AST::Increment(token.line, scanner.getFilename());
            getToken();

            temp->left = node;
            node = temp;

        }else if(token.type == Token::Type::Decrement){
            temp = new AST::Decrement(token.line, scanner.getFilename());
            getToken();

            temp->left = node;
            node = temp;

        }else if(token.type == Token::Type::Factorial){
            temp = new AST::Factorial(token.line, scanner.getFilename());
            getToken();

            temp->left = node;
            node = temp;

        }else{
            return node;
        }
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::unary()
{
    AST::Expression* node;
    AST::Expression* head = 0;
    AST::Expression* tail = 0;

    while(token.type != Token::Type::Unknown){
        if(token.type == Token::Type::Negate){
            node = new AST::Negate(token.line, scanner.getFilename());

        }else if(token.type == Token::Type::BitNot){
            node = new AST::BitNot(token.line, scanner.getFilename());

        }else if(token.type == Token::Type::Colon){
            node = new AST::Raw(token.line, scanner.getFilename());

        }else if(token.type == Token::Type::Increment){
            node = new AST::Increment(token.line, scanner.getFilename());

        }else if(token.type == Token::Type::Decrement){
            node = new AST::Decrement(token.line, scanner.getFilename());

        }else{
            break;
        }
        getToken();
        if(tail) tail->right = node;
        else     head        = node;
        tail = node;
    }
    node = postfix();
    if(tail) tail->right = node;
    else     head        = node;
    tail = node;

    if(!node){
        if(head){
            printError("Postfix expected");
            delete head;
        }
        return 0;
    }
    return head;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::range()
{
    AST::Expression* node;
    AST::Range*      temp;

    node = unary();
    if(!node) return 0;

    if(token.type == Token::Type::To){
        temp = new AST::Range(token.line, scanner.getFilename());
        getToken();

        temp->left  = node;
        temp->right = unary();
        if(!temp->right){
            printError("Range end expected");
            delete temp;
            return 0;
        }
        if(token.type == Token::Type::Step){
            getToken();
            temp->step = unary();
            if(!temp->step){
                printError("Range step expected");
                delete temp;
                return 0;
            }
        }
        node = temp;
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::reduction()
{
    AST::Expression* node = 0;
    switch(token.type){
        case Token::Type::BitAnd:
            node = new AST::AndReduce(token.line, scanner.getFilename());
            getToken();
            break;

        case Token::Type::BitNand:
            node = new AST::NandReduce(token.line, scanner.getFilename());
            getToken();
            break;

        case Token::Type::BitOr:
            node = new AST::OrReduce(token.line, scanner.getFilename());
            getToken();
            break;

        case Token::Type::BitNor:
            node = new AST::NorReduce(token.line, scanner.getFilename());
            getToken();
            break;

        case Token::Type::BitXor:
            node = new AST::XorReduce(token.line, scanner.getFilename());
            getToken();
            break;

        case Token::Type::BitXnor:
            node = new AST::XnorReduce(token.line, scanner.getFilename());
            getToken();
            break;

        case Token::Type::LogicalNot:
            node = new AST::LogicalNot(token.line, scanner.getFilename());
            getToken();
            break;

        default:
            break;
    }
    if(node){
        node->right = range();
        if(!node->right){
            printError("Range expected");
            delete node;
            return 0;
        }
    }else{
        node = range();
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::stringification()
{
    // TODO
    return reduction();
}
//------------------------------------------------------------------------------

AST::Expression* Parser::replication()
{
    AST::Expression* temp;
    AST::Expression* node;

    node = stringification();
    if(!node) return 0;

    if(token.type == Token::Type::Replicate){
        temp = new AST::Replicate(token.line, scanner.getFilename());
        getToken();

        temp->left = node;
        node = temp;

        node->right = primary();
        if(!node->right){
            printError("Primary expected");
            delete node;
            return 0;
        }
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::exponential()
{
    AST::Expression* temp;
    AST::Expression* node;

    node = replication();
    if(!node) return 0;

    while(token.type != Token::Type::Unknown){
        switch(token.type){
            case Token::Type::Exponential:
                temp = new AST::Exponential(token.line, scanner.getFilename());
                break;
            default:
                return node;
        }
        getToken();

        temp->left = node;
        node = temp;

        node->right = replication();
        if(!node->right){
            printError("Replication expected");
            delete node;
            return 0;
        }
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::multiplicative()
{
    AST::Expression* temp;
    AST::Expression* node;

    node = exponential();
    if(!node) return 0;

    while(token.type != Token::Type::Unknown){
        switch(token.type){
            case Token::Type::Multiply:
                temp = new AST::Multiply(token.line, scanner.getFilename());
                break;
            case Token::Type::Divide:
                temp = new AST::Divide(token.line, scanner.getFilename());
                break;
            case Token::Type::Modulus:
                temp = new AST::Modulus(token.line, scanner.getFilename());
                break;
            default:
                return node;
        }
        getToken();

        temp->left = node;
        node = temp;

        node->right = exponential();
        if(!node->right){
            printError("Exponential expected");
            delete node;
            return 0;
        }
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::additive()
{
    AST::Expression* temp;
    AST::Expression* node;

    node = multiplicative();
    if(!node) return 0;

    while(token.type != Token::Type::Unknown){
        switch(token.type){
            case Token::Type::Add:
                temp = new AST::Add(token.line, scanner.getFilename());
                break;
            case Token::Type::Subtract:
                temp = new AST::Subtract(token.line, scanner.getFilename());
                break;
            default:
                return node;
        }
        getToken();

        temp->left = node;
        node = temp;

        node->right = multiplicative();
        if(!node->right){
            printError("Multiplicative expected");
            delete node;
            return 0;
        }
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::shift()
{
    AST::Expression* temp;
    AST::Expression* node;

    node = additive();
    if(!node) return 0;

    while(token.type != Token::Type::Unknown){
        switch(token.type){
            case Token::Type::ShiftLeft:
                temp = new AST::ShiftLeft(token.line, scanner.getFilename());
                break;
            case Token::Type::ShiftRight:
                temp = new AST::ShiftRight(token.line, scanner.getFilename());
                break;
            default:
                return node;
        }
        getToken();

        temp->left = node;
        node = temp;

        node->right = additive();
        if(!node->right){
            printError("Additive expected");
            delete node;
            return 0;
        }
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::relational()
{
    AST::Expression* temp;
    AST::Expression* node;

    node = shift();
    if(!node) return 0;

    while(token.type != Token::Type::Unknown){
        switch(token.type){
            case Token::Type::Less:
                temp = new AST::Less(token.line, scanner.getFilename());
                break;
            case Token::Type::Greater:
                temp = new AST::Greater(token.line, scanner.getFilename());
                break;
            case Token::Type::LessEqual:
                temp = new AST::LessEqual(token.line, scanner.getFilename());
                break;
            case Token::Type::GreaterEqual:
                temp = new AST::GreaterEqual(token.line, scanner.getFilename());
                break;
            default:
                return node;
        }
        getToken();

        temp->left = node;
        node = temp;

        node->right = shift();
        if(!node->right){
            printError("Shift expected");
            delete node;
            return 0;
        }
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::equality()
{
    AST::Expression* temp;
    AST::Expression* node;

    node = relational();
    if(!node) return 0;

    while(token.type != Token::Type::Unknown){
        switch(token.type){
            case Token::Type::Equal:
                temp = new AST::Equal(token.line, scanner.getFilename());
                break;
            case Token::Type::NotEqual:
                temp = new AST::NotEqual(token.line, scanner.getFilename());
                break;
            default:
                return node;
        }
        getToken();

        temp->left = node;
        node = temp;

        node->right = relational();
        if(!node->right){
            printError("Relational expected");
            delete node;
            return 0;
        }
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::bitwiseAnd()
{
    AST::Expression* temp;
    AST::Expression* node;

    node = equality();
    if(!node) return 0;

    while(token.type != Token::Type::Unknown){
        switch(token.type){
            case Token::Type::BitAnd:
                temp = new AST::BitAnd(token.line, scanner.getFilename());
                break;
            case Token::Type::BitNand:
                temp = new AST::BitNand(token.line, scanner.getFilename());
                break;
            default:
                return node;
        }
        getToken();

        temp->left = node;
        node = temp;

        node->right = equality();
        if(!node->right){
            printError("Equality expected");
            delete node;
            return 0;
        }
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::bitwiseXor()
{
    AST::Expression* temp;
    AST::Expression* node;

    node = bitwiseAnd();
    if(!node) return 0;

    while(token.type != Token::Type::Unknown){
        switch(token.type){
            case Token::Type::BitXor:
                temp = new AST::BitXor(token.line, scanner.getFilename());
                break;
            case Token::Type::BitXnor:
                temp = new AST::BitXnor(token.line, scanner.getFilename());
                break;
            default:
                return node;
        }
        getToken();

        temp->left = node;
        node = temp;

        node->right = bitwiseAnd();
        if(!node->right){
            printError("BitwiseAnd expected");
            delete node;
            return 0;
        }
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::bitwiseOr()
{
    AST::Expression* temp;
    AST::Expression* node;

    node = bitwiseXor();
    if(!node) return 0;

    while(token.type != Token::Type::Unknown){
        switch(token.type){
            case Token::Type::BitOr:
                temp = new AST::BitOr(token.line, scanner.getFilename());
                break;
            case Token::Type::BitNor:
                temp = new AST::BitNor(token.line, scanner.getFilename());
                break;
            default:
                return node;
        }
        getToken();

        temp->left = node;
        node = temp;

        node->right = bitwiseXor();
        if(!node->right){
            printError("BitwiseXor expected");
            delete node;
            return 0;
        }
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::expression()
{
    AST::Expression* temp;
    AST::Expression* node;

    node = bitwiseOr();
    if(!node) return 0;

    if(token.type == Token::Type::TernaryIf){
        temp = new AST::Conditional(token.line, scanner.getFilename());
        getToken();

        temp->left = node;
        node = temp;

        node->right = cast();
        if(!node->right){
            printError("Cast expected");
            delete node;
            return 0;
        }

        if(token.type != Token::Type::TernaryElse){
            printError(": expected");
            delete node;
            return 0;
        }
        getToken();

        node->right->next = cast();
        if(!node->right->next){
            printError("Cast expected");
            delete node;
            return 0;
        }
        node->right->next->prev = node->right;
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Expression* Parser::typeIdentifier()
{
    AST::Expression* node = identifier();
    if(!node) return 0;

    AST::Expression* temp;

    while(token.type != Token::Type::Unknown){
        if(token.type == Token::Type::AccessMember){
            temp = new AST::AccessMember(token.line, scanner.getFilename());
            getToken();

            temp->left  = node;
            temp->right = identifier();
            node = temp;

            if(!node->right){
                printError("Identifier expected");
                delete node;
                return 0;
            }
        }else{
            return node;
        }
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Assignment* Parser::initialiser(std::string& identifier)
{
    AST::Assignment* node;

    switch(token.type){
        case Token::Type::Assign:
            node = new AST::Assign(token.line, scanner.getFilename());
            break;
        case Token::Type::RawAssign:
            node = new AST::RawAssign(token.line, scanner.getFilename());
            break;
        default:
            return 0;
    }
    getToken();

    node->right = expression();
    if(!node->right){
        printError("Expression expected");
        delete node;
        return 0;
    }
    node->left       = new AST::Identifier(node->source.line, scanner.getFilename());
    ((AST::Identifier*)node->left)->name = identifier;
    return node;
}
//------------------------------------------------------------------------------

AST::Definition::Array* Parser::arrayDefinition()
{
    AST::Definition::Array* array;

    if(token.type != Token::Type::OpenSquare) return 0;
    getToken();

    array = new AST::Definition::Array;

    array->size = expression();

    if(token.type != Token::Type::CloseSquare){
        printError("']' expected");
        delete array;
        return 0;
    }
    getToken();
    return array;
}
//------------------------------------------------------------------------------

AST::Definition* Parser::defParameter()
{
    AST::Definition* node;

    switch(token.type){
        case Token::Type::Pin:
            node = new AST::PinDefinition(token.line, scanner.getFilename());
            getToken();
            break;
        case Token::Type::Net:
            node = new AST::NetDefinition(token.line, scanner.getFilename());
            getToken();
            break;
        case Token::Type::Byte:
            node = new AST::ByteDefinition(token.line, scanner.getFilename());
            getToken();
            break;
        case Token::Type::Char:
            node = new AST::CharDefinition(token.line, scanner.getFilename());
            getToken();
            break;
        case Token::Type::Num:
            node = new AST::NumDefinition(token.line, scanner.getFilename());
            getToken();
            break;
        case Token::Type::Func:
            node = new AST::FuncPtrDefinition(token.line, scanner.getFilename());
            getToken();
            break;
        case Token::Type::Identifier:{
            auto temp = new AST::ClassInstance(token.line, scanner.getFilename());
            temp->className = typeIdentifier();
            node = temp;
            if(!temp->className){
                printError("Invalid identifier or class name specifier");
                delete node;
                return 0;
            }
            break;
        }

        default:
            return 0;
    }

    if(token.type == Token::Type::Identifier){
        node->identifiers = new AST::Definition::Identifier();
        node->identifiers->identifier = token.data;
        getToken();

    }else if(node->type == AST::Base::Type::ClassInstance){
        auto temp = ((AST::ClassInstance*)node);
        if(
            temp->className &&
            temp->className->type == AST::Base::Type::Identifier
        ){
            node = new AST::AutoDefinition(temp->source.line, temp->source.filename);
            node->identifiers = temp->identifiers;
            temp->identifiers = 0;
            delete temp;

        }else{
            printError("Identifier expected");
            delete node;
            return 0;
        }
    }else{
        printError("Identifier expected");
        delete node;
        return 0;
    }
    assert(node->identifiers);
    while(token.type == Token::Type::OpenSquare){
        getToken();
        if(token.type != Token::Type::CloseSquare){
            printError("] expected");
            delete node;
            return 0;
        }
        getToken();
        auto temp = new AST::Definition::Array();
        temp->next = node->identifiers->array;
        node->identifiers->array = temp;
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Definition* Parser::defParameterList()
{
    AST::Definition* head;
    AST::Definition* node;

    head = node = defParameter();
    if(!node) return 0;

    while(token.type == Token::Type::Comma){
        getToken();
        node->next = defParameter();
        if(node->next) node->next->prev = node;
        node = (AST::Definition*)node->next;
        if(!node){
            printError("DefParameter expected");
            delete head;
            return 0;
        }
    }
    return head;
}
//------------------------------------------------------------------------------

AST::Definition::Identifier* Parser::identifierList()
{
    AST::Definition::Identifier* head;
    AST::Definition::Identifier* node;
    AST::Definition::Array     * array;

    if(token.type != Token::Type::Identifier) return 0;

    head = node = new AST::Definition::Identifier;
    node->identifier = token.data;
    getToken();

    node->array = array = arrayDefinition();
    while(array){
        array->next = arrayDefinition();
        array = array->next;
    }

    if(token.type == Token::Type::OpenRound){ // function definition
        getToken();

        node->function   = true;
        node->parameters = defParameterList();
        if(error){
            delete head;
            return 0;
        }
        if(token.type != Token::Type::CloseRound){
            printError(") expected");
            delete head;
            return 0;
        }
        getToken();
        if(token.type != Token::Type::OpenCurly){
            printError("{ expected");
            delete head;
            return 0;
        }
        getToken();
        node->functionBody = statements();
        if(token.type != Token::Type::CloseCurly){
            printError("} expected");
            delete head;
            return 0;
        }
        getToken();
        return head;
    }

    node->initialiser = initialiser(node->identifier);

    while(token.type == Token::Type::Comma){
        getToken();

        if(token.type != Token::Type::Identifier){
            printError("Identifier expected");
            delete head;
            return 0;
        }
        node->next = new AST::Definition::Identifier;
        node = node->next;
        node->identifier = token.data;
        getToken();

        node->array = array = arrayDefinition();
        while(array){
            array->next = arrayDefinition();
            array = array->next;
        }

        node->initialiser = initialiser(node->identifier);
    }
    if(token.type != Token::Type::Semicolon){
        printError("; expected");
        delete head;
        return 0;
    }
    getToken();
    return head;
}
//------------------------------------------------------------------------------

AST::Definition* Parser::definition()
{
    AST::Definition::Direction direction =
        AST::Definition::Direction::Inferred;

    if(token.type == Token::Type::Input){
        direction = AST::Definition::Direction::Input;
        getToken();

    }else if(token.type == Token::Type::Output){
        direction = AST::Definition::Direction::Output;
        getToken();
    }

    AST::Definition* node = 0;
    switch(token.type){
        case Token::Type::Pin:
            node = new AST::PinDefinition    (token.line, scanner.getFilename()); break;
        case Token::Type::Net:
            node = new AST::NetDefinition    (token.line, scanner.getFilename()); break;
        case Token::Type::Void:
            node = new AST::VoidDefinition   (token.line, scanner.getFilename()); break;
        case Token::Type::Auto:
            node = new AST::AutoDefinition   (token.line, scanner.getFilename()); break;
        case Token::Type::Byte:
            node = new AST::ByteDefinition   (token.line, scanner.getFilename()); break;
        case Token::Type::Char:
            node = new AST::CharDefinition   (token.line, scanner.getFilename()); break;
        case Token::Type::Num:
            node = new AST::NumDefinition    (token.line, scanner.getFilename()); break;
        case Token::Type::Func:
            node = new AST::FuncPtrDefinition(token.line, scanner.getFilename()); break;
        default:
            if(direction != AST::Definition::Direction::Inferred) printError("Type name expected");
            return 0;
    }
    node->direction = direction;
    getToken();

    parameterList(node->parameters);

    if(!node->parameters.empty()){
        if(node->type == AST::Base::Type::VoidDefinition){
            printError("Void type does not take parameters");
            delete node;
            return 0;
        }
        if(node->type == AST::Base::Type::AutoDefinition){
            printError("Auto type does not take parameters");
            delete node;
            return 0;
        }
        if(node->type == AST::Base::Type::FuncPtrDefinition){
            printError("Func type does not take parameters");
            delete node;
            return 0;
        }
    }

    node->attributes = attributeList();
    if(error){
        delete node;
        return 0;
    }

    node->identifiers = identifierList();
    if(!node->identifiers){
        printError("IdentifierList expected");
        delete node;
        return 0;
    }
    if(node->type == AST::Base::Type::VoidDefinition){
        if(!node->identifiers->function){
            printError("Only functions can have \"void\" type.\n");
        }
    }
    return node;
}
//------------------------------------------------------------------------------

bool Parser::validNameSpaceSpecifier(AST::Expression* node)
{
    // Only a few operations are invalid:
    switch(node->type){
        case AST::Base::Type::AccessMember:
        case AST::Base::Type::Identifier:
            break;

        default:
            return false;
    }

    if(node->left){
        if(!validNameSpaceSpecifier(node->left)) return false;
    }
    if(node->right){
        if(!validNameSpaceSpecifier(node->right)) return false;
    }
    return true;
}
//------------------------------------------------------------------------------

bool Parser::validTypeSpecifier(AST::Expression* node)
{
    // Only a few operations are invalid:
    switch(node->type){
        case AST::Base::Type::AccessMember:
        case AST::Base::Type::Identifier:
            break;

        case AST::Base::Type::FunctionCall:
            if(node->left){ // Don't test the parameter list
                if(!validTypeSpecifier(node->left)) return false;
            }
            return true;

        default:
            return false;
    }

    // Only the root may be a function call, and test recursively
    if(node->left){
        if(node->left->type == AST::Base::Type::FunctionCall) return false;
        if(!validTypeSpecifier(node->left)) return false;
    }
    if(node->right){
        if(node->right->type == AST::Base::Type::FunctionCall) return false;
        if(!validTypeSpecifier(node->right)) return false;
    }
    return true;
}
//------------------------------------------------------------------------------

bool Parser::validLHS(AST::Expression* node)
{
    switch(node->type){
        case AST::Base::Type::AccessMember:
        case AST::Base::Type::AccessMemberSafe:
        case AST::Base::Type::AccessAttribute:
        case AST::Base::Type::Identifier:
        case AST::Base::Type::VectorConcatenate:
        case AST::Base::Type::ArrayConcatenate:
            break;

        case AST::Base::Type::Slice:
            if(node->left){ // Don't test the slice list
                if(!validLHS(node->left)) return false;
            }
            return true;

        default:
            return false;
    }

    if(node->left){
        if(!validLHS(node->left)) return false;
    }
    if(node->right){
        if(!validLHS(node->right)) return false;
    }
    if(node->type == AST::Base::Type::ArrayConcatenate){
        for(auto element: ((AST::ArrayConcatenate*)node)->elements){
            if(!validLHS(element)) return false;
        }
    }
    return true;
}
//------------------------------------------------------------------------------

AST::Base* Parser::other()
{
    AST::Expression* expr = postfix();
    if(!expr) return 0;

    if(token.type == Token::Type::Comma || token.type == Token::Type::Semicolon){
        if(
            expr->type != AST::Base::Type::FunctionCall &&
            expr->type != AST::Base::Type::Increment    &&
            expr->type != AST::Base::Type::Decrement
        ){
            printError("Unexpected end-of-statement");
            delete expr;
            return 0;
        }
        if(token.type == Token::Type::Semicolon){
            expr->next = new AST::Fence(
                token.line, scanner.getFilename()
            );
            if(expr->next) expr->next->prev = expr;
        }
        getToken();
        return expr;
    }

    if(token.type == Token::Type::AccessMemberPush){ // nameSpace push
        if(!validNameSpaceSpecifier(expr)){
            printError("Invalid name-space specifier expression");
            delete expr;
            return 0;
        }
        AST::NameSpacePush* nameSpace = new AST::NameSpacePush(
            token.line, scanner.getFilename()
        );
        getToken();

        nameSpace->nameSpace  = expr;
        nameSpace->statements = statements();

        if(token.type != Token::Type::CloseCurly){
            printError("} expected");
            delete nameSpace;
            return 0;
        }
        getToken();
        return nameSpace;
    }

    if(token.type == Token::Type::OpenAngle || token.type == Token::Type::Identifier){
        if(!validTypeSpecifier(expr)){
            printError("Invalid type specifier expression");
            delete expr;
            return 0;
        }

        AST::ClassInstance* def = new AST::ClassInstance(token.line, scanner.getFilename());
        if(expr->type == AST::Base::Type::FunctionCall){
            def->className  = expr->left;
            def->parameters = ((AST::FunctionCall*)expr)->parameters;
            expr->left  = 0;
            ((AST::FunctionCall*)expr)->parameters.clear();
            delete expr;
        }else{
            def->className = expr;
        }
        def->attributes  = attributeList ();
        def->identifiers = identifierList();
        if(!def->identifiers){
            printError("Identifier list expected");
            delete def;
            return 0;
        }
        return def;
    }

    if(!validLHS(expr)){
        printError("Invalid left-hand side expression");
        delete expr;
        return 0;
    }
    AST::Assignment* assign;
    switch(token.type){
        case Token::Type::Assign:
            assign = new AST::Assign(token.line, scanner.getFilename());
            break;
        case Token::Type::RawAssign:
            assign = new AST::RawAssign(token.line, scanner.getFilename());
            break;
        case Token::Type::AppendAssign:
            assign = new AST::AppendAssign(token.line, scanner.getFilename());
            break;
        case Token::Type::AddAssign:
            assign = new AST::AddAssign(token.line, scanner.getFilename());
            break;
        case Token::Type::SubtractAssign:
            assign = new AST::SubtractAssign(token.line, scanner.getFilename());
            break;
        case Token::Type::MultiplyAssign:
            assign = new AST::MultiplyAssign(token.line, scanner.getFilename());
            break;
        case Token::Type::DivideAssign:
            assign = new AST::DivideAssign(token.line, scanner.getFilename());
            break;
        case Token::Type::ModulusAssign:
            assign = new AST::ModulusAssign(token.line, scanner.getFilename());
            break;
        case Token::Type::ExponentialAssign:
            assign = new AST::ExponentialAssign(token.line, scanner.getFilename());
            break;
        case Token::Type::AndAssign:
            assign = new AST::AndAssign(token.line, scanner.getFilename());
            break;
        case Token::Type::OrAssign:
            assign = new AST::OrAssign(token.line, scanner.getFilename());
            break;
        case Token::Type::XorAssign:
            assign = new AST::XorAssign(token.line, scanner.getFilename());
            break;
        case Token::Type::ShiftLeftAssign:
            assign = new AST::ShiftLeftAssign(token.line, scanner.getFilename());
            break;
        case Token::Type::ShiftRightAssign:
            assign = new AST::ShiftRightAssign(token.line, scanner.getFilename());
            break;

        default:
            printError("Assignment operator expected");
            delete expr;
            return 0;
    }
    getToken();

    assign->left  = expr;
    assign->right = expression();

    if(!assign->right){
        printError("Expression expected");
        delete assign;
        return 0;
    }
    switch(token.type){
        case Token::Type::Comma: getToken(); break;
        case Token::Type::Semicolon: break;
        default:
            printError(", or ; expected");
            delete assign;
            return 0;
    }
    return assign;
}
//------------------------------------------------------------------------------

AST::IfStatement* Parser::ifStatement()
{
    if(token.type != Token::Type::If) return 0;
    AST::IfStatement* node = new AST::IfStatement(
        token.line, scanner.getFilename()
    );
    getToken();

    if(token.type != Token::Type::OpenRound){
        printError("( expected");
        delete node;
        return 0;
    }
    getToken();

    node->condition = expression();
    if(!node->condition){
        printError("Expression expected");
        delete node;
        return 0;
    }

    if(token.type != Token::Type::CloseRound){
        printError(") expected");
        delete node;
        return 0;
    }
    getToken();

    node->trueStatements = statementBlock();

    if(token.type == Token::Type::Else){
        getToken();
        node->falseStatements = statementBlock();
    }

    return node;
}
//------------------------------------------------------------------------------

AST::WhileLoop* Parser::whileLoop()
{
    if(token.type != Token::Type::While) return 0;
    AST::WhileLoop* node = new AST::WhileLoop(token.line, scanner.getFilename());
    getToken();

    if(token.type != Token::Type::OpenRound){
        printError("( expected");
        delete node;
        return 0;
    }
    getToken();

    node->condition = expression();
    if(!node->condition){
        printError("Expression expected");
        delete node;
        return 0;
    }

    if(token.type != Token::Type::CloseRound){
        printError(") expected");
        delete node;
        return 0;
    }
    getToken();

    node->statements = statementBlock();
    return node;
}
//------------------------------------------------------------------------------

AST::LoopLoop* Parser::loopLoop()
{
    if(token.type != Token::Type::Loop) return 0;
    AST::LoopLoop* node = new AST::LoopLoop(token.line, scanner.getFilename());
    getToken();

    if(token.type == Token::Type::OpenRound){
        getToken();

        node->count = expression();
        if(!node->count){
            printError("Expression expected");
            delete node;
            return 0;
        }

        if(token.type != Token::Type::CloseRound){
            printError(") expected");
            delete node;
            return 0;
        }
        getToken();
    }

    node->statements = statementBlock();
    return node;
}
//------------------------------------------------------------------------------

AST::ForLoop* Parser::forLoop()
{
    if(token.type != Token::Type::For) return 0;
    AST::ForLoop* node = new AST::ForLoop(token.line, scanner.getFilename());
    getToken();

    if(token.type != Token::Type::OpenRound){
        printError("( expected");
        delete node;
        return 0;
    }
    getToken();

    if(token.type != Token::Type::Identifier){
        printError("Identifier expected");
        delete node;
        return 0;
    }
    node->identifier = token.data;
    getToken();

    if(token.type != Token::Type::In){
        printError("\"in\" expected");
        delete node;
        return 0;
    }
    getToken();

    node->range = range();
    if(!node->range){
        printError("Range expected");
        delete node;
        return 0;
    }

    if(token.type != Token::Type::CloseRound){
        printError(") expected");
        delete node;
        return 0;
    }
    getToken();

    node->statements = statementBlock();
    return node;
}
//------------------------------------------------------------------------------

AST::Import* Parser::import()
{
    if(token.type != Token::Type::Import) return 0;
    AST::Import* node = new AST::Import(token.line, scanner.getFilename());
    getToken();

    if(token.type != Token::Type::String){
        printError("File name expected");
        delete node;
        return 0;
    }
    node->file = token.data;
    getToken();

    if(token.type == Token::Type::As){
        getToken();

        if(token.type != Token::Type::Identifier){
            printError("Identifier expected");
            delete node;
            return 0;
        }
        node->nameSpace = token.data;
        getToken();
    }

    if(token.type != Token::Type::Semicolon){
        printError("; expected");
        delete node;
        return 0;
    }
    getToken();

    return node;
}
//------------------------------------------------------------------------------

AST::Switch* Parser::switchStatement()
{
    if(token.type != Token::Type::Switch) return 0;
    AST::Switch* node = new AST::Switch(token.line, scanner.getFilename());
    getToken();

    if(token.type != Token::Type::OpenRound){
        printError("( expected");
        delete node;
        return 0;
    }
    getToken();

    node->expression = expression();
    if(!node->expression){
        printError("Expression expected");
        delete node;
        return 0;
    }
    if(token.type != Token::Type::CloseRound){
        printError(") expected");
        delete node;
        return 0;
    }
    getToken();
    if(token.type != Token::Type::OpenCurly){
        printError("{ expected");
        delete node;
        return 0;
    }
    getToken();

    AST::Switch::Case* theCase;
    AST::Switch::Case* tail = 0;

    while(token.type == Token::Type::Case){
        theCase = new AST::Switch::Case;
        if(tail) tail->next  = theCase;
        else     node->cases = theCase;
        tail = theCase;
        getToken();

        if(token.type != Token::Type::OpenRound){
            printError("'(' expected");
            delete node;
            return 0;
        }
        getToken();

        if(!expressionList(theCase->expressions)){
            printError("Expression list expected");
            delete node;
            return 0;
        }
        if(token.type != Token::Type::CloseRound){
            printError("')' expected");
            delete node;
            return 0;
        }
        getToken();

        theCase->statements = statementBlock();
    }

    if(token.type == Token::Type::Default){
        getToken();
        node->defaultCase = statementBlock();
    }

    if(token.type != Token::Type::CloseCurly){
        printError("} expected");
        delete node;
        return 0;
    }
    getToken();
    return node;
}
//------------------------------------------------------------------------------

AST::Alias* Parser::alias()
{
    if(token.type != Token::Type::Alias) return 0;
    getToken();

    AST::Alias* node = new AST::Alias(token.line, scanner.getFilename());

    if(token.type != Token::Type::Identifier){
        printError("Identifier expected");
        delete node;
        return 0;
    }
    node->identifier = token.data;
    getToken();

    if(token.type != Token::Type::Assign){
        printError("= expected");
        delete node;
        return 0;
    }
    getToken();

    node->expression = expression();
    if(!node->expression){
        printError("Identifier expected");
        delete node;
        return 0;
    }

    if(token.type != Token::Type::Semicolon){
        printError("; expected");
        delete node;
        return 0;
    }
    getToken();

    return node;
}
//------------------------------------------------------------------------------

AST::Group* Parser::group()
{
    if(token.type != Token::Type::Group) return 0;
    getToken();

    AST::Group* node = new AST::Group(token.line, scanner.getFilename());
    if(token.type == Token::Type::OpenAngle){
        node->attributes = attributeList();
    }

    if(token.type == Token::Type::Identifier){
        node->identifier = token.data;
        getToken();
    }

    if(token.type != Token::Type::OpenCurly){
        printError("{ expected");
        delete node;
        return 0;
    }
    getToken();

    node->body = statements();

    if(token.type != Token::Type::CloseCurly){
        printError("} expected");
        delete node;
        return 0;
    }
    getToken();

    return node;
}
//------------------------------------------------------------------------------

AST::Jump* Parser::jump()
{
    AST::Jump* node;

    switch(token.type){
        case Token::Type::Return:
            node = new AST::Jump(
                token.line, scanner.getFilename(), AST::Jump::JumpType::Return
            );
            break;
        case Token::Type::Break:
            node = new AST::Jump(
                token.line, scanner.getFilename(), AST::Jump::JumpType::Break
            );
            break;
        case Token::Type::Continue:
            node = new AST::Jump(
                token.line, scanner.getFilename(), AST::Jump::JumpType::Continue
            );
            break;
        default:
            return 0;
    }
    getToken();

    node->expression = expression();

    if(token.type != Token::Type::Semicolon){
        printError("; expected");
        delete node;
        return 0;
    }
    getToken();

    return node;
}
//------------------------------------------------------------------------------

AST::Rtl* Parser::rtl()
{
    if(token.type != Token::Type::RTL) return 0;
    AST::Rtl* node = new AST::Rtl(token.line, scanner.getFilename());
    getToken();

    parameterList(node->parameters);
    node->statements = statementBlock();
    return node;
}
//------------------------------------------------------------------------------

AST::Fsm* Parser::fsm()
{
    if(token.type != Token::Type::FSM) return 0;
    AST::Fsm* node = new AST::Fsm(token.line, scanner.getFilename());
    getToken();

    parameterList(node->parameters);
    node->statements = statementBlock();
    return node;
}
//------------------------------------------------------------------------------

AST::Hdl* Parser::hdl()
{
    if(token.type != Token::Type::HDL) return 0;
    AST::Hdl* node = new AST::Hdl(token.line, scanner.getFilename());
    getToken();

    if(token.type != Token::Type::OpenRound){
        printError("( expected");
        delete node;
        return 0;
    }
    getToken();

    AST::Expression* file = node->files = string();
    if(file){
        while(token.type == Token::Type::Comma){
            getToken();
            file->next = string();
            if(file->next) file->next->prev = file;
            file = (AST::Expression*)file->next;
            if(!file){
                printError("File name string expected");
                delete node;
                return 0;
            }
        }
    }
    if(token.type != Token::Type::CloseRound){
        printError(") expected");
        delete node;
        return 0;
    }
    getToken();

    if(token.type != Token::Type::Identifier){
        printError("Identifier expected");
        delete node;
        return 0;
    }
    node->identifier = token.data;
    getToken();

    if(token.type == Token::Type::OpenRound){
        getToken();

        AST::Base* tail = 0;
        AST::Base* temp = other(); // Also returns assignments
        while(temp){
            if(!temp->isAssignment()){
                printError("Assignment expected");
                delete temp;
                delete node;
                return 0;
            }
            if(tail){
                tail->next = temp;
                temp->prev = tail;
            }else{
                node->parameters = (AST::Assignment*)temp;
            }
            tail = temp;
            while(tail->next) tail = tail->next;
            temp = other();
        }
        if(token.type != Token::Type::CloseRound){
            printError(") expected");
            delete node;
            return 0;
        }
        getToken();
    }
    if(token.type != Token::Type::OpenCurly){
        printError("{ expected");
        delete node;
        return 0;
    }
    getToken();

    AST::Definition* tail = 0;
    AST::Definition* temp = definition();
    while(temp){
        if(tail){
            tail->next  = temp;
            temp->prev  = tail;
        }else{
            node->ports = temp;
        }
        tail = temp;
        temp = definition();
    }

    if(token.type != Token::Type::CloseCurly){
        printError("} expected");
        delete node;
        return 0;
    }
    getToken();
    return node;
}
//------------------------------------------------------------------------------

AST::Base* Parser::statement()
{
    AST::Base* node;

    node = classDefinition (); if(node) return node;
    node = enumDefinition  (); if(node) return node;
    node = definition      (); if(node) return node;
    node = ifStatement     (); if(node) return node;
    node = whileLoop       (); if(node) return node;
    node = loopLoop        (); if(node) return node;
    node = forLoop         (); if(node) return node;
    node = import          (); if(node) return node;
    node = switchStatement (); if(node) return node;
    node = group           (); if(node) return node;
    node = alias           (); if(node) return node;
    node = jump            (); if(node) return node;
    node = rtl             (); if(node) return node;
    node = fsm             (); if(node) return node;
    node = hdl             (); if(node) return node;
    node = other           (); if(node) return node;

    if(token.type == Token::Type::Semicolon){
        node = new AST::Fence(token.line, scanner.getFilename());
        getToken();
        return node;
    }
    return 0;
}
//------------------------------------------------------------------------------

AST::Base* Parser::statements()
{
    AST::Base* head = 0;
    AST::Base* tail = 0;
    AST::Base* node = 0;

    while(!error && (token.type != Token::Type::Unknown)){
        node = statement();
        if(error) break;
        if(!node) return head;

        if(tail){
            tail->next = node;
            node->prev = tail;
        }else{
            head       = node;
        }
        tail = node;
        while(tail->next) tail = tail->next;
    }
    if(error){
        if(head) delete head;
        return 0;
    }
    return head;
}
//------------------------------------------------------------------------------

AST::Base* Parser::statementBlock()
{
    AST::Base* node;

    if(token.type == Token::Type::OpenCurly){
        getToken();
        node = statements();
        if(token.type != Token::Type::CloseCurly){
            printError("} expected");
            if(node) delete node;
            return 0;
        }
        getToken();
    }else{
        node = statement();
        if(!node) printError("Statement expected");
    }
    return node;
}
//------------------------------------------------------------------------------

AST::Base* Parser::run(const char* filename)
{
    logger.print("Building AST for ");
    logger.print(filename);
    logger.print("...\n");

    error = false;

    if(!scanner.open(filename)) return 0;
    getToken();

    AST::Base* AST = statements();
    if(token.type != Token::Type::Unknown){
        printError("Unexpected token");
        if(AST) delete AST;
        return 0;
    }

    #ifdef DEBUG
        logger.print(ANSI_FG_GREEN "\nDisplaying AST of ");
        logger.print(filename);
        logger.print(" -------------------------------------\n\n" ANSI_RESET);
        if(AST) AST->display();
        else    logger.print("AST is empty\n");

        AST->validate();
    #endif
    return AST;
}
//------------------------------------------------------------------------------
