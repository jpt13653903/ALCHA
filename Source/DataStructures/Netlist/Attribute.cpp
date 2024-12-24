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

#include "Attribute.h"
//------------------------------------------------------------------------------

using std::string;
using namespace Netlist;
//------------------------------------------------------------------------------

Attribute::Attribute(int line, const string& filename, const char* name):
Base(line, filename, name, Type::Attribute)
{
    value = 0;
}
//------------------------------------------------------------------------------

Attribute::~Attribute()
{
    if(value) delete value;
}
//------------------------------------------------------------------------------

AST::Expression* Attribute::getExpression(int line, const string& filename)
{
    if(value) return (AST::Expression*)value->copy();
    ::printError(line, filename, "Operate-assign on empty object");
    return 0;
}
//------------------------------------------------------------------------------

bool Attribute::assign(AST::Expression* expression)
{
    return rawAssign(expression);
}
//------------------------------------------------------------------------------

bool Attribute::rawAssign(AST::Expression* expression)
{
    if(value){
        expression->printWarning();
        printf("Overwriting attribute %s\n", name.c_str());
        delete value;
    }
    value = expression;
    return true;
}
//------------------------------------------------------------------------------

bool Attribute::hasCircularReference(Base* object)
{
    if(this == object) return true;
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Attribute::display(int indent)
{
    logger.indent(indent);
    logger.print(name);
    logger.print(" = ");
    if(value) value->display();
    else      logger.print("{null}");
    logger.print("\n");
}
//------------------------------------------------------------------------------

void Attribute::validate()
{
    assert(type == Type::Attribute);

    assert(attributes.size() == 0);

    Base::validate();
}
//------------------------------------------------------------------------------

