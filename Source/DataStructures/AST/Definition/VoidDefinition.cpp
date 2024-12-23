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

#include "VoidDefinition.h"

#include "Netlist/Namespace/Module.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

VoidDefinition::VoidDefinition(
    int     line,
    string& filename
): VoidDefinition(line, filename.c_str()){}
//------------------------------------------------------------------------------

VoidDefinition::VoidDefinition(
    int             line,
    const char*     filename
): Definition(line, filename, Type::Void_Definition){}
//------------------------------------------------------------------------------

VoidDefinition::~VoidDefinition(){}
//------------------------------------------------------------------------------

Base* VoidDefinition::copy()
{
    VoidDefinition* copy = new VoidDefinition(source.line, source.filename.c_str());

    copyMembers(copy);

    return copy;
}
//------------------------------------------------------------------------------

bool VoidDefinition::runAST()
{
    auto identifier = identifiers;

    while(identifier){
        if(!verifyNotDefined(identifier)) return false;

        if(identifier->function){
            error("Not yet implemented");
            identifier = identifier->next;
            continue;
        }

        error("Not yet implemented");

        identifier = identifier->next;
    }
    return true;
}
//------------------------------------------------------------------------------

bool VoidDefinition::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void VoidDefinition::display()
{
    displayDefinition("Void");
}
//------------------------------------------------------------------------------

void VoidDefinition::validateMembers()
{
    assert(type == Type::Void_Definition);

    assert(parameters.empty());

    Definition::validateMembers();
}
//------------------------------------------------------------------------------

