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

#include "Fence.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Fence::Fence(int line, std::string& filename): Fence(line, filename.c_str()){}
//------------------------------------------------------------------------------

Fence::Fence(int line, const char* filename): Base(line, filename, Type::Fence){}
//------------------------------------------------------------------------------

Fence::~Fence(){}
//------------------------------------------------------------------------------

Base* Fence::copy()
{
    Fence* copy = new Fence(source.line, source.filename.c_str());

    return copy;
}
//------------------------------------------------------------------------------

bool Fence::runAST()
{
    // Fences are ignored during this stage.
    // They only have significance within FSM constructs
    return true;
}
//------------------------------------------------------------------------------

bool Fence::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Fence::display()
{
    displayInfo();
    logger.print("Fence\n");

    if(next) next->display();
}
//------------------------------------------------------------------------------

void Fence::validateMembers()
{
    assert(type == Type::Fence);
}
//------------------------------------------------------------------------------

