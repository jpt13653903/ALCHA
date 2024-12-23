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

#include "ClassInstance.h"

#include "Netlist/Namespace/Module.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

ClassInstance::ClassInstance(
    int     line,
    string& filename
): ClassInstance(line, filename.c_str()){}
//------------------------------------------------------------------------------

ClassInstance::ClassInstance(
    int             line,
    const char*     filename
): Definition(line, filename, Type::Class_Instance)
{
    className = 0;
}
//------------------------------------------------------------------------------

ClassInstance::~ClassInstance()
{
    if(className) delete className;
}
//------------------------------------------------------------------------------

Base* ClassInstance::copy()
{
    ClassInstance* copy = new ClassInstance(source.line, source.filename.c_str());

    copyMembers(copy);

    if(className) copy->className = (decltype(className))className->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool ClassInstance::runAST()
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

bool ClassInstance::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void ClassInstance::display()
{
    displayInfo();
    debug.print("Definition (");

    debug.print("Class instance definition (");
    if(className) className->display();
    else          debug.print("Class instance with no class name");
    debug.print("):\n");

    displayParameters ();
    displayAttributes ();
    displayIdentifiers();

    if(next) next->display();
}
//------------------------------------------------------------------------------

void ClassInstance::validateMembers()
{
    assert(type == Type::Class_Instance);

    if(className) className->validate();

    Definition::validateMembers();
}
//------------------------------------------------------------------------------

