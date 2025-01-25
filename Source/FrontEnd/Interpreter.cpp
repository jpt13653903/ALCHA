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
#include "Parser.h"

#include <time.h>
#include <filesystem>
//------------------------------------------------------------------------------

using std::string;
namespace fs = std::filesystem;
//------------------------------------------------------------------------------

Interpreter::Interpreter()
{
}
//------------------------------------------------------------------------------

Interpreter::~Interpreter()
{
}
//------------------------------------------------------------------------------

void Interpreter::printError(const AST::AST* node, const char* message)
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

Number Interpreter::evaluate(const AST::AST* node)
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

Number Interpreter::evaluate(const AST::Literal* literal)
{
    return literal->value;
}
//------------------------------------------------------------------------------

Number Interpreter::evaluate(const AST::Expression* expression)
{
    Number result;

    if(expression->left && expression->right){
        switch(expression->operation){
            case Token::Type::TernaryIf:
                printError(expression, "TODO: TernaryIf");
                break;

            case Token::Type::Elvis:
                printError(expression, "TODO: Elvis");
                break;

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

            default:{
                printError(expression, "TODO: Unsupported expression operation");
                return 0;
            }
        }
    }else{
        printError(expression, "Expression has no operands");
        return 0;
    }
    return 0;
}
//------------------------------------------------------------------------------

const tm* Interpreter::time() const
{
    time_t timer;
    ::time(&timer);
    return localtime(&timer);
}
//------------------------------------------------------------------------------

Number Interpreter::evaluate(const AST::Identifier* expression)
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

bool Interpreter::assign(const AST::AST* target, const AST::AST* expression)
{
    switch(target->type){
        case AST::Type::Identifier: return assign((AST::Identifier*)target, expression);

        default:
            printError(target, "Unsupported execute target type");
            return false;
    }
}
//------------------------------------------------------------------------------

bool Interpreter::assign(const AST::Identifier* target, const AST::AST* expression)
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
            printError(target, "Unsupported execute target type");
            return false;
    }
}
//------------------------------------------------------------------------------

bool Interpreter::assign(Symbols::Num* target, const AST::AST* expression)
{
    target->value = evaluate(expression);
    return !error;
}
//------------------------------------------------------------------------------

bool Interpreter::print(const AST::AST* node)
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

