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

#include "AndReduce.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

AndReduce::AndReduce(int line, const string& filename): AndReduce(line, filename.c_str()){}
//------------------------------------------------------------------------------

AndReduce::AndReduce(int line, const char* filename): Expression(line, filename, Type::AndReduce){}
//------------------------------------------------------------------------------

AndReduce::~AndReduce(){}
//------------------------------------------------------------------------------

Base* AndReduce::copy()
{
    AndReduce* copy = new AndReduce(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool AndReduce::getVerilog(string& body)
{
    body += "&(";
    right->getVerilog(body);
    body += ")";

    return true;

}
//------------------------------------------------------------------------------

Expression* AndReduce::evaluate()
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

int AndReduce::getWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

Number& AndReduce::getFullScale()
{
    error("Not yet implemented");
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool AndReduce::getSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool AndReduce::hasCircularReference(Netlist::Base* object)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void AndReduce::populateUsed()
{
    error("Not yet implemented");
}
//------------------------------------------------------------------------------

Expression* AndReduce::removeTempNet(int width, bool isSigned)
{
    error("Not yet implemented");
    return this;
}
//------------------------------------------------------------------------------

void AndReduce::display()
{
    displayStart();

    logger.print( " &");

    displayEnd();
}
//------------------------------------------------------------------------------

void AndReduce::validateMembers()
{
    assert(type == Type::AndReduce);

    assert(!next);
    assert(!prev);

    // TODO: assert(left );
    // TODO: assert(right);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

