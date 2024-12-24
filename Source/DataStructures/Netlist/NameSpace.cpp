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

#include "NameSpace.h"
//------------------------------------------------------------------------------

using std::string;
using namespace Netlist;
//------------------------------------------------------------------------------

NameSpace::NameSpace(int line, const string& filename,
                                          const char* name, Type nameSpaceType):
Base(line, filename, name, nameSpaceType)
{
    ast = 0;
}
//------------------------------------------------------------------------------

NameSpace::~NameSpace()
{
    if(ast) delete ast;
    for(auto s: symbols) delete s.second;
}
//------------------------------------------------------------------------------

AST::Expression* NameSpace::getExpression(int line, const string& filename)
{
    ::printError(line, filename, "Cannot obtain an expression from a namespace");
    return 0;
}
//------------------------------------------------------------------------------

bool NameSpace::assign(AST::Expression* expression)
{
    return rawAssign(expression);
}
//------------------------------------------------------------------------------

bool NameSpace::rawAssign(AST::Expression* expression)
{
    expression->printError("Cannot assign to a namespace");
    return false;
}
//------------------------------------------------------------------------------

bool NameSpace::hasCircularReference(Base* object)
{
    if(this == object) return true;
    return false;
}
//------------------------------------------------------------------------------

bool NameSpace::isNameSpace()
{
    return true;
}
//------------------------------------------------------------------------------

Base* NameSpace::getMember(const std::string& name)
{
    auto member = symbols.find(name);
    if(member != symbols.end()) return member->second;
    return 0;
}
//------------------------------------------------------------------------------

void NameSpace::validate()
{
    Base::validate();

    if(ast) ast->validate();
    for(auto symbol: symbols){
        assert(symbol.second, return);
        symbol.second->validate();
    }
}
//------------------------------------------------------------------------------

