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

#include "BitAnd.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

BitAnd::BitAnd(int line, const string& filename): BitAnd(line, filename.c_str()){}
//------------------------------------------------------------------------------

BitAnd::BitAnd(int line, const char* filename): Expression(line, filename, Type::Bit_AND){}
//------------------------------------------------------------------------------

BitAnd::~BitAnd(){}
//------------------------------------------------------------------------------

Base* BitAnd::copy()
{
    BitAnd* copy = new BitAnd(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool BitAnd::getVerilog(string& body)
{
    body += "(";
    left->getVerilog(body);
    body += ") & (";
    right->getVerilog(body);
    body += ")";

    return true;
}
//------------------------------------------------------------------------------

Expression* BitAnd::evaluate()
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

int BitAnd::getWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

Number& BitAnd::getFullScale()
{
    error("Not yet implemented");
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool BitAnd::getSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool BitAnd::hasCircularReference(Netlist::Base* object)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void BitAnd::populateUsed()
{
    error("Not yet implemented");
}
//------------------------------------------------------------------------------

Expression* BitAnd::removeTempNet(int width, bool isSigned)
{
    error("Not yet implemented");
    return this;
}
//------------------------------------------------------------------------------

void BitAnd::display()
{
    displayStart();

    debug.print( " & ");

    displayEnd();
}
//------------------------------------------------------------------------------

void BitAnd::validateMembers()
{
    assert(type == Type::Bit_AND);

    assert(!next);
    assert(!prev);

    // TODO: assert(!left );
    // TODO: assert(!right);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

