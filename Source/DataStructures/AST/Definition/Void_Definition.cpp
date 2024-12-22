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

#include "Void_Definition.h"

#include "Netlist/Namespace/Module.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

VOID_DEFINITION::VOID_DEFINITION(
    int     Line,
    string& Filename
): VOID_DEFINITION(Line, Filename.c_str())
{}
//------------------------------------------------------------------------------

VOID_DEFINITION::VOID_DEFINITION(
    int             Line,
    const char*     Filename
): DEFINITION(Line, Filename, TYPE::Void_Definition)
{
}
//------------------------------------------------------------------------------

VOID_DEFINITION::~VOID_DEFINITION()
{
}
//------------------------------------------------------------------------------

BASE* VOID_DEFINITION::Copy()
{
    VOID_DEFINITION* Copy = new VOID_DEFINITION(Source.Line, Source.Filename.c_str());

    CopyMembers(Copy);

    return Copy;
}
//------------------------------------------------------------------------------

bool VOID_DEFINITION::RunAST()
{
    auto Identifier = Identifiers;

    while(Identifier){
        if(!VerifyNotDefined(Identifier)) return false;

        if(Identifier->Function){
            error("Not yet implemented");
            Identifier = Identifier->Next;
            continue;
        }

        error("Not yet implemented");

        Identifier = Identifier->Next;
    }
    return true;
}
//------------------------------------------------------------------------------

bool VOID_DEFINITION::GetVerilog(string& Body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void VOID_DEFINITION::Display()
{
    DisplayDefinition("Void");
}
//------------------------------------------------------------------------------

void VOID_DEFINITION::ValidateMembers()
{
    assert(Type == TYPE::Void_Definition);

    assert(Parameters.empty());

    DEFINITION::ValidateMembers();
}
//------------------------------------------------------------------------------

