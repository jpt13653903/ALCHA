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

#include "Subtract.h"
#include "Literal.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Subtract::Subtract(int line, const string& filename): Subtract(line, filename.c_str()){}
//------------------------------------------------------------------------------

Subtract::Subtract(int line, const char* filename): Expression(line, filename, Type::Subtract){}
//------------------------------------------------------------------------------

Subtract::~Subtract(){}
//------------------------------------------------------------------------------

Base* Subtract::copy()
{
    Subtract* copy = new Subtract(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool Subtract::getVerilog(string& body)
{
    assert(left , return false);
    assert(right, return false);

    if(!left->getSigned() && right->getSigned()){
        body += "$signed({1'b0, (";
        left->getVerilog(body);
        body += ")})";
    }else{
        body += "(";
        left->getVerilog(body);
        body += ")";
    }
    body += " - ";
    if(left->getSigned() && !right->getSigned()){
        body += "$signed({1'b0, (";
        right->getVerilog(body);
        body += ")})";
    }else{
        body += "(";
        right->getVerilog(body);
        body += ")";
    }

    return true;
}
//------------------------------------------------------------------------------

Expression* Subtract::subtractLiteral(Expression* object, Expression* literal)
{
    resultWidth     = object->getWidth();
    resultFullScale = object->getFullScale();

    Number scale = 1;
    scale.binScale(resultWidth);
    scale.div(resultFullScale);
    ((Literal*)literal)->value.mul(scale);

    int literalWidth = literal->getWidth();

    if(literalWidth > resultWidth){
        resultFullScale.binScale(literalWidth - resultWidth);
        resultWidth = literalWidth;
    }
    // Make space for the overflow
    resultFullScale.binScale(1);
    resultWidth++;

    return makeObject();
}
//------------------------------------------------------------------------------

Expression* Subtract::evaluate()
{
    assert(left , return this);
    assert(right, return this);

    left  = left ->evaluate();
    right = right->evaluate();

    assert(left , return this);
    assert(right, return this);

    resultSigned = (left->getSigned() || right->getSigned());

    if(left->type == Type::Literal && right->type == Type::Literal){
        auto result = new Literal(source.line, source.filename);
        result->value =   ((Literal*)left )->value;
        result->value.sub(((Literal*)right)->value);
        delete this;
        return result;
    }

    if(left->type == Type::Literal && right->type == Type::Object){
        return subtractLiteral(right, left);

    }else if(left->type == Type::Object && right->type == Type::Literal){
        return subtractLiteral(left, right);

    }else if(left->type == Type::Object && right->type == Type::Object){
        int leftWidth  = left ->getWidth();
        int rightWidth = right->getWidth();

        Number leftFullScale  = left ->getFullScale();
        Number rightFullScale = right->getFullScale();

        // The numerator if choosing to scale the left
        Number scale1 = leftFullScale;
        scale1.binScale(rightWidth);

        // The numerator if choosing to scale the right
        Number scale2 = rightFullScale;
        scale2.binScale(leftWidth);

        if(scale1 > scale2){ // scale the left
            scale1.div(scale2);
            left = left->scaleWith(scale1, rightWidth, rightFullScale);
            leftWidth = left->getWidth();

            resultWidth     = rightWidth;
            resultFullScale = rightFullScale;

            if(leftWidth > resultWidth){
                resultFullScale.binScale(leftWidth - resultWidth);
                resultWidth = leftWidth;
            }

        }else{ // scale the right
            scale2.div(scale1);
            right = right->scaleWith(scale2, leftWidth, leftFullScale);
            rightWidth = right->getWidth();

            resultWidth     = leftWidth;
            resultFullScale = leftFullScale;

            if(rightWidth > resultWidth){
                resultFullScale.binScale(rightWidth - resultWidth);
                resultWidth = rightWidth;
            }
        }

        // Make space for the overflow
        resultFullScale.binScale(1);
        resultWidth++;

        return makeObject();
    }

    return this;
}
//------------------------------------------------------------------------------

int Subtract::getWidth()
{
    return resultWidth;
}
//------------------------------------------------------------------------------

Number& Subtract::getFullScale()
{
    return resultFullScale;
}
//------------------------------------------------------------------------------

bool Subtract::getSigned()
{
    return resultSigned;
}
//------------------------------------------------------------------------------

bool Subtract::hasCircularReference(Netlist::Base* object)
{
    assert(left , return false);
    assert(right, return false);

    if(left ->hasCircularReference(object)) return true;
    if(right->hasCircularReference(object)) return true;

    return false;
}
//------------------------------------------------------------------------------

void Subtract::populateUsed()
{
    assert(left , return);
    assert(right, return);

    left ->populateUsed();
    right->populateUsed();
}
//------------------------------------------------------------------------------

Expression* Subtract::removeTempNet(int width, bool isSigned)
{
    if(left ) left  = left ->removeTempNet(0, false);
    if(right) right = right->removeTempNet(0, false);
    return this;
}
//------------------------------------------------------------------------------

void Subtract::display()
{
    displayStart();

    debug.print(" - " );

    displayEnd();
}
//------------------------------------------------------------------------------

void Subtract::validateMembers()
{
    assert(type == Type::Subtract);

    assert(!next);
    assert(!prev);

    assert(left , return); left ->validate();
    assert(right, return); right->validate();
}
//------------------------------------------------------------------------------

