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

#include <time.h>
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

void Interpreter::printError(AST::AST* node, const char* message)
{
    if(error) return;
    error = true;

    if(!node) return;

    ::printError(node->line, AST::filenameBuffer[node->filenameIndex], message);
    if(node) printf(
        ANSI_FG_CYAN "  node: " ANSI_FG_BRIGHT_MAGENTA "%s\n"
        ANSI_RESET   "%s\n",
        node->decodeType(),
        node->print(1).c_str()
    );
}
//------------------------------------------------------------------------------

bool Interpreter::variableDef(AST::VariableDef* node)
{
    auto def = node->defList;
    while(def){
        if(!def->name.size()){
            printError(node, "Definition with no name");
            return false;
        }
        if(global.symbols.contains(def->name)){
            string s = def->name + " already exists in the current namespace";
            printError(node, s.c_str());
            return false;
        }

        switch(node->defType){
            case Token::Type::Pin:
                printError(node, "TODO Pin Definition");
                return false;

            case Token::Type::Net:
                printError(node, "TODO Net Definition");
                return false;

            case Token::Type::Void:
                printError(node, "TODO Void Definition");
                return false;

            case Token::Type::Auto:
                printError(node, "TODO Auto Definition");
                return false;

            case Token::Type::Byte:
                printError(node, "TODO Byte Definition");
                return false;

            case Token::Type::Char:
                printError(node, "TODO Char Definition");
                return false;

            case Token::Type::Num:{
                auto symbol = new Symbols::Num;
                global.symbols[def->name] = symbol;
                if(node->parameters){
                    printError(node, "TODO Definition with parameters");
                }
                if(node->attributes){
                    printError(node, "TODO Definition with attributes");
                }
                if(def->initialiser){
                    symbol->value = evaluate(def->initialiser);
                }
                break;
            }

            case Token::Type::Func:
                printError(node, "TODO Func Definition");
                return false;

            case Token::Type::Identifier:
                printError(node, "TODO TypeIdentifier Definition");
                return false;

            default:
                printError(node, "Unknown type in variable definition");
                return false;
        }
        def = def->next;
    }
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::functionDef(AST::FunctionDef* node)
{
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::operatorOverload(AST::OperatorOverload* node)
{
    return true;
}
//------------------------------------------------------------------------------

Number Interpreter::evaluate(AST::AST* node)
{
    switch(node->type){
        case AST::Type::Literal:    return evaluate((AST::Literal   *)node);
        case AST::Type::Expression: return evaluate((AST::Expression*)node);
        case AST::Type::Identifier: return evaluate((AST::Identifier*)node);
        default:
            printError(node, "Unsupported node type for evaluation");
            return 0;
    }
}
//------------------------------------------------------------------------------

Number Interpreter::evaluate(AST::Literal* literal)
{
    return literal->value;
}
//------------------------------------------------------------------------------

Number Interpreter::evaluate(AST::Expression* expression)
{
    Number result;

    if(expression->left && expression->right){
        switch(expression->operation){
            case Token::Type::BitOr:
                printError(expression, "TODO: BitOr");
                break;

            case Token::Type::BitNor:
                printError(expression, "TODO: BitNor");
                break;

            case Token::Type::BitXor:
                printError(expression, "TODO: BitXor");
                break;

            case Token::Type::BitXnor:
                printError(expression, "TODO: BitXnor");
                break;

            case Token::Type::BitAnd:
                printError(expression, "TODO: BitAnd");
                break;

            case Token::Type::BitNand:
                printError(expression, "TODO: BitNand");
                break;

            case Token::Type::Equal:
                printError(expression, "TODO: Equal");
                break;

            case Token::Type::NotEqual:
                printError(expression, "TODO: NotEqual");
                break;

            case Token::Type::Less:
                printError(expression, "TODO: Less");
                break;

            case Token::Type::Greater:
                printError(expression, "TODO: Greater");
                break;

            case Token::Type::LessEqual:
                printError(expression, "TODO: LessEqual");
                break;

            case Token::Type::GreaterEqual:
                printError(expression, "TODO: GreaterEqual");
                break;

            case Token::Type::ShiftLeft:
                printError(expression, "TODO: ShiftLeft");
                break;

            case Token::Type::ShiftRight:
                printError(expression, "TODO: ShiftRight");
                break;

            case Token::Type::Add:
                result = evaluate(expression->left) + evaluate(expression->right);
                return result;

            case Token::Type::Subtract:
                result = evaluate(expression->left) - evaluate(expression->right);
                break;

            case Token::Type::Multiply:
                result = evaluate(expression->left) * evaluate(expression->right);
                return result;

            case Token::Type::Divide:
                result = evaluate(expression->left) / evaluate(expression->right);
                return result;

            case Token::Type::Modulus:
                printError(expression, "TODO: Modulus");
                break;

            case Token::Type::Exponential:
                printError(expression, "TODO: Exponential");
                break;

            case Token::Type::Factorial:
                printError(expression, "TODO: Factorial");
                break;

            default:
                printError(expression, "TODO: Unsupported expression operation");
                return 0;
        }
    }else if(expression->left){
        switch(expression->operation){
            case Token::Type::Increment:
                printError(expression, "TODO: Increment");
                break;

            case Token::Type::Decrement:
                printError(expression, "TODO: Decrement");
                break;

            case Token::Type::Factorial:
                printError(expression, "TODO: Factorial");
                break;

            default:
                printError(expression, "TODO: Unsupported expression operation");
                return 0;
        }
    }else if(expression->right){
        switch(expression->operation){
            case Token::Type::AndReduce:
                printError(expression, "TODO: AndReduce");
                break;

            case Token::Type::NandReduce:
                printError(expression, "TODO: NandReduce");
                break;

            case Token::Type::OrReduce:
                printError(expression, "TODO: OrReduce");
                break;

            case Token::Type::NorReduce:
                printError(expression, "TODO: NorReduce");
                break;

            case Token::Type::XorReduce:
                printError(expression, "TODO: XorReduce");
                break;

            case Token::Type::XnorReduce:
                printError(expression, "TODO: XnorReduce");
                break;

            case Token::Type::LogicalNot:
                printError(expression, "TODO: LogicalNot");
                break;

            default:
                printError(expression, "TODO: Unsupported expression operation");
                return 0;
        }
    }else{
        printError(expression, "Expression has no operands");
        return 0;
    }
    return 0;
}
//------------------------------------------------------------------------------

tm* Interpreter::time()
{
    time_t timer;
    ::time(&timer);
    return localtime(&timer);
}
//------------------------------------------------------------------------------

Number Interpreter::evaluate(AST::Identifier* expression)
{
    auto symbol = global.symbols.find(expression->name);
    if(symbol == global.symbols.end()){
        Number result;
        if(expression->name == "pi" || expression->name == "π"){
            result.set_pi();
        }else if(expression->name == "e"){
            result.set_e();
        }else if(expression->name == "i"){
            result.set_i();
        }else if(expression->name == "j"){
            result.set_i();
        }else if(expression->name == "__YEAR__"){
            result = time()->tm_year+1900;
        }else if(expression->name == "__MONTH__"){
            result = time()->tm_mon+1;
        }else if(expression->name == "__DAY__"){
            result = time()->tm_mday;
        }else if(expression->name == "__HOUR__"){
            result = time()->tm_hour;
        }else if(expression->name == "__MINUTE__"){
            result = time()->tm_min;
        }else if(expression->name == "__SECOND__"){
            result = time()->tm_sec;
        }else if(expression->name == "__WEEKDAY__"){
            result = ((time()->tm_wday+6) % 7) + 1;
        }else if(expression->name == "__YEARDAY__"){
            result = time()->tm_yday+1;
        }else{
            printError(expression, "Undefined symbol");
        }
        return result;
    }
    auto object = symbol->second;
    switch(object->type){
        case Symbols::Type::Num:
            return ((Symbols::Num*)object)->value;

        default:
            printError(expression, "Unsupported symbol type for evaluation");
            return 0;
    }
}
//------------------------------------------------------------------------------

bool Interpreter::assign(AST::AST* target, AST::AST* expression)
{
    switch(target->type){
        case AST::Type::Identifier: return assign((AST::Identifier*)target, expression);

        default:
            printError(target, "Unsupported assignment target type");
            return false;
    }
}
//------------------------------------------------------------------------------

bool Interpreter::assign(AST::Identifier* target, AST::AST* expression)
{
    auto symbol = global.symbols.find(target->name);
    if(symbol == global.symbols.end()){
        printError(target, "Undefined symbol");
        return false;
    }
    auto object = symbol->second;
    switch(object->type){
        case Symbols::Type::Num: return assign((Symbols::Num*)object, expression);

        default:
            printError(target, "Unsupported assignment target type");
            return false;
    }
}
//------------------------------------------------------------------------------

bool Interpreter::assign(Symbols::Num* target, AST::AST* expression)
{
    target->value = evaluate(expression);
    return !error;
}
//------------------------------------------------------------------------------

bool Interpreter::assignment(AST::Assignment* node)
{
    return assign(node->target, node->expression);
}
//------------------------------------------------------------------------------

bool Interpreter::print(AST::AST* node)
{
    switch(node->type){
        case AST::Type::Literal:    return print((AST::Literal   *)node);
        case AST::Type::String:     return print((AST::String    *)node);
        case AST::Type::Expression: return print((AST::Expression*)node);
        case AST::Type::Identifier: return print((AST::Identifier*)node);

        default:
            printError(node, "Unsupported node type for printing");
            return false;
    }
}
//------------------------------------------------------------------------------

bool Interpreter::print(AST::Literal* node)
{
    printf(ANSI_FG_BRIGHT_BLACK "Literal:\n    " ANSI_RESET "%s\n",
           evaluate(node).print().c_str());
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::print(AST::String* node)
{
    printf(ANSI_FG_BRIGHT_BLACK "String:\n    " ANSI_RESET "%s\n",
           node->print().c_str());
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::print(AST::Expression* node)
{
    printf(ANSI_FG_BRIGHT_BLACK "%s:\n    " ANSI_RESET "%s\n",
           node->print().c_str(),
           evaluate(node).print().c_str());
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::print(AST::Identifier* node)
{
    printf(ANSI_FG_BRIGHT_BLACK "%s:\n    " ANSI_RESET "%s\n",
           node->print().c_str(),
           evaluate(node).print().c_str());
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::functionCall(AST::FunctionCall* node)
{
    if(node->name->type == AST::Type::Identifier &&
       ((AST::Identifier*)node->name)->name == "print"){
        if(node->parameters) print(node->parameters);
        else                 printf("\n");
    }
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::globalSpace(AST::AST* node)
{
    while(node){
        switch(node->type){
            case AST::Type::Label:
                printError(node, "TODO: Label");
                break;

            case AST::Type::VariableDef:
                if(!variableDef((AST::VariableDef*)node)) return false;
                break;

            case AST::Type::FunctionDef:
                if(!functionDef((AST::FunctionDef*)node)) return false;
                break;

            case AST::Type::OperatorOverload:
                if(!operatorOverload((AST::OperatorOverload*)node)) return false;
                break;

            case AST::Type::ClassDefinition:
                printError(node, "TODO: ClassDefinition");
                break;

            case AST::Type::EnumDefinition:
                printError(node, "TODO: EnumDefinition");
                break;

            case AST::Type::Alias:
                printError(node, "TODO: Alias");
                break;

            case AST::Type::Import:
                printError(node, "TODO: Import");
                break;

            case AST::Type::GroupDefinition:
                printError(node, "TODO: GroupDefinition");
                break;

            case AST::Type::AccessDirectionGroup:
                printError(node, "TODO: AccessDirectionGroup");
                break;

            case AST::Type::ControlStatement:
                printError(node, "TODO: ControlStatement");
                break;

            case AST::Type::Jump:
                printError(node, "TODO: Jump");
                break;

            case AST::Type::FunctionCall:
                if(!functionCall((AST::FunctionCall*)node)) return false;
                break;

            case AST::Type::NameSpacePush:
                printError(node, "TODO: NameSpacePush");
                break;

            case AST::Type::Assignment:
                if(!assignment((AST::Assignment*)node)) return false;
                break;

            case AST::Type::ClockedConstruct:
                printError(node, "TODO: ClockedConstruct");
                break;

            case AST::Type::HdlConstruct:
                printError(node, "TODO: HdlConstruct");
                break;

            case AST::Type::StimulusOrEmulate:
                printError(node, "TODO: StimulusOrEmulate");
                break;

            case AST::Type::ForkJoin:
                printError(node, "TODO: ForkJoin");
                break;

            case AST::Type::Assert:
                printError(node, "TODO: Assert");
                break;

            case AST::Type::Fence:
                printError(node, "TODO: Fence");
                break;

            default:
                printError(node, "Unexpected node type");
                return false;
        }
        node = node->next;
    }
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::run(AST::AST* ast)
{
    if(!globalSpace(ast)) return false;

    if(error) return false;
    return true;
}
//------------------------------------------------------------------------------
