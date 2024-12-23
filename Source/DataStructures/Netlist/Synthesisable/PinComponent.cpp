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

#include "PinComponent.h"
#include "Pin.h"
//------------------------------------------------------------------------------

using std::string;
using namespace Netlist;
//------------------------------------------------------------------------------

PinComponent::PinComponent(int line, const string& filename, const char* name, Pin* pin):
    Net(line, filename, name)
{
    type = Type::PinComponent;

    this->pin = pin;
    nameSpace = pin->nameSpace;
}
//------------------------------------------------------------------------------

PinComponent::~PinComponent(){}
//------------------------------------------------------------------------------

bool PinComponent::rawAssign(AST::Expression* expression)
{
    assert(expression, return false);

    if(value) delete value;
    value = expression->evaluate();
    if(value){
        if(value->hasCircularReference(this) ||
              value->hasCircularReference(pin ) ){
            value->printError("Circular combinational circuit");
        }
    }
    return value;
}
//------------------------------------------------------------------------------

void PinComponent::populateUsed(bool setUsed)
{
    if(value) value->populateUsed();
}
//------------------------------------------------------------------------------

Base* PinComponent::getAttribute(const std::string& name)
{
    return pin->getAttribute(name);
}
//------------------------------------------------------------------------------

AST::Expression* PinComponent::getAttribValue(const std::string& name)
{
    return pin->getAttribValue(name);
}
//------------------------------------------------------------------------------

AST::Expression* PinComponent::getBuiltInAttributeValue(const std::string& name)
{
    return pin->getBuiltInAttributeValue(name);
}
//------------------------------------------------------------------------------

int PinComponent::width()
{
    return pin->width();
}
//------------------------------------------------------------------------------

Number& PinComponent::fullScale()
{
    return pin->fullScale();
}
//------------------------------------------------------------------------------

bool PinComponent::isSigned()
{
    return pin->isSigned();
}
//------------------------------------------------------------------------------

void PinComponent::display(int indent)
{
    debug.indent(indent);
    debug.print("pin Component: %s\n", name.c_str());

    indent++;
    debug.indent(indent);
    debug.print("value = ");
    if(value) value->display();
    else      debug.print("{null}");
    debug.print("\n");
}
//------------------------------------------------------------------------------

void PinComponent::validate()
{
    assert(type == Type::PinComponent);

    assert(attributes.size() == 0);
    assert(nameSpace == pin->nameSpace);

    Base::validate();
}
//------------------------------------------------------------------------------

