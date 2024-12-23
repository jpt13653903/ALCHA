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

#include "WhileLoop.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

WhileLoop::WhileLoop(int line, std::string& filename): WhileLoop(line, filename.c_str()){}
//------------------------------------------------------------------------------

WhileLoop::WhileLoop(int line, const char* filename): Base(line, filename, Type::WhileLoop)
{
    condition  = 0;
    statements = 0;
}
//------------------------------------------------------------------------------

WhileLoop::~WhileLoop()
{
    if(condition ) delete condition;
    if(statements) delete statements;
}
//------------------------------------------------------------------------------

Base* WhileLoop::copy()
{
    WhileLoop* copy = new WhileLoop(source.line, source.filename.c_str());

    if(condition) copy->condition = (decltype(condition))condition->copy();

    copy->statements = copyList(statements);

    return copy;
}
//------------------------------------------------------------------------------

bool WhileLoop::runAST()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool WhileLoop::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void WhileLoop::display()
{
    displayInfo();
    logger.print("while(");
        if(condition) condition->display();
    logger.print("){\n");
        if(statements) statements->display();
    logger.print("}\n");

    if(next) next->display();
}
//------------------------------------------------------------------------------

void WhileLoop::validateMembers()
{
    assert(type == Type::WhileLoop);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

