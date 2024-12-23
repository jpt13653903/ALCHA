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

#include "Multiply.h"
#include "Literal.h"
#include "Negate.h"
#include "Object.h"

#include "Netlist/NameSpace/Module.h"
#include "Netlist/Synthesisable/Net.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Multiply::Multiply(int line, const string& filename): Multiply(line, filename.c_str()){}
//------------------------------------------------------------------------------

Multiply::Multiply(int line, const char* filename): Expression(line, filename, Type::Multiply){}
//------------------------------------------------------------------------------

Multiply::~Multiply(){}
//------------------------------------------------------------------------------

Base* Multiply::copy()
{
    Multiply* copy = new Multiply(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool Multiply::getVerilog(string& body)
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
    body += " * ";
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

Expression* Multiply::evaluate()
{
    assert(left , return this);
    assert(right, return this);

    left  = left ->evaluate();
    right = right->evaluate();

    assert(left , return this);
    assert(right, return this);

    if(left->type == Type::Literal && right->type == Type::Literal){
        auto result = new Literal(source.line, source.filename);
        result->value =   ((Literal*)left )->value;
        result->value.mul(((Literal*)right)->value);
        delete this;
        return result;
    }

    // Replace a object * object with a wire
    if(left->type == Type::Object && right->type == Type::Object){
        return makeObject();
    }

    // Put the literal on the right (if there is one)
    if(left->type == Type::Literal){
        auto temp = left;
        left  = right;
        right = temp;
    }

    if(left->type == Type::Object && right->type == Type::Literal){
        auto _left  = ((Object *)left )->objectRef;
        auto _right =  (Literal*)right;

        assert(_left);

        auto object = new Object      (source.line, source.filename);
        auto net    = new Netlist::Net(source.line, source.filename, 0);
        object->objectRef = net;

        Number fullScale = _left->fullScale();
        fullScale.mul(_right->value);

        bool isSigned = _left->isSigned();

        if(_right->getSigned()){
            isSigned = true;
            fullScale.mul(-1);

            auto negate = new Negate(source.line, source.filename);
            negate->right = left;
            net   ->value = negate;
        }else{ // Positive literal
            net->value = left;
        }
        net->setFixedPoint(_left->width(), fullScale, isSigned);
        Netlist::nameSpaceStack.front()->symbols[net->name] = net;
        left = 0;
        delete this;
        return object;
    }

    return this;
}
//------------------------------------------------------------------------------

int Multiply::getWidth()
{
    assert(left , return 0);
    assert(right, return 0);

    if(left->getSigned() && right->getSigned()){
        return left->getWidth() + right->getWidth() + 1;
    }
    return left->getWidth() + right->getWidth();
}
//------------------------------------------------------------------------------

Number& Multiply::getFullScale()
{
    static Number result;

    assert(left , result = 0; return result);
    assert(right, result = 0; return result);

    result = left->getFullScale();
    result.mul(right->getFullScale());

    // Make space for the signed bit
    if(left->getSigned() && right->getSigned()) result.binScale(1);

    return result;
}
//------------------------------------------------------------------------------

bool Multiply::getSigned()
{
    return (left->getSigned() || right->getSigned());
}
//------------------------------------------------------------------------------

bool Multiply::hasCircularReference(Netlist::Base* object)
{
    assert(left , return false);
    assert(right, return false);

    if(left ->hasCircularReference(object)) return true;
    if(right->hasCircularReference(object)) return true;

    return false;
}
//------------------------------------------------------------------------------

void Multiply::populateUsed()
{
    assert(left , return);
    assert(right, return);

    left ->populateUsed();
    right->populateUsed();
}
//------------------------------------------------------------------------------

Expression* Multiply::removeTempNet(int width, bool isSigned)
{
    if(left ) left  = left ->removeTempNet(0, false);
    if(right) right = right->removeTempNet(0, false);
    return this;
}
//------------------------------------------------------------------------------

void Multiply::display()
{
    displayStart();

    debug.print(" * " );

    displayEnd();
}
//------------------------------------------------------------------------------

void Multiply::validateMembers()
{
    assert(type == Type::Multiply);

    assert(!next);
    assert(!prev);

    assert(left , return); left ->validate();
    assert(right, return); right->validate();
}
//------------------------------------------------------------------------------

