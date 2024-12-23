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

#include "Byte.h"

#include "AST/expression/Literal.h"
//------------------------------------------------------------------------------

using std::string;
using namespace Netlist;
//------------------------------------------------------------------------------

Netlist::Byte::Byte(int line, const string& filename, const char* name) : Base(line, filename, name, Type::Byte)
{
    value = 0;
}
//------------------------------------------------------------------------------

Netlist::Byte::~Byte(){}
//------------------------------------------------------------------------------

AST::Expression* Netlist::Byte::getExpression(int line, const string& filename)
{
    AST::Literal* result = new AST::Literal(line, filename);
    result->value = value;
    result->setWidth(8);
    return result;
}
//------------------------------------------------------------------------------

bool Netlist::Byte::assign(AST::Expression* expression)
{
    error("Not yet implemented");
    return rawAssign(expression);
}
//------------------------------------------------------------------------------

bool Netlist::Byte::rawAssign(AST::Expression* expression)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool Netlist::Byte::hasCircularReference(Base* object)
{
    if(this == object) return true;
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Netlist::Byte::display(int indent)
{
    debug.indent(indent+1);
    debug.print("Byte: %s\n", name.c_str());
    debug.indent(indent+2);
    debug.print("value = 0x%02X\n", value);
}
//------------------------------------------------------------------------------

void Netlist::Byte::validate()
{
    assert(type == Type::Byte);

    Base::validate();

    error("Not implemented yet");
}
//------------------------------------------------------------------------------

