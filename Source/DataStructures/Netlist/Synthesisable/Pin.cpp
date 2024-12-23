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

#include "Pin.h"

#include "AST/Expression/Object.h"
//------------------------------------------------------------------------------

using std::string;
using namespace Netlist;
//------------------------------------------------------------------------------

Pin::Pin(int line, const string& filename, const char* name) : Synthesisable(line, filename, name, Type::Pin)
{
    driver  = new PinComponent(line, filename, "driver" , this);
    enabled = new PinComponent(line, filename, "enabled", this);
}
//------------------------------------------------------------------------------

Pin::~Pin()
{
    delete driver;
    delete enabled;
}
//------------------------------------------------------------------------------

AST::Expression* Pin::getExpression(int line, const string& filename)
{
    return (AST::Expression*)driver->getExpression(line, filename);
}
//------------------------------------------------------------------------------

bool Pin::assign(AST::Expression* expression)
{
    if(direction == AST::Definition::Direction::Input){
        expression->printError("Cannot assign to an input pin");
        return false;
    }
    return driver->assign(expression);
}
//------------------------------------------------------------------------------

bool Pin::rawAssign(AST::Expression* expression)
{
    if(direction == AST::Definition::Direction::Input){
        expression->printError("Cannot assign to an input pin");
        return false;
    }
    return driver->rawAssign(expression);
}
//------------------------------------------------------------------------------

bool Pin::hasCircularReference(Base* object)
{
    if(this == object) return true;
    if(driver ->hasCircularReference(object)) return true;
    if(enabled->hasCircularReference(object)) return true;
    return false;
}
//------------------------------------------------------------------------------

void Pin::populateUsed(bool setUsed)
{
    if(used) return; // Prevents circular loops
    used = setUsed;
    driver ->populateUsed(setUsed);
    enabled->populateUsed(setUsed);

    // Pins that have stuff assigned are always used
    if(driver->value) used = true;
}
//------------------------------------------------------------------------------

Base* Pin::getMember(const std::string& name)
{
    if(name == "driver" ) return driver;
    if(name == "enabled") return enabled;
    return 0;
}
//------------------------------------------------------------------------------

void Pin::display(int indent)
{
    logger.indent(indent);
    logger.print("Pin: ");

    indent++;
    displayParameters(indent);
    displayAttributes(indent);

    driver ->display(indent);
    enabled->display(indent);
}
//------------------------------------------------------------------------------

void Pin::validate()
{
    assert(type == Type::Pin);

    Synthesisable::validate();

    driver ->validate();
    enabled->validate();
}
//------------------------------------------------------------------------------

