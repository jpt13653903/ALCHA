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

#include "Literal.h"
//------------------------------------------------------------------------------

using std::string;
using std::to_string;
using namespace AST;
//------------------------------------------------------------------------------

Literal::Literal(int line, const string& filename): Literal(line, filename.c_str()){}
//------------------------------------------------------------------------------

Literal::Literal(int line, const char* filename): Expression(line, filename, Type::Literal){}
//------------------------------------------------------------------------------

Literal::~Literal(){}
//------------------------------------------------------------------------------

Base* Literal::copy()
{
    Literal* copy = new Literal(source.line, source.filename.c_str());

    copy->value = value;

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool Literal::getVerilog(string& body)
{
    if(!value.isReal()){
        printError("non-real literal");
        return false;
    }

    int  width  = getWidth ();
    bool isSigned = getSigned();

    Number result;
    if(isSigned){
        body += to_string(width+1) + "'sh";
        result = getFullScale();
        result.binScale(1);
        result.add(value);
    }else{
        body += to_string(width  ) + "'h";
        result = value;
    }
    result.round();
    body += result.getString(16);
    result.binScale(-width);
    if((!isSigned && result >= 1) || (isSigned && result > 2)){
        printError("The literal does not fit in its full-scale range");
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------

Expression* Literal::evaluate()
{
    return this;
}
//------------------------------------------------------------------------------

void Literal::setWidth(int width)
{
    widthOverride = width;
}
//------------------------------------------------------------------------------

int Literal::getWidth()
{
    if(widthOverride) return widthOverride;

    int width = 0;

    Number num = value;
    if(num < 0) num.mul(-1);
    num.round();

    while(num > 1){
        num.binScale(-1);
        width++;
    }
    if(!getSigned() && num == 1){
        num.binScale(-1);
        width++;
    }
    if(!width) return 1;
    return width;
}
//------------------------------------------------------------------------------

Number& Literal::getFullScale()
{
    static Number result;
    result = 1;
    result.binScale(getWidth());
    return result;
}
//------------------------------------------------------------------------------

bool Literal::getSigned()
{
    return value < 0;
}
//------------------------------------------------------------------------------

bool Literal::hasCircularReference(Netlist::Base* object)
{
    return false;
}
//------------------------------------------------------------------------------

void Literal::populateUsed(){}
//------------------------------------------------------------------------------

Expression* Literal::removeTempNet(int width, bool isSigned)
{
    return this;
}
//------------------------------------------------------------------------------

void Literal::display()
{
    displayStart();

    debug.print(value.display());

    displayEnd();
}
//------------------------------------------------------------------------------

void Literal::validateMembers()
{
    assert(type == Type::Literal);

    assert(!next);
    assert(!prev);

    assert(!left );
    assert(!right);
}
//------------------------------------------------------------------------------

