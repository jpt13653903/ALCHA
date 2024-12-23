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

#include "DivideAssign.h"

#include "Netlist/Byte.h"
#include "Netlist/Character.h"
#include "Netlist/Num.h"
#include "Netlist/Synthesisable/Pin.h"
#include "Netlist/Synthesisable/Net.h"

#include "../Expression/Divide.h"
#include "../Expression/Object.h"
#include "../Expression/Literal.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

DivideAssign::DivideAssign(int line, std::string filename):
    DivideAssign(line, filename.c_str()){}
//------------------------------------------------------------------------------

DivideAssign::DivideAssign(int line, const char* filename):
    Assignment(line, filename, Type::DivideAssign){}
//------------------------------------------------------------------------------

DivideAssign::~DivideAssign(){}
//------------------------------------------------------------------------------

Base* DivideAssign::copy()
{
    DivideAssign* copy = new DivideAssign(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool DivideAssign::runAST()
{
    TargetList targetList;

    assert(left , return false);
    assert(right, return false);

    if(!getLHS(left, targetList)) return false;
    if(targetList.empty()){
        printError("target object list is empty");
        return false;
    }

    if(targetList.size() > 1){
        error("Multiple assignment targets not supported yet");
        return false;
    }

    Netlist::Base* target = targetList.front();
    assert(target, return false);

    // Move the expression
    Expression* expression = new Divide(right->source.line, right->source.filename);
    expression->left = target->getExpression(right->source.line, right->source.filename);

    if(!expression->left){
        delete expression;
        return false;
    }

    expression->right = right;
    right = 0;

    bool result = target->assign(expression);

    return result;
}
//------------------------------------------------------------------------------

bool DivideAssign::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void DivideAssign::display()
{
    displayAssignment("/=");
}
//------------------------------------------------------------------------------

void DivideAssign::validateMembers()
{
    assert(type == Type::DivideAssign);

    assert(left, return);
    left->validate();

    if(right) right->validate();
}
//------------------------------------------------------------------------------

