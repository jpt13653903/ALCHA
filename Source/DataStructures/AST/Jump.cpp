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

#include "Jump.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Jump::Jump(int line, std::string& filename, JumpType jumpType): Jump(line, filename.c_str(), jumpType){}
//------------------------------------------------------------------------------

Jump::Jump(int line, const char* filename, JumpType jumpType): Base(line, filename, Type::Jump)
{
    this->jumpType = jumpType;

    expression = 0;
}
//------------------------------------------------------------------------------

Jump::~Jump()
{
    if(expression) delete expression;
}
//------------------------------------------------------------------------------

Base* Jump::copy()
{
    Jump* copy = new Jump(source.line, source.filename.c_str(), jumpType);

    if(expression) copy->expression = (decltype(expression))expression->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool Jump::runAST()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool Jump::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Jump::display()
{
    displayInfo();
    debug.print("jump(");
    switch(jumpType){
        case JumpType::Return  : debug.print("return) "           ); break;
        case JumpType::Break   : debug.print("break) "            ); break;
        case JumpType::Continue: debug.print("continue) "         ); break;
        default                : debug.print("Unknown jump type) "); break;
    }
    if(expression) expression->display();
    else           debug.print("{default}");
    debug.print("\n");

    if(next) next->display();
}
//------------------------------------------------------------------------------

void Jump::validateMembers()
{
    assert(type == Type::Jump);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