bool Interpreter::print(const AST::Literal* node)
{
    printf(ANSI_FG_BRIGHT_BLACK "Literal:\n    " ANSI_RESET "%s\n",
           evaluate(node).print().c_str());
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::print(const AST::String* node)
{
    printf(ANSI_FG_BRIGHT_BLACK "String:\n    " ANSI_RESET "%s\n",
           node->print().c_str());
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::print(const AST::Expression* node)
{
    printf(ANSI_FG_BRIGHT_BLACK "%s:\n    " ANSI_RESET "%s\n",
           node->print().c_str(),
           evaluate(node).print().c_str());
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::print(const AST::Identifier* node)
{
    printf(ANSI_FG_BRIGHT_BLACK "%s:\n    " ANSI_RESET "%s\n",
           node->print().c_str(),
           evaluate(node).print().c_str());
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::AccessDirectionGroup* node)
{
    printError(node, "TODO: AccessDirectionGroup");
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::Alias* node)
{
    printError(node, "TODO: Alias");
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::Array* node)
{
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::Assert* node)
{
    printError(node, "TODO: Assert");
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::Assignment* node)
{
    return assign(node->target, node->expression);
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::ClassDefinition* node)
{
    printError(node, "TODO: ClassDefinition");
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::ClockedConstruct* node)
{
    printError(node, "TODO: ClockedConstruct");
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::Concatenate* node)
{
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::ControlStatement* node)
{
    printError(node, "TODO: ControlStatement");
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::CoverBins* node)
{
    printError(node, "TODO: CoverBins");
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::CoverGroup* node)
{
    printError(node, "TODO: CoverGroup");
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::CycleDelay* node)
{
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::EnumDefinition* node)
{
    printError(node, "TODO: EnumDefinition");
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::Expression* node)
{
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::Fence* node)
{
    // TODO: Fence
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::ForkJoin* node)
{
    printError(node, "TODO: ForkJoin");
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::FunctionCall* node)
{
    if(node->name->type == AST::Type::Identifier &&
       ((AST::Identifier*)node->name)->name == "print"){
        if(node->parameters) print(node->parameters);
        else                 printf("\n");
    }
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::FunctionDef* node)
{
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::GroupDefinition* node)
{
    printError(node, "TODO: GroupDefinition");
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::HdlConstruct* node)
{
    printError(node, "TODO: HdlConstruct");
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::Identifier* node)
{
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::Import* node)
{
    if(node->filename->type != AST::Type::String){
        printError(node, "TODO: Imports by interpolated string");
        return false;
    }

    fs::path filename = ((AST::String*)node->filename)->data + ".alc";

    // If the path is absolute, use it directly
    if(filename.is_absolute()){
        if(!fs::exists(filename)){
            string msg = "Cannot find file ";
            msg += filename.string();
            printError(node, msg.c_str());
            return false;
        }
    }else{
        // otherwise, use the same path as the current file
        fs::path path = fs::path(AST::filenameBuffer[node->filenameIndex]).parent_path();
        filename = path / (((AST::String*)node->filename)->data + ".alc");

        // otherwise, use the path of the top-level module
        if(!fs::exists(filename)){
            path = fs::path(AST::filenameBuffer[0]).parent_path();
            filename = path / (((AST::String*)node->filename)->data + ".alc");

            // otherwise, use the current working directory
            if(!fs::exists(filename)){
                filename = ((AST::String*)node->filename)->data + ".alc";

                // otherwise, error
                if(!fs::exists(filename)){
                    string msg = "Cannot find file ";
                    msg += filename.string();
                    printError(node, msg.c_str());
                    return false;
                }
            }
        }
    }

    Parser parser;
    auto ast = parser.parse(filename.string().c_str());

    if(!ast){
        string msg = "Cannot parse file ";
        msg += filename.string();
        printError(node, msg.c_str());
        return false;
    }
    if(!run(ast)){
        string msg = "Cannot interpret AST of file ";
        msg += filename.string();
        printError(node, msg.c_str());
        delete ast;
        return false;
    }
    delete ast;
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::InterpolatedString* node)
{
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::Jump* node)
{
    printError(node, "TODO: Jump");
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::Literal* node)
{
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::NameSpacePush* node)
{
    printError(node, "TODO: NameSpacePush");
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::OperatorOverload* node)
{
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::ParameterDef* node)
{
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::Repetition* node)
{
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::SequenceDef* node)
{
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::Slice* node)
{
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::StimulusOrEmulate* node)
{
    printError(node, "TODO: StimulusOrEmulate");
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::String* node)
{
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::Stringify* node)
{
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::VariableDef* node)
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

bool Interpreter::execute(const AST::Wait* node)
{
    return false;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::AST* node)
{
    while(!error && node){
        switch(node->type){
            case AST::Type::VariableDef:
                if(!execute((AST::VariableDef*)node)) return false;
                break;

            case AST::Type::FunctionDef:
                if(!execute((AST::FunctionDef*)node)) return false;
                break;

            case AST::Type::OperatorOverload:
                if(!execute((AST::OperatorOverload*)node)) return false;
                break;

            case AST::Type::ClassDefinition:
                if(!execute((AST::ClassDefinition*)node)) return false;
                break;

            case AST::Type::EnumDefinition:
                if(!execute((AST::EnumDefinition*)node)) return false;
                break;

            case AST::Type::Alias:
                if(!execute((AST::Alias*)node)) return false;
                break;

            case AST::Type::Import:
                if(!execute((AST::Import*)node)) return false;
                break;

            case AST::Type::GroupDefinition:
                if(!execute((AST::GroupDefinition*)node)) return false;
                break;

            case AST::Type::AccessDirectionGroup:
                if(!execute((AST::AccessDirectionGroup*)node)) return false;
                break;

            case AST::Type::ControlStatement:
                if(!execute((AST::ControlStatement*)node)) return false;
                break;

            case AST::Type::Jump:
                if(!execute((AST::Jump*)node)) return false;
                break;

            case AST::Type::FunctionCall:
                if(!execute((AST::FunctionCall*)node)) return false;
                break;

            case AST::Type::NameSpacePush:
                if(!execute((AST::NameSpacePush*)node)) return false;
                break;

            case AST::Type::Assignment:
                if(!execute((AST::Assignment*)node)) return false;
                break;

            case AST::Type::ClockedConstruct:
                if(!execute((AST::ClockedConstruct*)node)) return false;
                break;

            case AST::Type::HdlConstruct:
                if(!execute((AST::HdlConstruct*)node)) return false;
                break;

            case AST::Type::StimulusOrEmulate:
                if(!execute((AST::StimulusOrEmulate*)node)) return false;
                break;

            case AST::Type::ForkJoin:
                if(!execute((AST::ForkJoin*)node)) return false;
                break;

            case AST::Type::Assert:
                if(!execute((AST::Assert*)node)) return false;
                break;

            case AST::Type::CoverBins:
                if(!execute((AST::CoverBins*)node)) return false;
                break;

            case AST::Type::CoverGroup:
                if(!execute((AST::CoverGroup*)node)) return false;
                break;

            case AST::Type::Fence:
                if(!execute((AST::Fence*)node)) return false;
                break;

            default:{
                string s = "Unexpected node type: ";
                s += node->decodeType();
                printError(node, s.c_str());
                return false;
            }
        }
        node = node->next;
    }
    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::run(const AST::AST* ast)
{
    if(!execute(ast)) return false;

    if(error) return false;
    return true;
}
//------------------------------------------------------------------------------
