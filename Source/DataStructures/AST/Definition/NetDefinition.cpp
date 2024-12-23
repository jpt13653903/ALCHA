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

#include "NetDefinition.h"

#include "Netlist/NameSpace/Module.h"
#include "Netlist/Synthesisable/Net.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

NetDefinition::NetDefinition(
    int     line,
    string& filename
): NetDefinition(line, filename.c_str()){}
//------------------------------------------------------------------------------

NetDefinition::NetDefinition(
    int             line,
    const char*     filename
): Definition(line, filename, Type::NetDefinition){}
//------------------------------------------------------------------------------

NetDefinition::~NetDefinition(){}
//------------------------------------------------------------------------------

Base* NetDefinition::copy()
{
    NetDefinition* copy = new NetDefinition(source.line, source.filename.c_str());

    copyMembers(copy);

    return copy;
}
//------------------------------------------------------------------------------

bool NetDefinition::runAST()
{
    auto identifier = identifiers;

    while(identifier){
        if(!verifyNotDefined(identifier)) return false;

        if(identifier->function){
            error("Not yet implemented");
            identifier = identifier->next;
            continue;
        }

        auto net = new Netlist::Net(source.line, source.filename, identifier->identifier.c_str());
        net->direction = direction;
        if(!net->applyParameters(parameters)) printError("Invalid parameters");
        if(!net->applyAttributes(attributes)) printError("Invalid attributes");
        Netlist::nameSpaceStack.front()->symbols[net->name] = net;
        if(identifier->initialiser){
            if(!identifier->initialiser->runAST()) return false;
        }

        identifier = identifier->next;
    }
    return true;
}
//------------------------------------------------------------------------------

bool NetDefinition::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void NetDefinition::display()
{
    displayDefinition("net");
}
//------------------------------------------------------------------------------

void NetDefinition::validateMembers()
{
    assert(type == Type::NetDefinition);

    Definition::validateMembers();
}
//------------------------------------------------------------------------------

