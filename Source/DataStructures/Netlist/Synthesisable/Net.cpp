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

#include "Net.h"

#include "AST/Expression/Literal.h"
#include "AST/Expression/Object.h"
//------------------------------------------------------------------------------

using std::string;
using namespace Netlist;
//------------------------------------------------------------------------------

Net::Net(int line, const string& filename, const char* name) : Synthesisable(line, filename, name, Type::Net)
{
    value = 0;
}
//------------------------------------------------------------------------------

Net::~Net()
{
    if(value) delete value;
}
//------------------------------------------------------------------------------

AST::Expression* Net::getExpression(int line, const string& filename)
{
    if(value) return (AST::Expression*)value->copy();
    ::printError(line, filename, "Operate-assign on empty object");
    return 0;
}
//------------------------------------------------------------------------------

bool Net::assign(AST::Expression* expression)
{
    assert(expression, return false);

    expression = expression->evaluate();
    if(!expression) return false;

    switch(expression->type){
        case AST::Base::Type::Literal:{
            Number scale = 1;
            scale.binScale(width());
            scale.div(fullScale());
            ((AST::Literal*)expression)->value.mul(scale);
            break;
        }

        case AST::Base::Type::Object:{
            auto object = ((AST::Object*)expression)->objectRef;
            assert(object, return false);

            int    exprWidth     = object->width    ();
            Number exprFullScale = object->fullScale();

            Number scale = 1;
            scale.binScale(width() - exprWidth);
            scale.mul(exprFullScale);
            scale.div(fullScale());

            expression = expression->scaleWith(scale, width(), fullScale());
            break;
        }

        default:
            printError("Cannot assign this type to a net");
            delete expression;
            return false;
    }
    return rawAssign(expression);
}
//------------------------------------------------------------------------------

bool Net::rawAssign(AST::Expression* expression)
{
    if(!expression) return false;

    if(value){
        expression->printWarning();
        printf("Overwriting net value %s\n", name.c_str());
        delete value;
    }
    value = expression->evaluate();
    if(value && value->hasCircularReference(this)){
        value->printError("Circular combinational circuit");
    }
    return value;
}
//------------------------------------------------------------------------------

bool Net::hasCircularReference(Base* object)
{
    if(this == object) return true;
    if(!value) return false;
    return value->hasCircularReference(object);
}
//------------------------------------------------------------------------------

void Net::populateUsed(bool setUsed)
{
    if(used) return; // Prevents circular loops
    used = setUsed;
    if(value) value->populateUsed();
}
//------------------------------------------------------------------------------

void Net::display(int indent)
{
    logger.indent(indent);
    logger.print("Net: ");

    indent++;
    displayParameters(indent);

    logger.indent(indent);
    logger.print("value      = ");
    if(value) value->display();
    else      logger.print("{open}");
    logger.print("\n");

    displayAttributes(indent);
}
//------------------------------------------------------------------------------

void Net::validate()
{
    assert(type == Type::Net);

    Synthesisable::validate();

    if(value) value->validate();
}
//------------------------------------------------------------------------------

