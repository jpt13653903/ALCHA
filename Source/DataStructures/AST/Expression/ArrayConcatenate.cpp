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

#include "ArrayConcatenate.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

ArrayConcatenate::ArrayConcatenate(int line, const string& filename): ArrayConcatenate(line, filename.c_str()){}
//------------------------------------------------------------------------------

ArrayConcatenate::ArrayConcatenate(int line, const char* filename): Expression(line, filename, Type::ArrayConcatenate){}
//------------------------------------------------------------------------------

ArrayConcatenate::~ArrayConcatenate()
{
    for(auto element: elements) delete element;
}
//------------------------------------------------------------------------------

Base* ArrayConcatenate::copy()
{
    ArrayConcatenate* copy = new ArrayConcatenate(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    for(auto element: elements){
        copy->elements.push_back((Expression*)element->copy());
    }

    return copy;
}
//------------------------------------------------------------------------------

bool ArrayConcatenate::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

Expression* ArrayConcatenate::evaluate()
{
    error("Not yet implemented");
    return this;
//   auto array = (AST::ArrayConcatenate*)copy(true);
//   for(auto element: array->elements) element = element->evaluate();
//
//   return array->simplify(false);
}
//------------------------------------------------------------------------------

int ArrayConcatenate::getWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

Number& ArrayConcatenate::getFullScale()
{
    error("Not yet implemented");
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool ArrayConcatenate::getSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool ArrayConcatenate::hasCircularReference(Netlist::Base* object)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void ArrayConcatenate::populateUsed()
{
    error("Not yet implemented");
}
//------------------------------------------------------------------------------

Expression* ArrayConcatenate::removeTempNet(int width, bool isSigned)
{
    error("Not yet implemented");
    return this;
}
//------------------------------------------------------------------------------

void ArrayConcatenate::display()
{
    debug.print("(ArrayConcat: (");
    bool isFirst = true;
    for(auto element: elements){
        if(!isFirst) debug.print(", ");
        element->display();
        isFirst = false;
    }
    debug.print("))");
}
//------------------------------------------------------------------------------

void ArrayConcatenate::validateMembers()
{
    assert(type == Type::ArrayConcatenate);

    assert(!next);
    assert(!prev);

    assert(!left );
    assert(!right);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

