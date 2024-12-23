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

#include "Definition.h"

#include "Netlist/NameSpace/Module.h"
//------------------------------------------------------------------------------

using namespace AST;
//------------------------------------------------------------------------------

Definition::Array::Array()
{
    size = 0;
    next = 0;
}
//------------------------------------------------------------------------------

Definition::Array::Array(const Array& array)
{
    size = 0;
    next = 0;

    if(array.size) size = (decltype(array.size))array.size->copy();
    if(array.next) next = new Array(*array.next);
}
//------------------------------------------------------------------------------

Definition::Array::~Array()
{
    if(size) delete size;
    if(next) delete next;
}
//------------------------------------------------------------------------------

Definition::Identifier::Identifier()
{
    next        = 0;
    array       = 0;
    initialiser = 0;

    function     = false;
    parameters   = 0;
    functionBody = 0;
}
//------------------------------------------------------------------------------

Definition::Identifier::Identifier(const Identifier& identifier)
{
    next        = 0;
    array       = 0;
    initialiser = 0;

    function     = identifier.function;
    parameters   = 0;
    functionBody = 0;

    if(identifier.next       ) next        = new Identifier(*identifier.next );
    if(identifier.array      ) array       = new Array     (*identifier.array);
    if(identifier.initialiser) initialiser = (decltype(identifier.initialiser))identifier.initialiser->copy();

    functionBody =              copyList(identifier.functionBody);
    parameters   = (Definition*)copyList(identifier.parameters  );
}
//------------------------------------------------------------------------------

Definition::Identifier::~Identifier()
{
    if(next        ) delete next;
    if(array       ) delete array;
    if(parameters  ) delete parameters;
    if(initialiser ) delete initialiser;
    if(functionBody) delete functionBody;
}
//------------------------------------------------------------------------------

Definition::Definition(
    int         line,
    const char* filename,
    Type        definitionType
): Base(line, filename, definitionType)
{
    direction = Direction::Inferred;

    attributes  = 0;
    identifiers = 0;
}
//------------------------------------------------------------------------------

Definition::~Definition()
{
    if(attributes ) delete attributes;
    if(identifiers) delete identifiers;

    for(auto parameter: parameters){
        if(parameter) delete parameter;
    }
}
//------------------------------------------------------------------------------

bool Definition::isDefinition()
{
    return true;
}
//------------------------------------------------------------------------------

void Definition::copyMembers(Definition* copy)
{
    copy->direction = direction;

    if(attributes ) copy->attributes  = (decltype(attributes))attributes->copy();
    if(identifiers) copy->identifiers = new Identifier(*identifiers);

    for(auto parameter: parameters){
        if(parameter) copy->parameters.push_back(parameter->copy());
    }
}
//------------------------------------------------------------------------------

bool Definition::verifyNotDefined(Identifier* identifier)
{
    auto symbol = Netlist::nameSpaceStack.front()->symbols.find(identifier->identifier);
    if(symbol != Netlist::nameSpaceStack.front()->symbols.end()){
        printError();
        printf("symbol \"%s\" already defined in the current namespace\n",
               identifier->identifier.c_str());
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------

void Definition::displayParameters()
{
    debug.print(" direction = ");
    switch(direction){
        case Direction::Input : debug.print("Input\n"   ); break;
        case Direction::Output: debug.print("Output\n"  ); break;
        default               : debug.print("Inferred\n"); break;
    }

    debug.print(" parameters: ");
    if(parameters.empty()){
        debug.print("none / default\n");
    }else{
        for(auto parameter: parameters){
            if(parameter) parameter->display();
            debug.print("\n");
        }
    }
}
//------------------------------------------------------------------------------

void Definition::displayAttributes()
{
    debug.print(" attributes: ");
    if(attributes){
        attributes->display();
        debug.print("\n");
    }
}
//------------------------------------------------------------------------------

void Definition::displayIdentifiers()
{
    debug.print(" identifiers:\n");
    Identifier* identifier = identifiers;
    Array     * array;
    while(identifier){
        debug.print(" - %s", identifier->identifier.c_str());
        array = identifier->array;
        while(array){
            debug.print("[");
            if(array->size) array->size->display();
            debug.print("]");
            array = array->next;
        }

        if(identifier->function){
            debug.print(" -- function:\n  parameters: (\n");
            if(identifier->parameters) identifier->parameters->display();
            debug.print(" )\n  Body:{\n");
            if(identifier->functionBody) identifier->functionBody->display();
            debug.print("  }\n");
        }
        if(identifier->initialiser){
            debug.print(" -- initialiser:");
            identifier->initialiser->display();
        }

        debug.print("\n");
        identifier = identifier->next;
    }
}
//------------------------------------------------------------------------------

void Definition::displayDefinition(const char* type)
{
    displayInfo();
    debug.print("Definition (%s):\n", type);

    displayParameters ();
    displayAttributes ();
    displayIdentifiers();

    if(next) next->display();
}
//------------------------------------------------------------------------------

void Definition::validateMembers()
{
    if(!parameters.empty()){
        for(auto parameter: parameters){
            if(parameter) parameter->validate();
        }
    }

    if(attributes) attributes ->validate();

    Identifier* identifier = identifiers;
    Array     * array;

    while(identifier){
        array = identifier->array;
        while(array){
            if(array->size) array->size->validate();
            array = array->next;
        }

        if(identifier->function){
            assert(identifier              == identifiers);
            assert(identifier->next        == 0);
            assert(identifier->initialiser == 0);

            if(identifier->parameters  ) identifier->parameters  ->validate();
            if(identifier->functionBody) identifier->functionBody->validate();

        }else{
            assert(identifier->parameters   == 0);
            assert(identifier->functionBody == 0);

            if(identifier->initialiser) identifier->initialiser ->validate();
        }

        identifier = identifier->next;
    }
}
//------------------------------------------------------------------------------

