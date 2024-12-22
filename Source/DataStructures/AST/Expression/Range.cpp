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

#include "Range.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

RANGE::RANGE(int Line, const string& Filename): RANGE(Line, Filename.c_str())
{}
//------------------------------------------------------------------------------

RANGE::RANGE(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Range)
{
    Step = 0;
}
//------------------------------------------------------------------------------

RANGE::~RANGE()
{
    if(Step) delete Step;
}
//------------------------------------------------------------------------------

BASE* RANGE::Copy()
{
    RANGE* Copy = new RANGE(Source.Line, Source.Filename.c_str());

    if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
    if(Right) Copy->Right = (decltype(Right))Right->Copy();
    if(Step ) Copy->Step  = (decltype(Step ))Step ->Copy();

    return Copy;
}
//------------------------------------------------------------------------------

bool RANGE::GetVerilog(string& Body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

EXPRESSION* RANGE::Evaluate()
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

int RANGE::GetWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

NUMBER& RANGE::GetFullScale()
{
    error("Not yet implemented");
    static NUMBER zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool RANGE::GetSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool RANGE::HasCircularReference(NETLIST::BASE* Object)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void RANGE::PopulateUsed()
{
    error("Not yet implemented");
}
//------------------------------------------------------------------------------

EXPRESSION* RANGE::RemoveTempNet(int Width, bool Signed)
{
    error("Not yet implemented");
    return this;
}
//------------------------------------------------------------------------------

void RANGE::Display()
{
    DisplayStart();

    Debug.Print("..");

    DisplayEnd();

    if(Step){
        Debug.Print(":");
        if(Step->Left || Step->Right) Debug.Print("(");
        Step->Display();
        if(Step->Left || Step->Right) Debug.Print(")");
    }
}
//------------------------------------------------------------------------------

void RANGE::ValidateMembers()
{
    assert(Type == TYPE::Range);

    assert(!Next);
    assert(!Prev);

    // TODO: assert(!Left );
    // TODO: assert(!Right);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

