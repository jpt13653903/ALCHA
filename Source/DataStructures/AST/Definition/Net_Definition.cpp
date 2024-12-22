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

#include "Net_Definition.h"

#include "Netlist/Namespace/Module.h"
#include "Netlist/Synthesisable/Net.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

NET_DEFINITION::NET_DEFINITION(
    int     Line,
    string& Filename
): NET_DEFINITION(Line, Filename.c_str())
{}
//------------------------------------------------------------------------------

NET_DEFINITION::NET_DEFINITION(
    int             Line,
    const char*     Filename
): DEFINITION(Line, Filename, TYPE::Net_Definition)
{
}
//------------------------------------------------------------------------------

NET_DEFINITION::~NET_DEFINITION()
{
}
//------------------------------------------------------------------------------

BASE* NET_DEFINITION::Copy()
{
    NET_DEFINITION* Copy = new NET_DEFINITION(Source.Line, Source.Filename.c_str());

    CopyMembers(Copy);

    return Copy;
}
//------------------------------------------------------------------------------

bool NET_DEFINITION::RunAST()
{
    auto Identifier = Identifiers;

    while(Identifier){
        if(!VerifyNotDefined(Identifier)) return false;

        if(Identifier->Function){
            error("Not yet implemented");
            Identifier = Identifier->Next;
            continue;
        }

        auto Net = new NETLIST::NET(Source.Line, Source.Filename, Identifier->Identifier.c_str());
        Net->Direction = Direction;
        if(!Net->ApplyParameters(Parameters)) Error("Invalid parameters");
        if(!Net->ApplyAttributes(Attributes)) Error("Invalid attributes");
        NETLIST::NamespaceStack.front()->Symbols[Net->Name] = Net;
        if(Identifier->Initialiser){
            if(!Identifier->Initialiser->RunAST()) return false;
        }

        Identifier = Identifier->Next;
    }
    return true;
}
//------------------------------------------------------------------------------

bool NET_DEFINITION::GetVerilog(string& Body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void NET_DEFINITION::Display()
{
    DisplayDefinition("Net");
}
//------------------------------------------------------------------------------

void NET_DEFINITION::ValidateMembers()
{
    assert(Type == TYPE::Net_Definition);

    DEFINITION::ValidateMembers();
}
//------------------------------------------------------------------------------

