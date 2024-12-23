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

#include "FSM.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Fsm::Fsm(int line, std::string& filename): Fsm(line, filename.c_str()){}
//------------------------------------------------------------------------------

Fsm::Fsm(int line, const char* filename): Base(line, filename, Type::FSM)
{
    statements = 0;
}
//------------------------------------------------------------------------------

Fsm::~Fsm()
{
    if(statements) delete statements;

    for(auto parameter: parameters){
        if(parameter) delete parameter;
    }
}
//------------------------------------------------------------------------------

Base* Fsm::copy()
{
    Fsm* copy = new Fsm(source.line, source.filename.c_str());

    copy->statements = copyList(statements);

    for(auto parameter: parameters){
        if(parameter) copy->parameters.push_back(parameter->copy());
    }

    return copy;
}
//------------------------------------------------------------------------------

bool Fsm::runAST()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool Fsm::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Fsm::display()
{
    displayInfo();
    debug.print("fsm(");
        bool isFirst = true;
        for(auto parameter: parameters){
            if(isFirst) debug.print(", ");
            isFirst = false;
            if(parameter) parameter->display();
        }
    debug.print("){\n");
        if(statements) statements->display();
    debug.print("}\n");

    if(next) next->display();
}
//------------------------------------------------------------------------------

void Fsm::validateMembers()
{
    assert(type == Type::FSM);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

