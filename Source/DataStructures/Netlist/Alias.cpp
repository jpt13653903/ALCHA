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
//------------------------------------------------------------------------------

using std::string;
using namespace Netlist;
//------------------------------------------------------------------------------

Alias::Alias(int line, const string& filename, const char* name, AST::Expression* expression): Base(line, filename, name, Type::Alias)
{
    this->expression = expression;
}
//------------------------------------------------------------------------------

Alias::~Alias()
{
    if(expression) delete expression;
}
//------------------------------------------------------------------------------

AST::Expression* Alias::getExpression(int line, const string& filename)
{
    assert(expression, return 0);
    return (AST::Expression*)expression->copy();
}
//------------------------------------------------------------------------------

bool Alias::assign(AST::Expression* expression)
{
    return rawAssign(expression);
}
//------------------------------------------------------------------------------

bool Alias::rawAssign(AST::Expression* expression)
{
    expression->printError("Cannot assign to an alias");
    return false;
}
//------------------------------------------------------------------------------

bool Alias::hasCircularReference(Base* object)
{
    if(this == object) return true;
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Alias::display(int indent)
{
    debug.indent(indent);
    debug.print("Alias: %s\n", name.c_str());

    debug.indent(indent+1);
    if(expression) expression->display();
    else           debug.print("{null}");
    debug.print("\n");
}
//------------------------------------------------------------------------------

void Alias::validate()
{
    assert(type == Type::Alias);

    Base::validate();

    if(expression) expression->validate();
}
//------------------------------------------------------------------------------

