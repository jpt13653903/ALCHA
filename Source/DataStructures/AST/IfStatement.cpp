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

#include "IfStatement.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

IfStatement::IfStatement(int line, std::string& filename): IfStatement(line, filename.c_str()){}
//------------------------------------------------------------------------------

IfStatement::IfStatement(int line, const char* filename): Base(line, filename, Type::IfStatement)
{
    condition       = 0;
    trueStatements  = 0;
    falseStatements = 0;
}
//------------------------------------------------------------------------------

IfStatement::~IfStatement()
{
    if(condition      ) delete condition;
    if(trueStatements ) delete trueStatements;
    if(falseStatements) delete falseStatements;
}
//------------------------------------------------------------------------------

Base* IfStatement::copy()
{
    IfStatement* copy = new IfStatement(source.line, source.filename.c_str());

    if(condition) copy->condition = (decltype(condition))condition->copy();

    copy->trueStatements  = copyList(trueStatements );
    copy->falseStatements = copyList(falseStatements);

    return copy;
}
//------------------------------------------------------------------------------

bool IfStatement::runAST()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool IfStatement::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void IfStatement::display()
{
    displayInfo();
    debug.print("if(");
        if(condition) condition->display();
    debug.print("){\n");
        if(trueStatements) trueStatements->display();
    debug.print("}else{\n");
        if(falseStatements) falseStatements->display();
    debug.print("}\n");

    if(next) next->display();
}
//------------------------------------------------------------------------------

void IfStatement::validateMembers()
{
    assert(type == Type::IfStatement);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

