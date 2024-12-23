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

#include "Num.h"

#include "AST/Expression/Literal.h"
//------------------------------------------------------------------------------

using std::string;
using namespace Netlist;
//------------------------------------------------------------------------------

Num::Num(int line, const string& filename, const char* name) : Base(line, filename, name, Type::Number)
{
    value = 0;
}
//------------------------------------------------------------------------------

Num::~Num(){}
//------------------------------------------------------------------------------

AST::Expression* Num::getExpression(int line, const string& filename)
{
    AST::Literal* result = new AST::Literal(line, filename);
    result->value = value;
    return result;
}
//------------------------------------------------------------------------------

bool Num::assign(AST::Expression* expression)
{
    return rawAssign(expression);
}
//------------------------------------------------------------------------------

bool Num::rawAssign(AST::Expression* expression)
{
    if(!expression) return false;

    expression = expression->evaluate();
    if(!expression) return false;

    bool result = false;

    if(expression->type == AST::Base::Type::Literal){
        value = ((AST::Literal*)expression)->value;
        result = true;
    }else{
        expression->printError("expression must evaluate to a literal");
        result = false;
    }

    // expression has been moved and then used, so it must be deleted
    delete expression;
    return result;
}
//------------------------------------------------------------------------------

bool Num::hasCircularReference(Base* object)
{
    if(this == object) return true;
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Num::display(int indent)
{
    logger.indent(indent);
    logger.print("Num: %s\n", name.c_str());

    logger.indent(indent+1);
    logger.print("value = ");
    logger.print(value.display());
    logger.print("\n");
}
//------------------------------------------------------------------------------

void Num::validate()
{
    assert(type == Type::Number);

    Base::validate();
}
//------------------------------------------------------------------------------

