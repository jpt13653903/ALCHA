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
    clearTempStack();
}
//------------------------------------------------------------------------------

void Interpreter::clearTempStack()
{
    for(auto& object: tempStack){
        delete object;
    }
    tempStack.clear();
}
//------------------------------------------------------------------------------

void Interpreter::printError(const AST::AST* node, const char* message)
{
    if(error) return;
    error = true;

    if(!node) return;

    ::printError(node->line, AST::filenameBuffer[node->filenameIndex], message);
    printf(
        ANSI_FG_CYAN "  node: " ANSI_FG_BRIGHT_MAGENTA "%s\n"
        ANSI_RESET   "%s\n",
        node->decodeType(),
        node->print(1).c_str()
    );
}
//------------------------------------------------------------------------------

const tm* Interpreter::time() const
{
    time_t timer;
    ::time(&timer);
    return localtime(&timer);
}
//------------------------------------------------------------------------------

Objects::Object* Interpreter::evaluateInfix(const AST::Expression* node,
                                            const Objects::Object* left,
                                            const Objects::Object* right)
{
    if(!left ) return 0;
    if(!right) return 0;

    switch(left->type){
        case Objects::Type::Num:
            switch(right->type){
                case Objects::Type::Num:
                    return evaluateInfix(node, (Objects::Num*)left, (Objects::Num*)right);

                default:
                    printError(node, "Invalid right-hand operand type for infix operation");
            }

        default:
            printError(node, "Invalid left-hand operand type for infix operation");
    }
    return 0;
}
//------------------------------------------------------------------------------

Objects::Object* Interpreter::evaluateInfix(const AST::Expression* node,
                                            const Objects::Num* left,
                                            const Objects::Num* right)
{
    auto result = new Objects::Num;
    tempStack.push_back(result);

    switch(node->operation){
        case Token::Type::TernaryIf:
            printError(node, "TODO: TernaryIf");
            break;

        case Token::Type::Elvis:
            printError(node, "TODO: Elvis");
            break;

        case Token::Type::BitOr:
            printError(node, "TODO: BitOr");
            break;

        case Token::Type::BitNor:
            printError(node, "TODO: BitNor");
            break;

        case Token::Type::BitXor:
            printError(node, "TODO: BitXor");
            break;

        case Token::Type::BitXnor:
            printError(node, "TODO: BitXnor");
            break;

        case Token::Type::BitAnd:
            printError(node, "TODO: BitAnd");
            break;

        case Token::Type::BitNand:
            printError(node, "TODO: BitNand");
            break;

        case Token::Type::Equal:
            printError(node, "TODO: Equal");
            break;

        case Token::Type::NotEqual:
            printError(node, "TODO: NotEqual");
            break;

        case Token::Type::Less:
            printError(node, "TODO: Less");
            break;

        case Token::Type::Greater:
            printError(node, "TODO: Greater");
            break;

        case Token::Type::LessEqual:
            printError(node, "TODO: LessEqual");
            break;

        case Token::Type::GreaterEqual:
            printError(node, "TODO: GreaterEqual");
            break;

        case Token::Type::ShiftLeft:
            printError(node, "TODO: ShiftLeft");
            break;

        case Token::Type::ShiftRight:
            printError(node, "TODO: ShiftRight");
            break;

        case Token::Type::Add:
            result->value = left->value + right->value;
            return result;

        case Token::Type::Subtract:
            result->value = left->value - right->value;
            return result;

        case Token::Type::Multiply:
            result->value = left->value * right->value;
            return result;

        case Token::Type::Divide:
            result->value = left->value / right->value;
            return result;

        case Token::Type::Modulus:
            printError(node, "TODO: Modulus");
            break;

        case Token::Type::Exponential:
            printError(node, "TODO: Exponential");
            break;

        case Token::Type::Factorial:
            printError(node, "TODO: Factorial");
            break;

        default:
            printError(node, "TODO: Unsupported expression operation");
            break;
    }
    return 0;
}
//------------------------------------------------------------------------------

Objects::Object* Interpreter::evaluatePrefix(const AST::Expression* node,
                                             const Objects::Object* right)
{
    if(!right) return 0;

    switch(right->type){
        case Objects::Type::Num:
            return evaluatePrefix(node, (Objects::Num*)right);

        default:
            printError(node, "Invalid operand type for prefix operation");
    }
    return 0;
}
//------------------------------------------------------------------------------

