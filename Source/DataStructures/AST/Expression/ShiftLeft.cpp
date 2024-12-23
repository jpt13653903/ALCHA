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

#include "ShiftLeft.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

ShiftLeft::ShiftLeft(int line, const string& filename): ShiftLeft(line, filename.c_str()){}
//------------------------------------------------------------------------------

ShiftLeft::ShiftLeft(int line, const char* filename): Expression(line, filename, Type::Shift_Left){}
//------------------------------------------------------------------------------

ShiftLeft::~ShiftLeft(){}
//------------------------------------------------------------------------------

Base* ShiftLeft::copy()
{
    ShiftLeft* copy = new ShiftLeft(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool ShiftLeft::getVerilog(string& body)
{
    body += "(";
    left->getVerilog(body);
    body += ") <<< (";
    right->getVerilog(body);
    body += ")";

    return true;
}
//------------------------------------------------------------------------------

Expression* ShiftLeft::evaluate()
{
    error("Not yet implemented");
    return this;
//   Expression* result = 0;
//
//   error("Not yet implemented");
//
//   if(!result) return 0;
//   return result->simplify(false);
}
//------------------------------------------------------------------------------

int ShiftLeft::getWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

Number& ShiftLeft::getFullScale()
{
    error("Not yet implemented");
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool ShiftLeft::getSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool ShiftLeft::hasCircularReference(Netlist::Base* object)
{
    assert(left , return false);
    assert(right, return false);

    if(left ->hasCircularReference(object)) return true;
    if(right->hasCircularReference(object)) return true;

    return false;
}
//------------------------------------------------------------------------------

void ShiftLeft::populateUsed()
{
    assert(left , return);
    assert(right, return);

    left ->populateUsed();
    right->populateUsed();
}
//------------------------------------------------------------------------------

Expression* ShiftLeft::removeTempNet(int width, bool isSigned)
{
    if(left ) left  = left ->removeTempNet(0, false);
    if(right) right = right->removeTempNet(0, false);
    return this;
}
//------------------------------------------------------------------------------

void ShiftLeft::display()
{
    displayStart();

    debug.print(" << ");

    displayEnd();
}
//------------------------------------------------------------------------------

void ShiftLeft::validateMembers()
{
    assert(type == Type::Shift_Left);

    assert(!next);
    assert(!prev);

    assert(left , return); left ->validate();
    assert(right, return); right->validate();
}
//------------------------------------------------------------------------------

