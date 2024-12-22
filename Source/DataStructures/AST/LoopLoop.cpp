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

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

LOOP_LOOP::LOOP_LOOP(int Line, std::string& Filename): LOOP_LOOP(Line, Filename.c_str())
{}
//------------------------------------------------------------------------------

LOOP_LOOP::LOOP_LOOP(int Line, const char* Filename): BASE(Line, Filename, TYPE::LoopLoop)
{
    Count      = 0;
    Statements = 0;
}
//------------------------------------------------------------------------------

LOOP_LOOP::~LOOP_LOOP()
{
    if(Count     ) delete Count;
    if(Statements) delete Statements;
}
//------------------------------------------------------------------------------

BASE* LOOP_LOOP::Copy()
{
    LOOP_LOOP* Copy = new LOOP_LOOP(Source.Line, Source.Filename.c_str());

    if(Count) Copy->Count = (decltype(Count))Count->Copy();

    Copy->Statements = CopyList(Statements);

    return Copy;
}
//------------------------------------------------------------------------------

bool LOOP_LOOP::RunAST()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool LOOP_LOOP::GetVerilog(string& Body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void LOOP_LOOP::Display()
{
    DisplayInfo();
    Debug.Print("loop(");
        if(Count) Count->Display();
        else      Debug.Print("{inf}");
    Debug.Print("){\n");
        if(Statements) Statements->Display();
    Debug.Print("}\n");

    if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void LOOP_LOOP::ValidateMembers()
{
    assert(Type == TYPE::LoopLoop);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

