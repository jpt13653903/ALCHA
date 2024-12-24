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

#include "ForLoop.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

ForLoop::ForLoop(int line, std::string& filename): ForLoop(line, filename.c_str()){}
//------------------------------------------------------------------------------

ForLoop::ForLoop(int line, const char* filename): Base(line, filename, Type::ForLoop)
{
    range      = 0;
    statements = 0;
}
//------------------------------------------------------------------------------

ForLoop::~ForLoop()
{
    if(range     ) delete range;
    if(statements) delete statements;
}
//------------------------------------------------------------------------------

Base* ForLoop::copy()
{
    ForLoop* copy = new ForLoop(source.line, source.filename.c_str());

    copy->identifier = identifier;

    if(range) copy->range = (decltype(range))range->copy();

    copy->statements = copyList(statements);

    return copy;
}
//------------------------------------------------------------------------------

bool ForLoop::runAST()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool ForLoop::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void ForLoop::display()
{
    displayInfo();
    logger.print("for(%s in ", identifier.c_str());
        if(range) range->display();
    logger.print("){\n");
        if(statements) statements->display();
    logger.print("}\n");

    if(next) next->display();
}
//------------------------------------------------------------------------------

void ForLoop::validateMembers()
{
    assert(type == Type::ForLoop);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

