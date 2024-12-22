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

#include "NOR_Reduce.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

NOR_REDUCE::NOR_REDUCE(int Line, const string& Filename): NOR_REDUCE(Line, Filename.c_str())
{}
//------------------------------------------------------------------------------

NOR_REDUCE::NOR_REDUCE(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::NOR_Reduce)
{
}
//------------------------------------------------------------------------------

NOR_REDUCE::~NOR_REDUCE()
{
}
//------------------------------------------------------------------------------

BASE* NOR_REDUCE::Copy()
{
    NOR_REDUCE* Copy = new NOR_REDUCE(Source.Line, Source.Filename.c_str());

    if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
    if(Right) Copy->Right = (decltype(Right))Right->Copy();

    return Copy;
}
//------------------------------------------------------------------------------

bool NOR_REDUCE::GetVerilog(string& Body)
{
    Body += "~|(";
    Right->GetVerilog(Body);
    Body += ")";

    return true;
}
//------------------------------------------------------------------------------

EXPRESSION* NOR_REDUCE::Evaluate()
{
    error("Not yet implemented");
    return this;
//   EXPRESSION* Result = 0;
//
//   error("Not yet implemented");
//
//   if(!Result) return 0;
//   return Result->Simplify(false);
}
//------------------------------------------------------------------------------

int NOR_REDUCE::GetWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

NUMBER& NOR_REDUCE::GetFullScale()
{
    error("Not yet implemented");
    static NUMBER zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool NOR_REDUCE::GetSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool NOR_REDUCE::HasCircularReference(NETLIST::BASE* Object)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void NOR_REDUCE::PopulateUsed()
{
    error("Not yet implemented");
}
//------------------------------------------------------------------------------

EXPRESSION* NOR_REDUCE::RemoveTempNet(int Width, bool Signed)
{
    error("Not yet implemented");
    return this;
}
//------------------------------------------------------------------------------

void NOR_REDUCE::Display()
{
    DisplayStart();

    Debug.Print(" ~|");

    DisplayEnd();
}
//------------------------------------------------------------------------------

void NOR_REDUCE::ValidateMembers()
{
    assert(Type == TYPE::NOR_Reduce);

    assert(!Next);
    assert(!Prev);

    // TODO: assert(!Left );
    // TODO: assert(!Right);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

