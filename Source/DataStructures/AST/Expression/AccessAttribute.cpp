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

#include "AccessAttribute.h"
#include "Object.h"
#include "Identifier.h"
#include "Netlist/Base.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

AccessAttribute::AccessAttribute(int line, const string& filename): AccessAttribute(line, filename.c_str()){}
//------------------------------------------------------------------------------

AccessAttribute::AccessAttribute(int line, const char* filename): Expression(line, filename, Type::AccessAttribute){}
//------------------------------------------------------------------------------

AccessAttribute::~AccessAttribute(){}
//------------------------------------------------------------------------------

Base* AccessAttribute::copy()
{
    AccessAttribute* copy = new AccessAttribute(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool AccessAttribute::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

Expression* AccessAttribute::evaluate()
{
    assert(left , delete this; return 0);
    assert(right, delete this; return 0);

    left = left->evaluate();

    assert(left, delete this; return 0);

    if(left->type != Type::Object){
        printError("Invalid attribute access expression");
        delete this;
        return 0;
    }
    switch(right->type){
        case Type::Identifier:
            break;

        case Type::Slice:
            error("Not yet implemented");
            delete this;
            return 0;

        default:
            printError("Invalid attribute access expression");
            delete this;
            return 0;
    }

    auto object = ((Object    *)left )->objectRef;
    auto name   = ((Identifier*)right)->name;

    auto result = object->getBuiltInAttributeValue(name);
    if(result){
        delete this;
        return result;
    }

    result = object->getAttribValue(name);
    if(result){
        delete this;
        return (decltype(result))result->copy();
    }

    printError();
    printf("Attribute %s not found in object %s\n",
           name.c_str(), object->name.c_str());
    delete this;
    return 0;
}
//------------------------------------------------------------------------------

int AccessAttribute::getWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

Number& AccessAttribute::getFullScale()
{
    error("Not yet implemented");
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool AccessAttribute::getSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool AccessAttribute::hasCircularReference(Netlist::Base* object)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void AccessAttribute::populateUsed()
{
    error("Not yet implemented");
}
//------------------------------------------------------------------------------

Expression* AccessAttribute::removeTempNet(int width, bool isSigned)
{
    error("Not yet implemented");
    return this;
}
//------------------------------------------------------------------------------

void AccessAttribute::display()
{
    displayStart();

    debug.print("'" );

    displayEnd();
}
//------------------------------------------------------------------------------

void AccessAttribute::validateMembers()
{
    assert(type == Type::AccessAttribute);

    assert(!next);
    assert(!prev);

    if(left) left->validate();
    assert(right, return); right->validate();
}
//------------------------------------------------------------------------------