Objects::Object* Interpreter::evaluatePrefix(const AST::Expression* node,
                                             const Objects::Num* right)
{
    auto result = new Objects::Num;
    tempStack.push_back(result);

    switch(node->operation){
        case Token::Type::AndReduce:
            printError(node, "TODO: AndReduce");
            break;

        case Token::Type::NandReduce:
            printError(node, "TODO: NandReduce");
            break;

        case Token::Type::OrReduce:
            printError(node, "TODO: OrReduce");
            break;

        case Token::Type::NorReduce:
            printError(node, "TODO: NorReduce");
            break;

        case Token::Type::XorReduce:
            printError(node, "TODO: XorReduce");
            break;

        case Token::Type::XnorReduce:
            printError(node, "TODO: XnorReduce");
            break;

        case Token::Type::LogicalNot:
            printError(node, "TODO: LogicalNot");
            break;

        default:{
            printError(node, "TODO: Unsupported prefix expression operation");
            break;
        }
    }
    delete result;
    return 0;
}
//------------------------------------------------------------------------------

Objects::Object* Interpreter::evaluatePostfix(const AST::Expression* node,
                                              const Objects::Object* left)
{
    if(!left) return 0;

    switch(left->type){
        case Objects::Type::Num:
            return evaluatePostfix(node, (Objects::Num*)left);

        default:
            printError(node, "Invalid operand type for postfix operation");
    }
    return 0;
}
//------------------------------------------------------------------------------

Objects::Object* Interpreter::evaluatePostfix(const AST::Expression* node,
                                              const Objects::Num* left)
{
    auto result = new Objects::Num;
    tempStack.push_back(result);

    switch(node->operation){
        case Token::Type::Increment:
            printError(node, "TODO: Increment");
            break;

        case Token::Type::Decrement:
            printError(node, "TODO: Decrement");
            break;

        case Token::Type::Factorial:
            printError(node, "TODO: Factorial");
            break;

        default:
            printError(node, "TODO: Unsupported postfix expression operation");
            break;
    }
    delete result;
    return 0;
}
//------------------------------------------------------------------------------

Objects::Object* Interpreter::evaluate(const AST::AST* node)
{
    switch(node->type){
        case AST::Type::Literal:    return evaluate((AST::Literal   *)node);
        case AST::Type::Expression: return evaluate((AST::Expression*)node);
        case AST::Type::Identifier: return evaluate((AST::Identifier*)node);
        case AST::Type::String:     return evaluate((AST::String    *)node);
        default:
            printError(node, "Unsupported AST type for evaluation");
            return 0;
    }
}
//------------------------------------------------------------------------------

Objects::Object* Interpreter::evaluate(const AST::Array* node)
{
    printError(node, "TODO: Evaluate Array");
    return 0;
}
//------------------------------------------------------------------------------

Objects::Object* Interpreter::evaluate(const AST::Concatenate* node)
{
    printError(node, "TODO: Evaluate Concatenate");
    return 0;
}
//------------------------------------------------------------------------------

Objects::Object* Interpreter::evaluate(const AST::Expression* node)
{
    Number result;

    if(node->left && node->right){
        return evaluateInfix(node, evaluate(node->left), evaluate(node->right));

    }else if(node->right){
        return evaluatePrefix(node, evaluate(node->right));

    }else if(node->left){
        return evaluatePostfix(node, evaluate(node->left));

    }else{
        printError(node, "Expression has no operands");
        return 0;
    }
    return 0;
}
//------------------------------------------------------------------------------

