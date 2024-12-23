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

#include "FunctionCall.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

FunctionCall::FunctionCall(int line, const string& filename): FunctionCall(line, filename.c_str()){}
//------------------------------------------------------------------------------

FunctionCall::FunctionCall(int line, const char* filename): Expression(line, filename, Type::FunctionCall){}
//------------------------------------------------------------------------------

FunctionCall::~FunctionCall()
{
    for(auto parameter: parameters){
        if(parameter) delete parameter;
    }
}
//------------------------------------------------------------------------------

Base* FunctionCall::copy()
{
    FunctionCall* copy = new FunctionCall(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    for(auto parameter: parameters){
        if(parameter) copy->parameters.push_back(parameter->copy());
    }

    return copy;
}
//------------------------------------------------------------------------------

bool FunctionCall::runAST()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool FunctionCall::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

Expression* FunctionCall::evaluate()
{
    error("Not yet implemented");
    return this;
//   Expression* result = 0;
//
//   error("Not yet implemented");
//
//   if(!result) return 0;
//   return result->simplify(false);
}
//------------------------------------------------------------------------------

int FunctionCall::getWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

Number& FunctionCall::getFullScale()
{
    error("Not yet implemented");
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool FunctionCall::getSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool FunctionCall::hasCircularReference(Netlist::Base* object)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void FunctionCall::populateUsed()
{
    error("Not yet implemented");
}
//------------------------------------------------------------------------------

Expression* FunctionCall::removeTempNet(int width, bool isSigned)
{
    error("Not yet implemented");
    return this;
}
//------------------------------------------------------------------------------

void FunctionCall::display()
{
    displayStart();

    debug.print("{call}(");

    bool isFirst = true;
    for(auto parameter: parameters){
        if(!isFirst) debug.print(", ");
        isFirst = false;
        parameter->display();
    }
    debug.print(")");

    displayEnd();
}
//------------------------------------------------------------------------------

void FunctionCall::validateMembers()
{
    assert(type == Type::FunctionCall);

    assert(!next);
    assert(!prev);

    assert(!left );
    assert(!right);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

