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

#include "PinDefinition.h"

#include "Netlist/Synthesisable/Pin.h"
#include "Netlist/NameSpace/Module.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

PinDefinition::PinDefinition(
    int     line,
    string& filename
): PinDefinition(line, filename.c_str()){}
//------------------------------------------------------------------------------

PinDefinition::PinDefinition(
    int             line,
    const char*     filename
): Definition(line, filename, Type::Pin_Definition){}
//------------------------------------------------------------------------------

PinDefinition::~PinDefinition(){}
//------------------------------------------------------------------------------

Base* PinDefinition::copy()
{
    PinDefinition* copy = new PinDefinition(source.line, source.filename.c_str());

    copyMembers(copy);

    return copy;
}
//------------------------------------------------------------------------------

bool PinDefinition::runAST()
{
    auto identifier = identifiers;

    while(identifier){
        if(!verifyNotDefined(identifier)) return false;

        if(identifier->function){
            error("Not yet implemented");
            identifier = identifier->next;
            continue;
        }

        auto pin = new Netlist::Pin(source.line, source.filename, identifier->identifier.c_str());
        pin->direction = direction;

        if(!pin->applyParameters(parameters)) printError("Invalid parameters");
        if(!pin->applyAttributes(attributes)) printError("Invalid attributes");

        Netlist::nameSpaceStack.front()->symbols[pin->name] = pin;
        if(identifier->initialiser){
            if(!identifier->initialiser->runAST()) return false;
        }

        identifier = identifier->next;
    }
    return true;
}
//------------------------------------------------------------------------------

bool PinDefinition::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void PinDefinition::display()
{
    displayDefinition("pin");
}
//------------------------------------------------------------------------------

void PinDefinition::validateMembers()
{
    assert(type == Type::Pin_Definition);

    Definition::validateMembers();
}
//------------------------------------------------------------------------------

