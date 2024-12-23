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

#include "Replicate.h"
#include "Literal.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Replicate::Replicate(int line, const string& filename): Replicate(line, filename.c_str()){}
//------------------------------------------------------------------------------

Replicate::Replicate(int line, const char* filename): Expression(line, filename, Type::Replicate){}
//------------------------------------------------------------------------------

Replicate::~Replicate(){}
//------------------------------------------------------------------------------

Base* Replicate::copy()
{
    Replicate* copy = new Replicate(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool Replicate::getVerilog(string& body)
{
    body += "{(";
    right->getVerilog(body);
    body += "){";
    left->getVerilog(body);
    body += "}}";

    return true;
}
//------------------------------------------------------------------------------

Expression* Replicate::evaluate()
{
    assert(left , return this);
    assert(right, return this);

    left  = left ->evaluate();
    right = right->evaluate();

    assert(left , return this);
    assert(right, return this);

    if(right->type != Type::Literal){
        printError("Replicate number should evaluate to a literal");
        return this;
    }

    return makeObject();
}
//------------------------------------------------------------------------------

int Replicate::getWidth()
{
    assert(left , return 0);
    assert(right, return 0);

    left  = left ->evaluate();
    right = right->evaluate();

    assert(left , return 0);
    assert(right, return 0);

    if(right->type != Type::Literal){
        printError("Replicate number should evaluate to a literal");
        return 0;
    }

    return left->getWidth() * ((Literal*)right)->value.getReal();
}
//------------------------------------------------------------------------------

Number& Replicate::getFullScale()
{
    static Number result;
    result = 1;
    result.binScale(getWidth());
    return result;
}
//------------------------------------------------------------------------------

bool Replicate::getSigned()
{
    return false;
}
//------------------------------------------------------------------------------

bool Replicate::hasCircularReference(Netlist::Base* object)
{
    assert(left , return false);
    assert(right, return false);

    if(left ->hasCircularReference(object)) return true;
    if(right->hasCircularReference(object)) return true;

    return false;
}
//------------------------------------------------------------------------------

void Replicate::populateUsed()
{
    assert(left , return);
    assert(right, return);

    left ->populateUsed();
    right->populateUsed();
}
//------------------------------------------------------------------------------

Expression* Replicate::removeTempNet(int width, bool isSigned)
{
    if(left ) left  = left ->removeTempNet(0, false);
    if(right) right = right->removeTempNet(0, false);
    return this;
}
//------------------------------------------------------------------------------

void Replicate::display()
{
    displayStart();

    logger.print("{rep}");

    displayEnd();
}
//------------------------------------------------------------------------------

void Replicate::validateMembers()
{
    assert(type == Type::Replicate);

    assert(!next);
    assert(!prev);

    assert(left , return); left ->validate();
    assert(right, return); right->validate();
}
//------------------------------------------------------------------------------

