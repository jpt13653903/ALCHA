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

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

FENCE::FENCE(int Line, std::string& Filename): FENCE(Line, Filename.c_str())
{}
//------------------------------------------------------------------------------

FENCE::FENCE(int Line, const char* Filename): BASE(Line, Filename, TYPE::Fence)
{
}
//------------------------------------------------------------------------------

FENCE::~FENCE()
{
}
//------------------------------------------------------------------------------

BASE* FENCE::Copy()
{
    FENCE* Copy = new FENCE(Source.Line, Source.Filename.c_str());

    return Copy;
}
//------------------------------------------------------------------------------

bool FENCE::RunAST()
{
    // Fences are ignored during this stage.
    // They only have significance within FSM constructs
    return true;
}
//------------------------------------------------------------------------------

bool FENCE::GetVerilog(string& Body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void FENCE::Display()
{
    DisplayInfo();
    Debug.Print("Fence\n");

    if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void FENCE::ValidateMembers()
{
    assert(Type == TYPE::Fence);
}
//------------------------------------------------------------------------------

