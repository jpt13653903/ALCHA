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

#include "Class_Instance.h"

#include "Netlist/Namespace/Module.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

CLASS_INSTANCE::CLASS_INSTANCE(
    int     Line,
    string& Filename
): CLASS_INSTANCE(Line, Filename.c_str())
{}
//------------------------------------------------------------------------------

CLASS_INSTANCE::CLASS_INSTANCE(
    int             Line,
    const char*     Filename
): DEFINITION(Line, Filename, TYPE::Class_Instance)
{
    ClassName = 0;
}
//------------------------------------------------------------------------------

CLASS_INSTANCE::~CLASS_INSTANCE()
{
    if(ClassName) delete ClassName;
}
//------------------------------------------------------------------------------

BASE* CLASS_INSTANCE::Copy()
{
    CLASS_INSTANCE* Copy = new CLASS_INSTANCE(Source.Line, Source.Filename.c_str());

    CopyMembers(Copy);

    if(ClassName) Copy->ClassName = (decltype(ClassName))ClassName->Copy();

    return Copy;
}
//------------------------------------------------------------------------------

bool CLASS_INSTANCE::RunAST()
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

bool CLASS_INSTANCE::GetVerilog(string& Body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void CLASS_INSTANCE::Display()
{
    DisplayInfo();
    Debug.Print("Definition (");

    Debug.Print("Class instance definition (");
    if(ClassName) ClassName->Display();
    else          Debug.Print("Class instance with no class name");
    Debug.Print("):\n");

    DisplayParameters ();
    DisplayAttributes ();
    DisplayIdentifiers();

    if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void CLASS_INSTANCE::ValidateMembers()
{
    assert(Type == TYPE::Class_Instance);

    if(ClassName) ClassName->Validate();

    DEFINITION::ValidateMembers();
}
//------------------------------------------------------------------------------

