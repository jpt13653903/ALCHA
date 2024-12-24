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

#include "ShiftRight.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

ShiftRight::ShiftRight(int line, const string& filename): ShiftRight(line, filename.c_str()){}
//------------------------------------------------------------------------------

ShiftRight::ShiftRight(int line, const char* filename): Expression(line, filename, Type::ShiftRight){}
//------------------------------------------------------------------------------

ShiftRight::~ShiftRight(){}
//------------------------------------------------------------------------------

Base* ShiftRight::copy()
{
    ShiftRight* copy = new ShiftRight(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool ShiftRight::getVerilog(string& body)
{
    body += "(";
    left->getVerilog(body);
    body += ") >>> (";
    right->getVerilog(body);
    body += ")";

    return true;
}
//------------------------------------------------------------------------------

Expression* ShiftRight::evaluate()
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

int ShiftRight::getWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

Number& ShiftRight::getFullScale()
{
    error("Not yet implemented");
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool ShiftRight::getSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool ShiftRight::hasCircularReference(Netlist::Base* object)
{
    assert(left , return false);
    assert(right, return false);

    if(left ->hasCircularReference(object)) return true;
    if(right->hasCircularReference(object)) return true;

    return false;
}
//------------------------------------------------------------------------------

void ShiftRight::populateUsed()
{
    assert(left , return);
    assert(right, return);

    left ->populateUsed();
    right->populateUsed();
}
//------------------------------------------------------------------------------

Expression* ShiftRight::removeTempNet(int width, bool isSigned)
{
    if(left ) left  = left ->removeTempNet(0, false);
    if(right) right = right->removeTempNet(0, false);
    return this;
}
//------------------------------------------------------------------------------

void ShiftRight::display()
{
    displayStart();

    logger.print(" >> ");

    displayEnd();
}
//------------------------------------------------------------------------------

void ShiftRight::validateMembers()
{
    assert(type == Type::ShiftRight);

    assert(!next);
    assert(!prev);

    assert(left , return); left ->validate();
    assert(right, return); right->validate();
}
//------------------------------------------------------------------------------

