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

#include "ClassDefinition.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

ClassDefinition::Parent::Parent()
{
    className  = 0;
    next       = 0;
}
//------------------------------------------------------------------------------

ClassDefinition::Parent::Parent(const Parent& parent)
{
    className  = 0;
    next       = 0;

    if(parent.className) className  = (decltype(parent.className))parent.className->copy();
    if(parent.next     ) next       = new Parent(*parent.next);

    for(auto parameter: parent.parameters){
        if(parameter) parameters.push_back(parameter->copy());
    }
}
//------------------------------------------------------------------------------

ClassDefinition::Parent::~Parent()
{
    if(className ) delete className;
    if(next      ) delete next;

    for(auto parameter: parameters){
        if(parameter) delete parameter;
    }
}
//------------------------------------------------------------------------------

ClassDefinition::ClassDefinition(int line, std::string& filename): ClassDefinition(line, filename.c_str()){}
//------------------------------------------------------------------------------

ClassDefinition::ClassDefinition(int line, const char* filename): Base(line, filename, Type::Class_Definition)
{
    attributes = 0;
    parameters = 0;

    parents = 0;

    body = 0;
}
//------------------------------------------------------------------------------

ClassDefinition::~ClassDefinition()
{
    if(body      ) delete body;
    if(parents   ) delete parents;
    if(attributes) delete attributes;
    if(parameters) delete parameters;
}
//------------------------------------------------------------------------------

Base* ClassDefinition::copy()
{
    ClassDefinition* copy = new ClassDefinition(source.line, source.filename.c_str());

    copy->identifier = identifier;

    if(parents) copy->parents = new Parent(*parents);

    copy->body       = (decltype(body      ))copyList(body);
    copy->attributes = (decltype(attributes))copyList(attributes);
    copy->parameters = (decltype(parameters))copyList(parameters);

    return copy;
}
//------------------------------------------------------------------------------

bool ClassDefinition::runAST()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool ClassDefinition::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void ClassDefinition::display()
{
    displayInfo();
    debug.print("Class Definition (%s):\n", identifier.c_str());

    if(attributes){
        debug.print(" Attributes:\n"); attributes->display(); debug.print("\n");
    }
    if(parameters){
        debug.print(" Parameters:\n"); parameters->display(); debug.print("\n");
    }

    if(parents) debug.print(" Parents:\n");
    Parent* parent = parents;
    while(parent){
        debug.print(" - ");
        if(parent->className) parent->className->display();
        debug.print("(");
        bool isFirst = true;
        for(auto parameter: parent->parameters){
            if(isFirst) debug.print(", ");
            isFirst = false;
            parameter->display();
        }
        debug.print(")\n");
        parent = parent->next;
    }

    debug.print(" {\n");
    if(body) body->display();
    debug.print(" }\n");

    if(next) next->display();
}
//------------------------------------------------------------------------------

void ClassDefinition::validateMembers()
{
    assert(type == Type::Class_Definition);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

