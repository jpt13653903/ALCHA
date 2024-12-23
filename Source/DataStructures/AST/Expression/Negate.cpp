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

#include "Negate.h"
#include "Literal.h"
#include "Object.h"

#include "Netlist/NameSpace/Module.h"
#include "Netlist/Synthesisable/Net.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Negate::Negate(int line, const string& filename): Negate(line, filename.c_str()){}
//------------------------------------------------------------------------------

Negate::Negate(int line, const char* filename): Expression(line, filename, Type::Negate){}
//------------------------------------------------------------------------------

Negate::~Negate(){}
//------------------------------------------------------------------------------

Base* Negate::copy()
{
    Negate* copy = new Negate(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool Negate::getVerilog(string& body)
{
    body += "-(";
    if(right->getSigned()){
        right->getVerilog(body);
    }else{
        body += "$signed(";
        right->getVerilog(body);
        body += ")";
    }
    body += ")";

    return true;
}
//------------------------------------------------------------------------------

Expression* Negate::evaluate()
{
    assert(right, delete this; return 0);
    right = right->evaluate();
    assert(right, delete this; return 0);

    switch(right->type){
        case Type::Literal:{
            auto result = (Literal*)right;
            right = 0;
            result->value.mul(-1);
            delete this;
            return result;
        }
        case Type::Object:{
            return makeObject();
        }
        default:
            break;
    }
    return this;
}
//------------------------------------------------------------------------------

int Negate::getWidth()
{
    assert(right, return false);
    return right->getWidth();
}
//------------------------------------------------------------------------------

Number& Negate::getFullScale()
{
    static Number zero = 0;
    assert(right, return zero);
    return right->getFullScale();
}
//------------------------------------------------------------------------------

bool Negate::getSigned()
{
    return true;
}
//------------------------------------------------------------------------------

bool Negate::hasCircularReference(Netlist::Base* object)
{
    assert(right, return false);
    return right->hasCircularReference(object);
}
//------------------------------------------------------------------------------

void Negate::populateUsed()
{
    assert(right, return);
    right->populateUsed();
}
//------------------------------------------------------------------------------

Expression* Negate::removeTempNet(int width, bool isSigned)
{
    if(right) right = right->removeTempNet(0, false);
    return this;
}
//------------------------------------------------------------------------------

void Negate::display()
{
    displayStart();

    debug.print(" -");

    displayEnd();
}
//------------------------------------------------------------------------------

void Negate::validateMembers()
{
    assert(type == Type::Negate);

    assert(!next);
    assert(!prev);

    assert(!left);
    assert(right, return); right->validate();
}
//------------------------------------------------------------------------------

