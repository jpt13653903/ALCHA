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

#include "String.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

String::String(int line, const string& filename): String(line, filename.c_str()){}
//------------------------------------------------------------------------------

String::String(int line, const char* filename): Expression(line, filename, Type::String){}
//------------------------------------------------------------------------------

String::~String(){}
//------------------------------------------------------------------------------

Base* String::copy()
{
    String* copy = new String(source.line, source.filename.c_str());

    copy->value = value;

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool String::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

Expression* String::evaluate()
{
    error("Not yet implemented");
    return this;
//   Expression* result = 0;
//
//   result = (Expression*)copy();
//
//   if(!result) return 0;
//   return result->simplify(false);
}
//------------------------------------------------------------------------------

int String::getWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

Number& String::getFullScale()
{
    error("Not yet implemented");
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool String::getSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool String::hasCircularReference(Netlist::Base* object)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void String::populateUsed()
{
    error("Not yet implemented");
}
//------------------------------------------------------------------------------

Expression* String::removeTempNet(int width, bool isSigned)
{
    error("Not yet implemented");
    return this;
}
//------------------------------------------------------------------------------

void String::display()
{
    displayStart();

    logger.print("\"%s\"", value.c_str());

    displayEnd();
}
//------------------------------------------------------------------------------

void String::validateMembers()
{
    assert(type == Type::String);

    assert(!next);
    assert(!prev);

    assert(!left );
    assert(!right);
}
//------------------------------------------------------------------------------

