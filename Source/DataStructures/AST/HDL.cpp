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

#include "HDL.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

HDL::HDL(int Line, std::string& Filename): HDL(Line, Filename.c_str())
{}
//------------------------------------------------------------------------------

HDL::HDL(int Line, const char* Filename): BASE(Line, Filename, TYPE::HDL)
{
    Files      = 0;
    Ports      = 0;
    Parameters = 0;
}
//------------------------------------------------------------------------------

HDL::~HDL()
{
    if(Files     ) delete Files;
    if(Ports     ) delete Ports;
    if(Parameters) delete Parameters;
}
//------------------------------------------------------------------------------

BASE* HDL::Copy()
{
    HDL* Copy = new HDL(Source.Line, Source.Filename.c_str());

    Copy->Identifier = Identifier;

    Copy->Files      = (decltype(Files     ))CopyList(Files);
    Copy->Ports      = (decltype(Ports     ))CopyList(Ports);
    Copy->Parameters = (decltype(Parameters))CopyList(Parameters);

    return Copy;
}
//------------------------------------------------------------------------------

bool HDL::RunAST()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool HDL::GetVerilog(string& Body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void HDL::Display()
{
    DisplayInfo();
    Debug.Print("hdl (%s):\n", Identifier.c_str());
    Debug.Print(" Files:\n  "     ); if(Files     ) Files     ->Display(); Debug.Print("\n");
    Debug.Print(" Parameters:\n  "); if(Parameters) Parameters->Display();
    Debug.Print(" Ports:\n  "     ); if(Ports     ) Ports     ->Display();

    if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void HDL::ValidateMembers()
{
    assert(Type == TYPE::HDL);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

