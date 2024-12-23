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

#include "BitXnor.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

BitXnor::BitXnor(int line, const string& filename): BitXnor(line, filename.c_str()){}
//------------------------------------------------------------------------------

BitXnor::BitXnor(int line, const char* filename): Expression(line, filename, Type::Bit_XNOR){}
//------------------------------------------------------------------------------

BitXnor::~BitXnor(){}
//------------------------------------------------------------------------------

Base* BitXnor::copy()
{
    BitXnor* copy = new BitXnor(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool BitXnor::getVerilog(string& body)
{
    body += "(";
    left->getVerilog(body);
    body += ") ~^ (";
    right->getVerilog(body);
    body += ")";

    return true;
}
//------------------------------------------------------------------------------

Expression* BitXnor::evaluate()
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

int BitXnor::getWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

Number& BitXnor::getFullScale()
{
    error("Not yet implemented");
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool BitXnor::getSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool BitXnor::hasCircularReference(Netlist::Base* object)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void BitXnor::populateUsed()
{
    error("Not yet implemented");
}
//------------------------------------------------------------------------------

Expression* BitXnor::removeTempNet(int width, bool isSigned)
{
    error("Not yet implemented");
    return this;
}
//------------------------------------------------------------------------------

void BitXnor::display()
{
    displayStart();

    debug.print(" ~# ");

    displayEnd();
}
//------------------------------------------------------------------------------

void BitXnor::validateMembers()
{
    assert(type == Type::Bit_XNOR);

    assert(!next);
    assert(!prev);

    // TODO: assert(!left );
    // TODO: assert(!right);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

