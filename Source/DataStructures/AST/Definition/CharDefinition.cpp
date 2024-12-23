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

#include "CharDefinition.h"

#include "Netlist/Character.h"
#include "Netlist/NameSpace/Module.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

CharDefinition::CharDefinition(
    int     line,
    string& filename
): CharDefinition(line, filename.c_str()){}
//------------------------------------------------------------------------------

CharDefinition::CharDefinition(
    int             line,
    const char*     filename
): Definition(line, filename, Type::Char_Definition){}
//------------------------------------------------------------------------------

CharDefinition::~CharDefinition(){}
//------------------------------------------------------------------------------

Base* CharDefinition::copy()
{
    CharDefinition* copy = new CharDefinition(source.line, source.filename.c_str());

    copyMembers(copy);

    return copy;
}
//------------------------------------------------------------------------------

bool CharDefinition::runAST()
{
    auto identifier = identifiers;

    while(identifier){
        if(!verifyNotDefined(identifier)) return false;

        if(identifier->function){
            error("Not yet implemented");
            identifier = identifier->next;
            continue;
        }

        auto character = new Netlist::Character(source.line, source.filename, identifier->identifier.c_str());
        if(!character->applyAttributes(attributes)) printError("Invalid attributes");
        Netlist::nameSpaceStack.front()->symbols[character->name] = character;
        if(identifier->initialiser){
            if(!identifier->initialiser->runAST()) return false;
        }

        identifier = identifier->next;
    }
    return true;
}
//------------------------------------------------------------------------------

bool CharDefinition::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void CharDefinition::display()
{
    displayDefinition("Character");
}
//------------------------------------------------------------------------------

void CharDefinition::validateMembers()
{
    assert(type == Type::Char_Definition);

    Definition::validateMembers();
}
//------------------------------------------------------------------------------

