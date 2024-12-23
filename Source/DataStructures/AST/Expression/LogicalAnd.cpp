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

#include "LogicalAnd.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

LogicalAnd::LogicalAnd(int line, const string& filename): LogicalAnd(line, filename.c_str()){}
//------------------------------------------------------------------------------

LogicalAnd::LogicalAnd(int line, const char* filename): Expression(line, filename, Type::Logical_AND){}
//------------------------------------------------------------------------------

LogicalAnd::~LogicalAnd(){}
//------------------------------------------------------------------------------

Base* LogicalAnd::copy()
{
    LogicalAnd* copy = new LogicalAnd(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool LogicalAnd::getVerilog(string& body)
{
    body += "(";
    left->getVerilog(body);
    body += ") && (";
    right->getVerilog(body);
    body += ")";

    return true;
}
//------------------------------------------------------------------------------

Expression* LogicalAnd::evaluate()
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

int LogicalAnd::getWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

Number& LogicalAnd::getFullScale()
{
    error("Not yet implemented");
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool LogicalAnd::getSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool LogicalAnd::hasCircularReference(Netlist::Base* object)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void LogicalAnd::populateUsed()
{
    error("Not yet implemented");
}
//------------------------------------------------------------------------------

Expression* LogicalAnd::removeTempNet(int width, bool isSigned)
{
    error("Not yet implemented");
    return this;
}
//------------------------------------------------------------------------------

void LogicalAnd::display()
{
    displayStart();

    debug.print(" && ");

    displayEnd();
}
//------------------------------------------------------------------------------

void LogicalAnd::validateMembers()
{
    assert(type == Type::Logical_AND);

    assert(!next);
    assert(!prev);

    // TODO: assert(!left );
    // TODO: assert(!right);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

