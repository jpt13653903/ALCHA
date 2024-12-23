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

#include "NumDefinition.h"

#include "Netlist/NameSpace/Module.h"
#include "Netlist/Num.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

NumDefinition::NumDefinition(
    int     line,
    string& filename
): NumDefinition(line, filename.c_str()){}
//------------------------------------------------------------------------------

NumDefinition::NumDefinition(
    int             line,
    const char*     filename
): Definition(line, filename, Type::Num_Definition){}
//------------------------------------------------------------------------------

NumDefinition::~NumDefinition(){}
//------------------------------------------------------------------------------

Base* NumDefinition::copy()
{
    NumDefinition* copy = new NumDefinition(source.line, source.filename.c_str());

    copyMembers(copy);

    return copy;
}
//------------------------------------------------------------------------------

bool NumDefinition::runAST()
{
    auto identifier = identifiers;

    while(identifier){
        if(!verifyNotDefined(identifier)) return false;

        if(identifier->function){
            error("Not yet implemented");
            identifier = identifier->next;
            continue;
        }

        auto number = new Netlist::Num(source.line, source.filename, identifier->identifier.c_str());
        if(!number->applyAttributes(attributes)) printError("Invalid attributes");
        Netlist::nameSpaceStack.front()->symbols[number->name] = number;
        if(identifier->initialiser){
            if(!identifier->initialiser->runAST()) return false;
        }

        identifier = identifier->next;
    }
    return true;
}
//------------------------------------------------------------------------------

bool NumDefinition::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void NumDefinition::display()
{
    displayDefinition("number");
}
//------------------------------------------------------------------------------

void NumDefinition::validateMembers()
{
    assert(type == Type::Num_Definition);

    Definition::validateMembers();
}
//------------------------------------------------------------------------------

