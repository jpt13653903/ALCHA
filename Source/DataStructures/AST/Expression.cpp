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

#include "Expression.h"
#include "Assignment.h"
#include "Expression/literal.h"
#include "Expression/Multiply.h"
#include "Expression/object.h"
#include "Expression/ShiftLeft.h"
#include "Expression/ShiftRight.h"

#include "Netlist/Namespace/Module.h"
#include "Netlist/Synthesisable/net.h"
//------------------------------------------------------------------------------

using namespace AST;
//------------------------------------------------------------------------------

Expression::Expression(int line, const char* filename, Type expressionType):
    Base(line, filename, expressionType)
{
    left  = 0;
    right = 0;
}
//------------------------------------------------------------------------------

Expression::~Expression()
{
    if(left ) delete left;
    if(right) delete right;
    assert(!next);
}
//------------------------------------------------------------------------------

bool Expression::isExpression()
{
    return true;
}
//------------------------------------------------------------------------------

Expression* Expression::scaleWith(Number scale, int width, Number fullScale)
{
    if(scale == 1) return this;

    bool isSigned  = getSigned();
    int  thisWidth = getWidth ();

    int scaleWidth = 0;
    Number num = scale;
    if(num < 0) num.mul(-1);
    while(num < 1){
        num.binScale(1);
        scaleWidth--;
    }
    while(num > 1){
        num.binScale(-1);
        scaleWidth++;
    }

    if(thisWidth + scaleWidth > width){
        fullScale.binScale(thisWidth + scaleWidth - width);
        width = thisWidth + scaleWidth;
    }

    // Calculate the limit of the inferred multiplier size.  Most FPGAs have
    // 18-bit multipliers, so make that the minimum limit, otherwise use the
    // target width as the limit so that no to little resolution is lost.
    Number limit(1);
    if(isSigned) limit.binScale(18);
    else         limit.binScale(17);

    // Convert the multiplication to a shift, as far as possible
    int shift = 0;
    while(scale.isInt()){
        scale.binScale(-1);
        shift--;
    }
    while(!scale.isInt() && (scale < limit)){
        scale.binScale(1);
        shift++;
    }
    while(scale >= limit){
        scale.binScale(-1);
        shift--;
    }
    Number fullFactor(scale);
    scale.round();
    if(scale != fullFactor){
        printWarning("Rounding the scaling factor - this can be fixed "
                     "with an explicit scaling multiplication.");
        while(scale.isInt()){ // Make sure it's still minimised after rounding
            scale.binScale(-1);
            shift--;
        }
        while(!scale.isInt()){
            scale.binScale(1);
            shift++;
        }
    }

    auto net = new Netlist::Net(source.line, source.filename, 0);
    net->setFixedPoint(width, fullScale, isSigned);
    Netlist::nameSpaceStack.front()->symbols[net->name] = net;

    if(scale == 1){ // shift only
        auto literal = new Literal(source.line, source.filename);

        if(shift > 0){
            net->value = new ShiftRight(source.line, source.filename);
            literal->value = shift;

        }else{
            net->value = new ShiftLeft(source.line, source.filename);
            literal->value = -shift;
        }

        net->value->left  = this;
        net->value->right = literal;

    }else if(shift == 0){ // Multiply only
        auto literal = new Literal(source.line, source.filename);
        literal->value = scale;

        auto mul = new Multiply(source.line, source.filename);
        mul->left  = this;
        mul->right = literal;

        net->value = mul;

    }else{ // Multiply and shift
        auto mulLiteral = new Literal(source.line, source.filename);
        mulLiteral->value = scale;

        auto mul = new Multiply(source.line, source.filename);
        mul->left  = this;
        mul->right = mulLiteral;

        auto mulNet = new Netlist::Net(source.line, source.filename, 0);
        Number mulFullScale = fullScale;
        mulFullScale.binScale(shift);
        mulNet->setFixedPoint(mul->getWidth(), mulFullScale, getSigned());
        mulNet->value = mul;
        Netlist::nameSpaceStack.front()->symbols[mulNet->name] = mulNet;

        auto mulObject = new Object(source.line, source.filename);
        mulObject->objectRef = mulNet;

        auto shiftLiteral = new Literal(source.line, source.filename);

        if(shift > 0){
            net->value = new ShiftRight(source.line, source.filename);
            shiftLiteral->value = shift;

        }else{
            net->value = new ShiftLeft(source.line, source.filename);
            shiftLiteral->value = -shift;
        }

        net->value->left  = mulObject;
        net->value->right = shiftLiteral;
    }

    auto object = new Object(source.line, source.filename);
    object->objectRef = net;
    return object;
}
//------------------------------------------------------------------------------

Expression* Expression::makeObject()
{
    if(type == Type::Object) return this;

    auto object = new Object      (source.line, source.filename);
    auto net    = new Netlist::Net(source.line, source.filename, 0);
    object->objectRef = net;

    net->setFixedPoint(getWidth(), getFullScale(), getSigned());
    net->value = this;

    Netlist::nameSpaceStack.front()->symbols[net->name] = net;

    return object;
}
//------------------------------------------------------------------------------

void Expression::displayStart()
{
    if(left){
        if(left->left || left->right) debug.print("(");
        left->display();
        if(left->left || left->right) debug.print(")");
    }
}
//------------------------------------------------------------------------------

void Expression::displayEnd()
{
    if(right){
        if(right->left || right->right || right->next) debug.print("(");
        right->display();
        if(right->left || right->right || right->next) debug.print(")");
    }
}
//------------------------------------------------------------------------------

bool Expression::runAST()
{
    // For most expression types, this function is never called
    error("Unexpected runAST call");
    return false;
}
//------------------------------------------------------------------------------

