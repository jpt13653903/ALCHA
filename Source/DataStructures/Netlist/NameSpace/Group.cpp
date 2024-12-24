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

#include "Group.h"
//------------------------------------------------------------------------------

using namespace Netlist;
//------------------------------------------------------------------------------

Netlist::Group::Group(int line, const std::string& filename, const char* name):
    NameSpace(line, filename, name, Type::Group){}
//------------------------------------------------------------------------------

Netlist::Group::~Group(){}
//------------------------------------------------------------------------------

void Netlist::Group::display(int indent)
{
    logger.indent(indent);
    logger.print("Group: ");

    if(name.empty()) logger.print("{Anonymous}");
    else             displayLongName();
    logger.print("\n");

    indent++;
    displayAttributes(indent);
    logger.print("\n");

    for(auto s: symbols){
        if(s.second){
            s.second->display(indent);
        }else{
            logger.indent(indent);
            logger.print("- %s: {null}\n", s.first.c_str());
        }
        logger.print("\n");
    }
}
//------------------------------------------------------------------------------

void Netlist::Group::validate()
{
    assert(type == Type::Group);

    NameSpace::validate();
}
//------------------------------------------------------------------------------

