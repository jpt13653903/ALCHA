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

#include "AccessMember.h"
#include "Object.h"
#include "Identifier.h"
#include "Netlist/Alias.h"
#include "Netlist/NameSpace/Module.h"
#include "Netlist/NameSpace.h"
#include "Netlist/Synthesisable.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

AccessMember::AccessMember(int line, const string& filename): AccessMember(line, filename.c_str()){}
//------------------------------------------------------------------------------

AccessMember::AccessMember(int line, const char* filename): Expression(line, filename, Type::AccessMember){}
//------------------------------------------------------------------------------

AccessMember::~AccessMember(){}
//------------------------------------------------------------------------------

Base* AccessMember::copy()
{
    AccessMember* copy = new AccessMember(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool AccessMember::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

Expression* AccessMember::evaluate()
{
    assert(left , delete this; return 0);
    assert(right, delete this; return 0);

    left = left->evaluate();

    assert(left, delete this; return 0);

    if(left->type != Type::Object){
        printError("Invalid member access expression");
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
            printError("Invalid member access expression");
            delete this;
            return 0;
    }

    auto object = ((Object    *)left )->objectRef;
    auto name   = ((Identifier*)right)->name;

    auto member = object->getMember(name);
    if(!member){
        printError();
        printf("member %s not found in object %s\n",
               name.c_str(), object->name.c_str());
        delete this;
        return 0;
    }

    auto result = new Object(source.line, source.filename);
    result->objectRef = member;
    delete this;
    return result->evaluate();
}
//------------------------------------------------------------------------------

int AccessMember::getWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

Number& AccessMember::getFullScale()
{
    error("Not yet implemented");
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool AccessMember::getSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool AccessMember::hasCircularReference(Netlist::Base* object)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void AccessMember::populateUsed()
{
    error("Not yet implemented");
}
//------------------------------------------------------------------------------

Expression* AccessMember::removeTempNet(int width, bool isSigned)
{
    error("Not yet implemented");
    return this;
}
//------------------------------------------------------------------------------

void AccessMember::display()
{
    displayStart();

    logger.print("." );

    displayEnd();
}
//------------------------------------------------------------------------------

void AccessMember::validateMembers()
{
    assert(type == Type::AccessMember);

    assert(!next);
    assert(!prev);

    // TODO: The "left" assertion fails when the member cannot be found
    assert(left , return); left ->validate();
    assert(right, return); right->validate();
}
//------------------------------------------------------------------------------

