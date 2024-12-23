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

#include "Assignment.h"

#include "Netlist/Alias.h"
#include "Netlist/attribute.h"
#include "Netlist/Synthesisable/Pin.h"
#include "Netlist/Synthesisable/Net.h"
#include "Netlist/Namespace/Module.h"

#include "Expression/Object.h"
#include "Expression/Identifier.h"
//------------------------------------------------------------------------------

using namespace AST;
//------------------------------------------------------------------------------

Assignment::Assignment(
    int             line,
    const char*     filename,
    Type            AssignmentType
): Base(line, filename, AssignmentType)
{
    left = right = 0;
}
//------------------------------------------------------------------------------

Assignment::~Assignment()
{
    if(left ) delete left;
    if(right) delete right;
}
//------------------------------------------------------------------------------

bool Assignment::isAssignment()
{
    return true;
}
//------------------------------------------------------------------------------

bool Assignment::addLhsObject(Netlist::Base* object, TargetList& list)
{
    bool result = false;

    if(object){
        switch(object->type){
            case Netlist::Base::Type::Pin:
            case Netlist::Base::Type::PinComponent:
            case Netlist::Base::Type::Net:
            case Netlist::Base::Type::Number:
            case Netlist::Base::Type::Byte:
            case Netlist::Base::Type::Character:
            case Netlist::Base::Type::Module:
            case Netlist::Base::Type::Group:{
                list.push_back(object);
                result = true;
                break;
            }
            case Netlist::Base::Type::Alias:{
                auto alias = (Netlist::Alias*)object;
                Netlist::nameSpaceStack.push_front(alias->nameSpace);
                    result = getLHS(alias->expression, list);
                Netlist::nameSpaceStack.pop_front();
                break;
            }
            case Netlist::Base::Type::Array:{
                error("Array assignment not yet implemented");
                break;
            }
            default:
                error("Unknown object type %d", (int)object->type);
                break;
        }
    }
    return result;
}
//------------------------------------------------------------------------------

bool Assignment::getLHS(Expression* node, TargetList& list)
{
    if(!node) return 0;

    bool result = false;

    switch(node->type){
        case Type::Array:{
            error("Array not yet implemented");
            // Idea: Simply call getLHS recursively for each array element
            //
            // auto Element = (expression*)node->right;
            // while(Element){
            //   result->Elements.push_back(Evaluate(Element));
            //   Element = (expression*)Element->next;
            // }
            break;
        }

        case Type::Identifier:{
            auto identifier = (Identifier*)node;
            for(auto nameSpace: Netlist::nameSpaceStack){
                while(!result && nameSpace){
                    auto object = nameSpace->getMember(identifier->name);
                    if(object){
                        result = addLhsObject(object, list);
                    }
                    nameSpace = nameSpace->nameSpace;
                }
                if(result) break;
            }
            if(!result){
                node->printError();
                printf("Undefined identifier: \"%s\"\n", identifier->name.c_str());
            }
            break;
        }

        case Type::VectorConcatenate:{
            error("VectorConcatenate not yet implemented");
            // result = (Expression*)node->copy(true);
            break;
        }

        case Type::ArrayConcatenate:{
            error("ArrayConcatenate not yet implemented");
            // result = (Expression*)node->copy(true);
            break;
        }

        case Type::Slice:
            error("Slice not yet implemented");
            break;

        case Type::AccessMember:{
            assert(node->left , return false);
            assert(node->right, return false);

            TargetList leftList;
            if(!getLHS(node->left, leftList)) return false;
            assert(!leftList.empty(), return false);
            if(leftList.size() > 1){
                error("Multiple assignment targets not yet supported");
                return false;
            }
            auto left  = leftList.front();
            auto right = node->right;

            assert(right->type == Type::Identifier, return false);
            auto object = left->getMember(((Identifier*)right)->name);
            if(!object){
                node->printError();
                printf("object %s not a member of %s\n",
                              ((Identifier*)right)->name.c_str(), left->name.c_str());
                return false;
            }
            result = addLhsObject(object, list);

            break;
        }

        case Type::AccessMemberSafe:
            error("AccessMemberSafe not yet implemented");
            break;

        case Type::AccessAttribute:{
            assert(node->right, return false);

            Netlist::Base* left = 0;
            if(node->left){
                TargetList leftList;
                if(!getLHS(node->left, leftList)) return false;
                assert(!leftList.empty(), return false);
                if(leftList.size() > 1){
                    error("Multiple assignment targets not yet supported");
                    return false;
                }
                left = leftList.front();
            }else{ // An attribute of the current namespace
                left = Netlist::nameSpaceStack.front();
            }
            if(left->type == Netlist::Base::Type::Attribute){
                node->printError("attributes are not hierarchical");
                return false;
            }
            if(left->type == Netlist::Base::Type::PinComponent){
                node->printError("Cannot assign attributes to pin components.  Assign to the pin directly");
                return false;
            }

            auto right = node->right;
            if(right->type == Type::Identifier){
                // The process of adding an entry initialises the pointer to null.
                // The default constructor of the pointer type is called.
                auto attribute = left->getAttribute(((Identifier*)right)->name);
                if(!attribute){ // Create a new attribute
                    auto attrib = new Netlist::Attribute(right->source.line,
                                                         right->source.filename,
                                                         ((Identifier*)right)->name.c_str());
                    left->attributes[((Identifier*)right)->name] = attrib;
                    attribute = attrib;
                }
                list.push_back(attribute);
                result = true;
            }else{
                // TODO Could be a slice expression, which is not supported yet
                error("Unimplemented attribute access expression");
            }
            break;
        }

        default:
            node->printError("Invalid LHS expression");
            break;
    }
    return result;
}
//------------------------------------------------------------------------------

void Assignment::displayAssignment(const char* theOperator)
{
    displayInfo();
    debug.print("Assignment: ");

    if(left){
        if(left->left || left->right) debug.print("(");
        left->display();
        if(left->left || left->right) debug.print(")");
    }

    debug.print(" %s ", theOperator);

    if(right){
        if(right->left || right->right) debug.print("(");
        right->display();
        if(right->left || right->right) debug.print(")");
    }else{
        debug.print("{Moved expression}");
    }

    debug.print("\n");
    if(next) next->display();
}
//------------------------------------------------------------------------------

