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

#include "VectorConcatenate.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

VectorConcatenate::VectorConcatenate(int line, const string& filename): VectorConcatenate(line, filename.c_str()){}
//------------------------------------------------------------------------------

VectorConcatenate::VectorConcatenate(int line, const char* filename): Expression(line, filename, Type::VectorConcatenate){}
//------------------------------------------------------------------------------

VectorConcatenate::~VectorConcatenate()
{
    for(auto element: elements) delete element;
}
//------------------------------------------------------------------------------

Base* VectorConcatenate::copy()
{
    VectorConcatenate* copy = new VectorConcatenate(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    for(auto element: elements){
        copy->elements.push_back((Expression*)element->copy());
    }

    return copy;
}
//------------------------------------------------------------------------------

bool VectorConcatenate::getVerilog(string& body)
{
    bool isFirst = true;

    if(elements.size() > 1){
        for(auto element: elements){
            if(isFirst) body += "{";
            else        body += ", ";
            element->getVerilog(body);
            isFirst = false;
        }
        body += "}";
    }else{
        elements.front()->getVerilog(body);
    }

    return true;
}
//------------------------------------------------------------------------------

Expression* VectorConcatenate::evaluate()
{
    for(auto element: elements){
        element = element->evaluate();
    }
    warning("Incomplete implementation");
    // TODO: If there are elements that can be combined, do so
    return this;
}
//------------------------------------------------------------------------------

int VectorConcatenate::getWidth()
{
    int result = 0;
    for(auto element: elements){
        element = element->evaluate();
        result += element->getWidth();
    }
    return result;
}
//------------------------------------------------------------------------------

Number& VectorConcatenate::getFullScale()
{
    static Number result;
    result = 1;
    result.binScale(getWidth());
    return result;
}
//------------------------------------------------------------------------------

bool VectorConcatenate::getSigned()
{
    return false;
}
//------------------------------------------------------------------------------

bool VectorConcatenate::hasCircularReference(Netlist::Base* object)
{
    for(auto element: elements){
        assert(element, return false);
        if(element->hasCircularReference(object)) return true;
    }
    return false;
}
//------------------------------------------------------------------------------

void VectorConcatenate::populateUsed()
{
    for(auto element: elements){
        assert(element, return);
        element->populateUsed();
    }
}
//------------------------------------------------------------------------------

Expression* VectorConcatenate::removeTempNet(int width, bool isSigned)
{
    for(auto element: elements){
        if(element) element->removeTempNet(0, false);
    }
    return this;
}
//------------------------------------------------------------------------------

void VectorConcatenate::display()
{
    debug.print("(VectorConcat: (");
    bool isFirst = true;
    for(auto element: elements){
        if(!isFirst) debug.print(", ");
        element->display();
        isFirst = false;
    }
    debug.print("))");
}
//------------------------------------------------------------------------------

void VectorConcatenate::validateMembers()
{
    assert(type == Type::VectorConcatenate);

    assert(!next);
    assert(!prev);

    assert(!left);
    assert(!right);

    for(auto element: elements){
        assert(element);
        element->validate();
    }
}
//------------------------------------------------------------------------------

