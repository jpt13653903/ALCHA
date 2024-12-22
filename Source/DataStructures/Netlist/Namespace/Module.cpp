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

#include "Module.h"
//------------------------------------------------------------------------------

namespace NETLIST{
    std::list<NAMESPACE*> NamespaceStack; // Initialised in Engine::Run()
    MODULE                Global(0, "");
}
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

MODULE::MODULE(int Line, const std::string& Filename, const char* Name):
NAMESPACE(Line, Filename, Name, TYPE::Module)
{
}
//------------------------------------------------------------------------------

MODULE::~MODULE()
{
}
//------------------------------------------------------------------------------

void MODULE::Display(int Indent)
{
    Debug.Indent(Indent);
    Debug.Print("Module: ");

    if(this == &Global) Debug.Print("{Global}");
    else                DisplayLongName();
    Debug.Print("\n");
    Indent++;

    DisplayAttributes(Indent);
    Debug.Print("\n");

    list<BASE*> Modules;

    foreach(s, Symbols){
        if(s->second){
            if(s->second->Type == TYPE::Module) Modules.push_back(s->second);
            else s->second->Display(Indent);
        }else{
            Debug.Indent(Indent);
            Debug.Print("- %s: {null}\n", s->first.c_str());
        }
        Debug.Print("\n");
    }

    foreach(s, Modules){
        (*s)->Display(Indent);
        Debug.Print("\n");
    }
}
//------------------------------------------------------------------------------

void MODULE::Validate()
{
    assert(Type == TYPE::Module);

    NAMESPACE::Validate();
}
//------------------------------------------------------------------------------

