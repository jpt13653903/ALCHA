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

#include "Group.h"
#include "Netlist/Namespace/Group.h"
#include "Netlist/Namespace/Module.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

AST::Group::Group(int line, std::string& filename): AST::Group(line, filename.c_str()){}
//------------------------------------------------------------------------------

AST::Group::Group(int line, const char* filename): Base(line, filename, Type::Group)
{
    attributes = 0;
    body       = 0;
}
//------------------------------------------------------------------------------

AST::Group::~Group()
{
    if(attributes) delete attributes;
    if(body      ) delete body;
}
//------------------------------------------------------------------------------

Base* AST::Group::copy()
{
    Group* copy = new AST::Group(source.line, source.filename.c_str());

    copy->identifier = identifier;

    if(attributes) copy->attributes = (decltype(attributes))attributes->copy();
    if(body      ) copy->body       = (decltype(body      ))body      ->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool AST::Group::runAST()
{
    if(identifier.empty()){
        error("Anonymous groups not supported yet");
        return false;
    }

    auto found = Netlist::nameSpaceStack.front()->symbols.find(identifier);
    if(found != Netlist::nameSpaceStack.front()->symbols.end()){
        printError();
        printf("Symbol \"%s\" already exists in the current namespace\n",
               identifier.c_str());
        return false;
    }
    auto object = new Netlist::Group(source.line, source.filename, identifier.c_str());
    object->applyAttributes(attributes);
    Netlist::nameSpaceStack.front()->symbols[identifier] = object;
    Netlist::nameSpaceStack.push_front(object);

    bool result  = true;
    auto element = body;
    while(result && element){
        result  = element->runAST();
        element = element->next;
    }

    Netlist::nameSpaceStack.pop_front();
    return result;
}
//------------------------------------------------------------------------------

bool AST::Group::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void AST::Group::display()
{
    displayInfo();

    if(identifier.empty()) debug.print("Group:\n");
    else                   debug.print("Group (%s):\n", identifier.c_str());

    debug.print(" attributes: ");
    if(attributes){
        attributes->display();
        debug.print("\n");
    }

    debug.print(" {\n");
    if(body) body->display();
    debug.print(" }\n");

    if(next) next->display();
}
//------------------------------------------------------------------------------

void AST::Group::validateMembers()
{
    assert(type == Type::Group);

    if(attributes) attributes->validate();
    if(body      ) body      ->validate();
}
//------------------------------------------------------------------------------

