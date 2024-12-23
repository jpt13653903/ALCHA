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

#include "module.h"
//------------------------------------------------------------------------------

namespace Netlist{
    std::list<NameSpace*> nameSpaceStack; // Initialised in Engine::run()
    Module                global(0, "");
}
//------------------------------------------------------------------------------

using std::list;
using namespace Netlist;
//------------------------------------------------------------------------------

Module::Module(int line, const std::string& filename, const char* name):
    NameSpace(line, filename, name, Type::Module){}
//------------------------------------------------------------------------------

Module::~Module(){}
//------------------------------------------------------------------------------

void Module::display(int indent)
{
    debug.indent(indent);
    debug.print("module: ");

    if(this == &global) debug.print("{global}");
    else                displayLongName();
    debug.print("\n");
    indent++;

    displayAttributes(indent);
    debug.print("\n");

    list<Base*> modules;

    for(auto s: symbols){
        if(s.second){
            if(s.second->type == Type::Module) modules.push_back(s.second);
            else s.second->display(indent);
        }else{
            debug.indent(indent);
            debug.print("- %s: {null}\n", s.first.c_str());
        }
        debug.print("\n");
    }

    for(auto s: modules){
        s->display(indent);
        debug.print("\n");
    }
}
//------------------------------------------------------------------------------

void Module::validate()
{
    assert(type == Type::Module);

    NameSpace::validate();
}
//------------------------------------------------------------------------------

