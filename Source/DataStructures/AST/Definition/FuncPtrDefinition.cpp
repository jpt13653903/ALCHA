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

#include "FuncPtrDefinition.h"

#include "Netlist/NameSpace/Module.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

FuncPtrDefinition::FuncPtrDefinition(
    int     line,
    string& filename
): FuncPtrDefinition(line, filename.c_str()){}
//------------------------------------------------------------------------------

FuncPtrDefinition::FuncPtrDefinition(
    int             line,
    const char*     filename
): Definition(line, filename, Type::FuncPtr_Definition){}
//------------------------------------------------------------------------------

FuncPtrDefinition::~FuncPtrDefinition(){}
//------------------------------------------------------------------------------

Base* FuncPtrDefinition::copy()
{
    FuncPtrDefinition* copy = new FuncPtrDefinition(source.line, source.filename.c_str());

    copyMembers(copy);

    return copy;
}
//------------------------------------------------------------------------------

bool FuncPtrDefinition::runAST()
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

bool FuncPtrDefinition::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void FuncPtrDefinition::display()
{
    displayDefinition("function");
}
//------------------------------------------------------------------------------

void FuncPtrDefinition::validateMembers()
{
    assert(type == Type::FuncPtr_Definition);

    assert(parameters.empty());

    Definition::validateMembers();
}
//------------------------------------------------------------------------------

