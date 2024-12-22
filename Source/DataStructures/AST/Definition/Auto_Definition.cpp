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

#include "Auto_Definition.h"

#include "Netlist/Namespace/Module.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

AUTO_DEFINITION::AUTO_DEFINITION(
    int     Line,
    string& Filename
): AUTO_DEFINITION(Line, Filename.c_str())
{}
//------------------------------------------------------------------------------

AUTO_DEFINITION::AUTO_DEFINITION(
    int             Line,
    const char*     Filename
): DEFINITION(Line, Filename, TYPE::Auto_Definition)
{
}
//------------------------------------------------------------------------------

AUTO_DEFINITION::~AUTO_DEFINITION()
{
}
//------------------------------------------------------------------------------

BASE* AUTO_DEFINITION::Copy()
{
    AUTO_DEFINITION* Copy = new AUTO_DEFINITION(Source.Line, Source.Filename.c_str());

    CopyMembers(Copy);

    return Copy;
}
//------------------------------------------------------------------------------

bool AUTO_DEFINITION::RunAST()
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

bool AUTO_DEFINITION::GetVerilog(string& Body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void AUTO_DEFINITION::Display()
{
    DisplayDefinition("Auto");
}
//------------------------------------------------------------------------------

void AUTO_DEFINITION::ValidateMembers()
{
    assert(Type == TYPE::Auto_Definition);

    assert(Parameters.empty());

    DEFINITION::ValidateMembers();
}
//------------------------------------------------------------------------------

