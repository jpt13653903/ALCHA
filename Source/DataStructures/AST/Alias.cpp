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

#include "Alias.h"
#include "Netlist/Alias.h"
#include "Netlist/NameSpace/Module.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Alias::Alias(int line, string& filename): Alias(line, filename.c_str()){}
//------------------------------------------------------------------------------

Alias::Alias(int line, const char* filename): Base(line, filename, Type::Alias)
{
    expression = 0;
}
//------------------------------------------------------------------------------

Alias::~Alias()
{
    if(expression) delete expression;
}
//------------------------------------------------------------------------------

Base* Alias::copy()
{
    Alias* copy = new Alias(source.line, source.filename.c_str());

    copy->identifier = identifier;

    if(expression) copy->expression = (decltype(expression))expression->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool Alias::runAST()
{
    auto symbol = Netlist::nameSpaceStack.front()->symbols.find(identifier);
    if(symbol != Netlist::nameSpaceStack.front()->symbols.end()){
        printError();
        printf("Symbol \"%s\" already defined in the current namespace\n",
                      identifier.c_str());
        return false;
    }

    auto object = new Netlist::Alias(source.line, source.filename, identifier.c_str(), expression);
    expression = 0; // It's been moved to object, so remove it from this node.

    Netlist::nameSpaceStack.front()->symbols[object->name] = object;

    return true;
}
//------------------------------------------------------------------------------

bool Alias::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Alias::display()
{
    displayInfo();
    logger.print("alias (%s):\n", identifier.c_str());

    if(expression) expression->display();
    else           logger.print("{Moved expression}\n");

    if(next) next->display();
}
//------------------------------------------------------------------------------

void Alias::validateMembers()
{
    assert(type == Type::Alias);
    if(expression) expression->validate();
}
//------------------------------------------------------------------------------

