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

#include "LoopLoop.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

LoopLoop::LoopLoop(int line, std::string& filename): LoopLoop(line, filename.c_str()){}
//------------------------------------------------------------------------------

LoopLoop::LoopLoop(int line, const char* filename): Base(line, filename, Type::LoopLoop)
{
    count      = 0;
    statements = 0;
}
//------------------------------------------------------------------------------

LoopLoop::~LoopLoop()
{
    if(count     ) delete count;
    if(statements) delete statements;
}
//------------------------------------------------------------------------------

Base* LoopLoop::copy()
{
    LoopLoop* copy = new LoopLoop(source.line, source.filename.c_str());

    if(count) copy->count = (decltype(count))count->copy();

    copy->statements = copyList(statements);

    return copy;
}
//------------------------------------------------------------------------------

bool LoopLoop::runAST()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool LoopLoop::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void LoopLoop::display()
{
    displayInfo();
    debug.print("loop(");
        if(count) count->display();
        else      debug.print("{inf}");
    debug.print("){\n");
        if(statements) statements->display();
    debug.print("}\n");

    if(next) next->display();
}
//------------------------------------------------------------------------------

void LoopLoop::validateMembers()
{
    assert(type == Type::LoopLoop);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

