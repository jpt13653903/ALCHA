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
using namespace Netlist;
//------------------------------------------------------------------------------

Array::Array(int line, const string& filename, const char* name) : Base(line, filename, name, Type::Array)
{
    error("Not yet implemented");
}
//------------------------------------------------------------------------------

Array::~Array(){}
//------------------------------------------------------------------------------

AST::Expression* Array::getExpression(int line, const string& filename)
{
    error("Not yet implemented");
    // TODO: Generate an AST::Array expression and fill with elements
    return 0;
}
//------------------------------------------------------------------------------

bool Array::assign(AST::Expression* expression)
{
    error("Not yet implemented");
    return rawAssign(expression);
}
//------------------------------------------------------------------------------

bool Array::rawAssign(AST::Expression* expression)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool Array::hasCircularReference(Base* object)
{
    if(this == object) return true;
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Array::display(int indent)
{
    error("Not implemented yet");
}
//------------------------------------------------------------------------------

void Array::validate()
{
    assert(type == Type::Array);

    Base::validate();

    error("Not implemented yet");
}
//------------------------------------------------------------------------------

