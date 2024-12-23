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

#include "Array.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Array::Array(int line, const string& filename): Array(line, filename.c_str()){}
//------------------------------------------------------------------------------

Array::Array(int line, const char* filename): Expression(line, filename, Type::Array){}
//------------------------------------------------------------------------------

Array::~Array()
{
    for(auto element: elements) delete element;
}
//------------------------------------------------------------------------------

Base* Array::copy()
{
    Array* copy = new Array(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    for(auto element: elements){
        copy->elements.push_back((Expression*)element->copy());
    }

    return copy;
}
//------------------------------------------------------------------------------

bool Array::getVerilog(string& body)
{
    error("not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

Expression* Array::evaluate()
{
    error("not yet implemented");
    return this;
//   Array* array = (Array*)copy(true);
//   for(auto element: array->elements) element = element->evaluate();
//
//   return array->simplify(false);
}
//------------------------------------------------------------------------------

int Array::getWidth()
{
    error("not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

Number& Array::getFullScale()
{
    error("not yet implemented");
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool Array::getSigned()
{
    error("not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool Array::hasCircularReference(Netlist::Base* object)
{
    error("not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Array::populateUsed()
{
    error("not yet implemented");
}
//------------------------------------------------------------------------------

Expression* Array::removeTempNet(int width, bool isSigned)
{
    error("not yet implemented");
    return this;
}
//------------------------------------------------------------------------------

void Array::display()
{
    debug.print("(array: (");
    bool isFirst = true;
    for(auto element: elements){
        if(!isFirst) debug.print(", ");
        element->display();
        isFirst = false;
    }
    debug.print("))");
}
//------------------------------------------------------------------------------

void Array::validateMembers()
{
    assert(type == Type::Array);

    assert(!next);
    assert(!prev);

    assert(!left);
    assert(!right);

    for(auto element: elements) element->validate();
}
//------------------------------------------------------------------------------

