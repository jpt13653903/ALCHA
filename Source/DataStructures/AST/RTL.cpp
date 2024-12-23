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

#include "RTL.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Rtl::Rtl(int line, std::string& filename): Rtl(line, filename.c_str()){}
//------------------------------------------------------------------------------

Rtl::Rtl(int line, const char* filename): Base(line, filename, Type::RTL)
{
    statements = 0;
}
//------------------------------------------------------------------------------

Rtl::~Rtl()
{
    if(statements) delete statements;

    for(auto parameter: parameters){
        if(parameter) delete parameter;
    }
}
//------------------------------------------------------------------------------

Base* Rtl::copy()
{
    Rtl* copy = new Rtl(source.line, source.filename.c_str());

    copy->statements = (decltype(statements))copyList(statements);

    for(auto parameter: parameters){
        if(parameter) copy->parameters.push_back(parameter->copy());
    }

    return copy;
}
//------------------------------------------------------------------------------

bool Rtl::runAST()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool Rtl::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Rtl::display()
{
    displayInfo();
    logger.print("rtl(");
        bool isFirst = true;
        for(auto parameter: parameters){
            if(isFirst) logger.print(", ");
            isFirst = false;
            if(parameter) parameter->display();
        }
    logger.print("){\n");
        if(statements) statements->display();
    logger.print("}\n");

    if(next) next->display();
}
//------------------------------------------------------------------------------

void Rtl::validateMembers()
{
    assert(type == Type::RTL);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

