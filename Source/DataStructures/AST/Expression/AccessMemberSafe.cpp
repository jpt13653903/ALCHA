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

#include "AccessMemberSafe.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

AccessMemberSafe::AccessMemberSafe(int line, const string& filename): AccessMemberSafe(line, filename.c_str()){}
//------------------------------------------------------------------------------

AccessMemberSafe::AccessMemberSafe(int line, const char* filename): Expression(line, filename, Type::AccessMemberSafe){}
//------------------------------------------------------------------------------

AccessMemberSafe::~AccessMemberSafe(){}
//------------------------------------------------------------------------------

Base* AccessMemberSafe::copy()
{
    AccessMemberSafe* copy = new AccessMemberSafe(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool AccessMemberSafe::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

Expression* AccessMemberSafe::evaluate()
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

int AccessMemberSafe::getWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

Number& AccessMemberSafe::getFullScale()
{
    error("Not yet implemented");
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool AccessMemberSafe::getSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool AccessMemberSafe::hasCircularReference(Netlist::Base* object)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void AccessMemberSafe::populateUsed()
{
    error("Not yet implemented");
}
//------------------------------------------------------------------------------

Expression* AccessMemberSafe::removeTempNet(int width, bool isSigned)
{
    error("Not yet implemented");
    return this;
}
//------------------------------------------------------------------------------

void AccessMemberSafe::display()
{
    displayStart();

    logger.print("?." );

    displayEnd();
}
//------------------------------------------------------------------------------

void AccessMemberSafe::validateMembers()
{
    assert(type == Type::AccessMemberSafe);

    assert(!next);
    assert(!prev);

    // TODO: assert(left );
    // TODO: assert(right);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