Objects::Object* Interpreter::evaluate(const AST::Identifier* node)
{
    auto object = global.symbols.find(node->name);

    if(object == global.symbols.end()){
        auto result = new Objects::Num;
        if(node->name == "pi" || node->name == "π"){
            result->value.set_pi();
        }else if(node->name == "e"){
            result->value.set_e();
        }else if(node->name == "i"){
            result->value.set_i();
        }else if(node->name == "j"){
            result->value.set_i();
        }else if(node->name == "__YEAR__"){
            result->value = time()->tm_year+1900;
        }else if(node->name == "__MONTH__"){
            result->value = time()->tm_mon+1;
        }else if(node->name == "__DAY__"){
            result->value = time()->tm_mday;
        }else if(node->name == "__HOUR__"){
            result->value = time()->tm_hour;
        }else if(node->name == "__MINUTE__"){
            result->value = time()->tm_min;
        }else if(node->name == "__SECOND__"){
            result->value = time()->tm_sec;
        }else if(node->name == "__WEEKDAY__"){
            result->value = ((time()->tm_wday+6) % 7) + 1;
        }else if(node->name == "__YEARDAY__"){
            result->value = time()->tm_yday+1;
        }else{
            printError(node, "Undefined symbol");
            delete result;
            return 0;
        }
        tempStack.push_back(result);
        return result;
    }
    return object->second;
}
//------------------------------------------------------------------------------

Objects::Object* Interpreter::evaluate(const AST::InterpolatedString* node)
{
    printError(node, "TODO: Evaluate Interpolated String");
    return 0;
}
//------------------------------------------------------------------------------

Objects::Object* Interpreter::evaluate(const AST::Literal* node)
{
    auto result = new Objects::Num;
    tempStack.push_back(result);
    result->value = node->value;
    return result;
}
//------------------------------------------------------------------------------

Objects::Object* Interpreter::evaluate(const AST::Repetition* node)
{
    printError(node, "TODO: Evaluate Repetition");
    return 0;
}
//------------------------------------------------------------------------------

Objects::Object* Interpreter::evaluate(const AST::Slice* node)
{
    printError(node, "TODO: Evaluate Slice");
    return 0;
}
//------------------------------------------------------------------------------

Objects::Object* Interpreter::evaluate(const AST::String* node)
{
    auto result = new Objects::String;
    tempStack.push_back(result);
    result->data = node->data;
    return result;
}
//------------------------------------------------------------------------------

Objects::Object* Interpreter::evaluate(const AST::Stringify* node)
{
    printError(node, "TODO: Evaluate Stringify");
    return 0;
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
        case Objects::Type::Num: return assign((Objects::Num*)object, expression);

        default:
            printError(target, "Unsupported execute target type");
            return false;
    }
}
//------------------------------------------------------------------------------

bool Interpreter::assign(Objects::Num* target, const AST::AST* expression)
{
    auto result = evaluate(expression);
    if(!result) return false;

    switch(result->type){
        case Objects::Type::Num:
            target->value = ((Objects::Num*)result)->value;
            return true;

        default:
            printError(expression, "Invalid type for assignment to num");
            return false;
    }
    return !error;
}
//------------------------------------------------------------------------------

bool Interpreter::print(const AST::AST* node)
{
    auto object = evaluate(node);
    if(!object) return false;

    printf(ANSI_FG_BRIGHT_BLACK "%s:\n    " ANSI_RESET "%s\n",
           node  ->print().c_str(),
           object->print().c_str());

    return true;
}
//------------------------------------------------------------------------------

bool Interpreter::execute(const AST::AST* node)
{
    while(!error && node){
        clearTempStack();

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
    return !error;
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
    if(node->name->type == AST::Type::Identifier){
        auto name = ((AST::Identifier*)node->name)->name;
        if(name == "print"){
            if(node->parameters){
                return print(node->parameters);
            }else{
                printf("\n");
                return true;
            }
        }else{
            printError(node, "TODO: Function calls");
        }
    }else{
        printError(node, "TODO: Function calls with name resolution");
    }
    return false;
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

bool Interpreter::execute(const AST::Jump* node)
{
    printError(node, "TODO: Jump");
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

bool Interpreter::execute(const AST::SequenceDef* node)
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
                auto object = new Objects::Num;
                global.symbols[def->name] = object;
                if(node->parameters){
                    printError(node, "TODO Definition with parameters");
                }
                if(node->attributes){
                    printError(node, "TODO Definition with attributes");
                }
                if(def->initialiser){
                    auto result = evaluate(def->initialiser);
                    if(!result) return false;
                    switch(result->type){
                        case Objects::Type::Num:
                            object->value = ((Objects::Num*)result)->value;
                            return true;

                        default:
                            printError(node, "Invalid type for num initialiser");
                            return false;
                    }
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

bool Interpreter::run(const AST::AST* ast)
{
    if(!execute(ast)) return false;

    if(error) return false;
    return true;
}
//------------------------------------------------------------------------------
