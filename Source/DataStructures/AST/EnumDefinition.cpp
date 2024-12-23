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

#include "EnumDefinition.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

EnumDefinition::Value::Value()
{
    next = 0;
}
//------------------------------------------------------------------------------

EnumDefinition::Value::Value(const Value& value)
{
    next = 0;

    if(value.next) next = new Value(*value.next);
}
//------------------------------------------------------------------------------

EnumDefinition::Value::~Value()
{
    if(next) delete next;
}
//------------------------------------------------------------------------------

EnumDefinition::EnumDefinition(int line, std::string& filename):
    EnumDefinition(line, filename.c_str()){}
//------------------------------------------------------------------------------

EnumDefinition::EnumDefinition(int line, const char* filename):
    Base(line, filename, Type::Enum_Definition)
{
    values = 0;
}
//------------------------------------------------------------------------------

EnumDefinition::~EnumDefinition()
{
    if(values) delete values;
}
//------------------------------------------------------------------------------

Base* EnumDefinition::copy()
{
    EnumDefinition* copy = new EnumDefinition(source.line, source.filename.c_str());

    copy->identifier = identifier;

    if(values) copy->values = new Value(*values);

    return copy;
}
//------------------------------------------------------------------------------

bool EnumDefinition::runAST()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool EnumDefinition::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void EnumDefinition::display()
{
    displayInfo();
    debug.print("Enum Definition (%s):\n", identifier.c_str());

    debug.print(" Values: ");
    Value* value = values;
    while(value){
        debug.print("%s", value->identifier.c_str());
        value = value->next;
        if(value) debug.print(", ");
        else      debug.print("\n");
    }

    if(next) next->display();
}
//------------------------------------------------------------------------------

void EnumDefinition::validateMembers()
{
    assert(type == Type::Enum_Definition);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

