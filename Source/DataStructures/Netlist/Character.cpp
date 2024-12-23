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

#include "Character.h"

#include "AST/expression/Literal.h"
//------------------------------------------------------------------------------

using std::string;
using namespace Netlist;
//------------------------------------------------------------------------------

Character::Character(int line, const string& filename, const char* name) : Base(line, filename, name, Type::Character)
{
    value = 0;
}
//------------------------------------------------------------------------------

Character::~Character(){}
//------------------------------------------------------------------------------

AST::Expression* Character::getExpression(int line, const string& filename)
{
    AST::Literal* result = new AST::Literal(line, filename);
    result->value = value;
    result->setWidth(32);
    return result;
}
//------------------------------------------------------------------------------

bool Character::assign(AST::Expression* expression)
{
    error("Not yet implemented");
    return rawAssign(expression);
}
//------------------------------------------------------------------------------

bool Character::rawAssign(AST::Expression* expression)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool Character::hasCircularReference(Base* object)
{
    if(this == object) return true;
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Character::display(int indent)
{
    debug.indent(indent+1);
    debug.print("Character: %s\n", name.c_str());

    debug.indent(indent+2);
    debug.print("value = '%s' ", utfConverter.toUtf8(value));
    debug.print("(%d)\n", value);
}
//------------------------------------------------------------------------------

void Character::validate()
{
    assert(type == Type::Character);

    Base::validate();

    error("Not implemented yet");
}
//------------------------------------------------------------------------------

