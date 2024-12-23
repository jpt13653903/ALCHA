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

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Hdl::Hdl(int line, std::string& filename): Hdl(line, filename.c_str()){}
//------------------------------------------------------------------------------

Hdl::Hdl(int line, const char* filename): Base(line, filename, Type::HDL)
{
    files      = 0;
    ports      = 0;
    parameters = 0;
}
//------------------------------------------------------------------------------

Hdl::~Hdl()
{
    if(files     ) delete files;
    if(ports     ) delete ports;
    if(parameters) delete parameters;
}
//------------------------------------------------------------------------------

Base* Hdl::copy()
{
    Hdl* copy = new Hdl(source.line, source.filename.c_str());

    copy->identifier = identifier;

    copy->files      = (decltype(files     ))copyList(files);
    copy->ports      = (decltype(ports     ))copyList(ports);
    copy->parameters = (decltype(parameters))copyList(parameters);

    return copy;
}
//------------------------------------------------------------------------------

bool Hdl::runAST()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool Hdl::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Hdl::display()
{
    displayInfo();
    debug.print("hdl (%s):\n", identifier.c_str());
    debug.print(" Files:\n  "     ); if(files     ) files     ->display(); debug.print("\n");
    debug.print(" Parameters:\n  "); if(parameters) parameters->display();
    debug.print(" Ports:\n  "     ); if(ports     ) ports     ->display();

    if(next) next->display();
}
//------------------------------------------------------------------------------

void Hdl::validateMembers()
{
    assert(type == Type::HDL);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

