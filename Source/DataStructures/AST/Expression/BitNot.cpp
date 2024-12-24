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

#include "BitNot.h"
#include "Literal.h"
#include "Object.h"

#include "Netlist/NameSpace/Module.h"
#include "Netlist/Synthesisable/Net.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

BitNot::BitNot(int line, const string& filename): BitNot(line, filename.c_str()){}
//------------------------------------------------------------------------------

BitNot::BitNot(int line, const char* filename): Expression(line, filename, Type::BitNot){}
//------------------------------------------------------------------------------

BitNot::~BitNot(){}
//------------------------------------------------------------------------------

Base* BitNot::copy()
{
    BitNot* copy = new BitNot(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool BitNot::getVerilog(string& body)
{
    body += "~(";
    right->getVerilog(body);
    body += ")";

    return true;
}
//------------------------------------------------------------------------------

Expression* BitNot::evaluate()
{
    assert(right, delete this; return 0);
    right = right->evaluate();
    assert(right, delete this; return 0);

    switch(right->type){
        case Type::Literal:{
            auto result = (Literal*)right;
            right = 0;
            int resultWidth = result->getWidth();
            if(result->getWidth()){
                Number n(1);
                n.binScale(resultWidth);
                n.sub(result->value);
                n.sub(1);
                result->value = n;
            }else{
                printError("Taking bitwise NOT of literal with unknown width");
                delete result;
                result = 0;
            }
            delete this;
            return result;
        }
        case Type::Object:{
            auto net = new Netlist::Net(source.line, source.filename, 0);
            net->value = this;

            auto objectRef = ((Object*)right)->objectRef;
            if(objectRef && objectRef->isSynthesisable()){
                auto synthesisable = (Netlist::Synthesisable*)objectRef;
                net->setFixedPoint(synthesisable->width(), synthesisable->fullScale());
            }
            Netlist::nameSpaceStack.front()->symbols[net->name] = net;

            auto object = new Object(source.line, source.filename);
            object->objectRef = net;
            return object;
        }
        default:
            error("Unexpected default");
            break;
    }
    return this;
}
//------------------------------------------------------------------------------

int BitNot::getWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

Number& BitNot::getFullScale()
{
    error("Not yet implemented");
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool BitNot::getSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool BitNot::hasCircularReference(Netlist::Base* object)
{
    assert(right, return false);
    return right->hasCircularReference(object);
}
//------------------------------------------------------------------------------

void BitNot::populateUsed()
{
    assert(right, return);
    right->populateUsed();
}
//------------------------------------------------------------------------------

Expression* BitNot::removeTempNet(int width, bool isSigned)
{
    if(right) right = right->removeTempNet(0, false);
    return this;
}
//------------------------------------------------------------------------------

void BitNot::display()
{
    displayStart();

    logger.print(" ~");

    displayEnd();
}
//------------------------------------------------------------------------------

void BitNot::validateMembers()
{
    assert(type == Type::BitNot);

    assert(!next);
    assert(!prev);

    assert(!left);
    assert(right, return); right->validate();
}
//------------------------------------------------------------------------------

