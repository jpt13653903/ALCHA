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

#include "AppendAssign.h"

#include "Netlist/Byte.h"
#include "Netlist/Character.h"
#include "Netlist/Num.h"
#include "Netlist/Synthesisable/Pin.h"
#include "Netlist/Synthesisable/Net.h"

#include "../Expression/ArrayConcatenate.h"
#include "../Expression/Object.h"
#include "../Expression/Literal.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

AppendAssign::AppendAssign(int line, std::string filename):
    AppendAssign(line, filename.c_str()){}
//------------------------------------------------------------------------------

AppendAssign::AppendAssign(int line, const char* filename):
    Assignment(line, filename, Type::AppendAssign){}
//------------------------------------------------------------------------------

AppendAssign::~AppendAssign(){}
//------------------------------------------------------------------------------

Base* AppendAssign::copy()
{
    AppendAssign* copy = new AppendAssign(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool AppendAssign::runAST()
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
    ArrayConcatenate* expression = new ArrayConcatenate(right->source.line, right->source.filename);
    error("Not yet implemented"); // TODO: Should append in-place if left is an array, otherwise create a new array
    expression->elements.push_back(target->getExpression(right->source.line, right->source.filename));

    if(!expression->elements.back()){
        delete expression;
        return false;
    }

    expression->elements.push_back(right);
    right = 0;

    bool result = target->assign(expression);

    return result;
}
//------------------------------------------------------------------------------

bool AppendAssign::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void AppendAssign::display()
{
    displayAssignment("~=");
}
//------------------------------------------------------------------------------

void AppendAssign::validateMembers()
{
    assert(type == Type::AppendAssign);

    assert(left, return);
    left->validate();

    if(right) right->validate();
}
//------------------------------------------------------------------------------

