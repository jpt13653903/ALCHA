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

#include "String.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

STRING::STRING(int Line, const string& Filename): STRING(Line, Filename.c_str())
{}
//------------------------------------------------------------------------------

STRING::STRING(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::String)
{
}
//------------------------------------------------------------------------------

STRING::~STRING()
{
}
//------------------------------------------------------------------------------

BASE* STRING::Copy()
{
    STRING* Copy = new STRING(Source.Line, Source.Filename.c_str());

    Copy->Value = Value;

    if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
    if(Right) Copy->Right = (decltype(Right))Right->Copy();

    return Copy;
}
//------------------------------------------------------------------------------

bool STRING::GetVerilog(string& Body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

EXPRESSION* STRING::Evaluate()
{
    error("Not yet implemented");
    return this;
//   EXPRESSION* Result = 0;
//
//   Result = (EXPRESSION*)Copy();
//
//   if(!Result) return 0;
//   return Result->Simplify(false);
}
//------------------------------------------------------------------------------

int STRING::GetWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

NUMBER& STRING::GetFullScale()
{
    error("Not yet implemented");
    static NUMBER zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool STRING::GetSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool STRING::HasCircularReference(NETLIST::BASE* Object)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void STRING::PopulateUsed()
{
    error("Not yet implemented");
}
//------------------------------------------------------------------------------

EXPRESSION* STRING::RemoveTempNet(int Width, bool Signed)
{
    error("Not yet implemented");
    return this;
}
//------------------------------------------------------------------------------

void STRING::Display()
{
    DisplayStart();

    Debug.Print("\"%s\"", Value.c_str());

    DisplayEnd();
}
//------------------------------------------------------------------------------

void STRING::ValidateMembers()
{
    assert(Type == TYPE::String);

    assert(!Next);
    assert(!Prev);

    assert(!Left );
    assert(!Right);
}
//------------------------------------------------------------------------------

