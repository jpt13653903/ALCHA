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

#include "object.h"
#include "Literal.h"

#include "Netlist/alias.h"
#include "Netlist/byte.h"
#include "Netlist/Character.h"
#include "Netlist/num.h"
#include "Netlist/Namespace/Module.h"
#include "Netlist/Synthesisable/net.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Object::Object(int line, const string& filename): Object(line, filename.c_str()){}
//------------------------------------------------------------------------------

Object::Object(int line, const char* filename): Expression(line, filename, Type::Object)
{
    objectRef = 0;
}
//------------------------------------------------------------------------------

Object::~Object()
{
    // Don't delete the object reference -- it's part of the namespace tree
}
//------------------------------------------------------------------------------

Base* Object::copy()
{
    Object* copy = new Object(source.line, source.filename.c_str());

    copy->objectRef = objectRef;

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool Object::getVerilog(string& body)
{
    assert(objectRef, return false);
    body += objectRef->escapedName();

    return true;
}
//------------------------------------------------------------------------------

Expression* Object::evaluate()
{
    assert(objectRef, delete this; return 0);

    switch(objectRef->type){
        case Netlist::Base::Type::Byte:{
            auto byte = (Netlist::Byte*)objectRef;
            auto result = new Literal(source.line, source.filename);
            result->value = byte->value;
            result->setWidth(8);
            delete this;
            return result;
        }
        case Netlist::Base::Type::Character:{
            auto character = (Netlist::Character*)objectRef;
            auto result = new Literal(source.line, source.filename);
            result->value = character->value;
            result->setWidth(32);
            delete this;
            return result;
        }
        case Netlist::Base::Type::Number:{
            auto num = (Netlist::Num*)objectRef;
            auto result = new Literal(source.line, source.filename);
            result->value = num->value;
            delete this;
            return result;
        }
        case Netlist::Base::Type::Alias:{
            auto alias  = (Netlist::Alias*)objectRef;
            auto result = alias->expression;
            if(!result){
                printError("alias has no expression");
                delete this;
                return 0;
            }
            result = (Expression*)result->copy();
            Netlist::nameSpaceStack.push_front(alias->nameSpace);
                result = result->evaluate();
            Netlist::nameSpaceStack.pop_front();
            if(!result){
                printError("Error evaluating alias");
                delete this;
                return 0;
            }
            delete this;
            return result;
        }
        default:
            break;
    }
    return this;
}
//------------------------------------------------------------------------------

int Object::getWidth()
{
    assert(objectRef, return 0);
    return objectRef->width();
}
//------------------------------------------------------------------------------

Number& Object::getFullScale()
{
    assert(objectRef);
    return objectRef->fullScale();
}
//------------------------------------------------------------------------------

bool Object::getSigned()
{
    assert(objectRef, return false);
    return objectRef->isSigned();
}
//------------------------------------------------------------------------------

bool Object::hasCircularReference(Netlist::Base* object)
{
    if(!objectRef) return false;
    return objectRef->hasCircularReference(object);
}
//------------------------------------------------------------------------------

void Object::populateUsed()
{
    if(objectRef) objectRef->populateUsed(true);
}
//------------------------------------------------------------------------------

Expression* Object::removeTempNet(int width, bool isSigned)
{
    // At this point, everything has been broken down to raw bits, so the
    // full-scale is not important.

    if(objectRef && objectRef->type == Netlist::Base::Type::Net){
        auto net = (Netlist::Net*)objectRef;
        if(net->value && net->isTemporary()){
            if(width  == net->width () &&
                  isSigned == net->isSigned() ){
                delete this;
                return ((Expression*)net->value->copy())->removeTempNet(width, isSigned);
            }
            if(net->value->type == Type::Object){
                auto object = (Object*)net->value;
                if(getWidth () == object->getWidth () &&
                      getSigned() == object->getSigned() ){
                    delete this;
                    return ((Expression*)object->copy())->removeTempNet(width, isSigned);
                }
            }
        }
    }
    return this;
}
//------------------------------------------------------------------------------

void Object::display()
{
    displayStart();

    if(objectRef){
        // TODO: display the width here, because it should be a property
        //       of the target object, not the expression itself
        objectRef->displayLongName();
    }else{
        error("Null object reference");
    }

    displayEnd();
}
//------------------------------------------------------------------------------

void Object::validateMembers()
{
    assert(type == Type::Object);

    assert(!next);
    assert(!prev);

    assert(!left );
    assert(!right);

    assert(objectRef);
}
//------------------------------------------------------------------------------

