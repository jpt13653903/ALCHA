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

#include "Range.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Range::Range(int line, const string& filename): Range(line, filename.c_str()){}
//------------------------------------------------------------------------------

Range::Range(int line, const char* filename): Expression(line, filename, Type::Range)
{
    step = 0;
}
//------------------------------------------------------------------------------

Range::~Range()
{
    if(step) delete step;
}
//------------------------------------------------------------------------------

Base* Range::copy()
{
    Range* copy = new Range(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();
    if(step ) copy->step  = (decltype(step ))step ->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool Range::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

Expression* Range::evaluate()
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

int Range::getWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

Number& Range::getFullScale()
{
    error("Not yet implemented");
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool Range::getSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool Range::hasCircularReference(Netlist::Base* object)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Range::populateUsed()
{
    error("Not yet implemented");
}
//------------------------------------------------------------------------------

Expression* Range::removeTempNet(int width, bool isSigned)
{
    error("Not yet implemented");
    return this;
}
//------------------------------------------------------------------------------

void Range::display()
{
    displayStart();

    logger.print("..");

    displayEnd();

    if(step){
        logger.print(":");
        if(step->left || step->right) logger.print("(");
        step->display();
        if(step->left || step->right) logger.print(")");
    }
}
//------------------------------------------------------------------------------

void Range::validateMembers()
{
    assert(type == Type::Range);

    assert(!next);
    assert(!prev);

    // TODO: assert(!left );
    // TODO: assert(!right);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

