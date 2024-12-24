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

#include "NameSpacePush.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

NameSpacePush::NameSpacePush(int line, std::string& filename): NameSpacePush(line, filename.c_str()){}
//------------------------------------------------------------------------------

NameSpacePush::NameSpacePush(int line, const char* filename): Base(line, filename, Type::NameSpacePush)
{
    nameSpace  = 0;
    statements = 0;
}
//------------------------------------------------------------------------------

NameSpacePush::~NameSpacePush()
{
    if(nameSpace ) delete nameSpace;
    if(statements) delete statements;
}
//------------------------------------------------------------------------------

Base* NameSpacePush::copy()
{
    NameSpacePush* copy = new NameSpacePush(source.line, source.filename.c_str());

    if(nameSpace ) copy->nameSpace  = (decltype(nameSpace ))nameSpace ->copy();
    if(statements) copy->statements = (decltype(statements))statements->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool NameSpacePush::runAST()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool NameSpacePush::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void NameSpacePush::display()
{
    displayInfo();
    logger.print("NameSpacePush:\n ");

    if(nameSpace){
        if(nameSpace->left || nameSpace->right) logger.print("(");
        nameSpace->display();
        if(nameSpace->left || nameSpace->right) logger.print(")");
    }

    logger.print(".{\n");
        if(statements) statements->display();
    logger.print("}\n");

    if(next) next->display();
}
//------------------------------------------------------------------------------

void NameSpacePush::validateMembers()
{
    assert(type == Type::NameSpacePush);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

