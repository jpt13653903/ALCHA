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

#include "Switch.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Switch::Case::Case()
{
    next       = 0;
    statements = 0;
}
//------------------------------------------------------------------------------

Switch::Case::Case(const Case& source)
{
    if(source.next) next = new Case(*source.next);

    statements = copyList(source.statements);

    for(auto Element: source.expressions){
        expressions.push_back((Expression*)Element->copy());
    }
}
//------------------------------------------------------------------------------

Switch::Case::~Case()
{
    if(next       ) delete next;
    if(statements ) delete statements;

    for(auto Element: expressions) delete Element;
}
//------------------------------------------------------------------------------

Switch::Switch(int line, std::string& filename): Switch(line, filename.c_str()){}
//------------------------------------------------------------------------------

Switch::Switch(int line, const char* filename): Base(line, filename, Type::Switch)
{
    cases       = 0;
    defaultCase = 0;
    expression  = 0;
}
//------------------------------------------------------------------------------

Switch::~Switch()
{
    if(cases      ) delete cases;
    if(defaultCase) delete defaultCase;
    if(expression ) delete expression;
}
//------------------------------------------------------------------------------

Base* Switch::copy()
{
    Switch* copy = new Switch(source.line, source.filename.c_str());

    if(cases     ) copy->cases      = new Case(*cases);
    if(expression) copy->expression = (decltype(expression))expression->copy();

    copy->defaultCase = copyList(defaultCase);

    return copy;
}
//------------------------------------------------------------------------------

bool Switch::runAST()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool Switch::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Switch::display()
{
    displayInfo();
    logger.print("switch(");
        if(expression) expression->display();
    logger.print("){\n");
        Case* temp = cases;
        while(temp){
            logger.print(" case(");
                bool isFirst = true;
                for(auto expression: temp->expressions){
                    if(!isFirst) logger.print(", ");
                    expression->display();
                    isFirst = false;
                }
            logger.print("){\n");
                if(temp->statements) temp->statements->display();
            logger.print(" }\n");
            temp = temp->next;
        }
        logger.print(" default{\n");
            if(defaultCase) defaultCase->display();
        logger.print(" }\n");
    logger.print("}\n");

    if(next) next->display();
}
//------------------------------------------------------------------------------

void Switch::validateMembers()
{
    assert(type == Type::Switch);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

