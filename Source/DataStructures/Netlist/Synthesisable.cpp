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

#include "Synthesisable.h"

#include "AST/Expression/Literal.h"
//------------------------------------------------------------------------------

using std::list;
using namespace Netlist;
//------------------------------------------------------------------------------

Synthesisable::Synthesisable(int line, const std::string& filename, const char* name, Type type) : Base(line, filename, name, type)
{
    used = false;

    format.width     = new Num(line, filename, "width"    );
    format.fullScale = new Num(line, filename, "fullScale");
    format.isSigned  = new Num(line, filename, "Signed"   );

    format.width    ->value = 1;
    format.fullScale->value = 2;
    format.isSigned ->value = 0;
}
//------------------------------------------------------------------------------

Synthesisable::~Synthesisable()
{
    delete format.width;
    delete format.fullScale;
    delete format.isSigned;
}
//------------------------------------------------------------------------------

bool Synthesisable::isSynthesisable()
{
    return true;
}
//------------------------------------------------------------------------------

int Synthesisable::width()
{
    return format.width->value.getReal();
}
//------------------------------------------------------------------------------

Number& Synthesisable::fullScale()
{
    return format.fullScale->value;
}
//------------------------------------------------------------------------------

bool Synthesisable::isSigned()
{
    return format.isSigned->value != 0;
}
//------------------------------------------------------------------------------

void Synthesisable::setFixedPoint(int width, const Number& fullScale, bool isSigned)
{
    assert(width     > 0);
    assert(fullScale > 0);

    format.width    ->value = width;
    format.fullScale->value = fullScale;
    format.isSigned ->value = isSigned ? 1 : 0;
}
//------------------------------------------------------------------------------

void Synthesisable::setFixedPoint(int width, const Number& fullScale)
{
    format.isSigned->value = 0;

    if(width < 0){
        format.width   ->value = -width;
        format.isSigned->value =  1;
    }else{
        format.width   ->value =  width;
    }

    format.fullScale->value = fullScale;
    if(fullScale < 0){
        format.fullScale->value.mul(-1);
        format.isSigned ->value = 1;
    }
}
//------------------------------------------------------------------------------

bool Synthesisable::applyParameters(list<AST::Base*>& parameters)
{
    int  position          = 0; // Negative => named parameters
    bool explicitFullScale = false;

    int    width = 0;
    Number fullScale;

    for(auto parameter: parameters){
        if(parameter->isExpression()){
            if(position < 0) return false; // Mixing named and positional parameters

            AST::Expression* param = ((AST::Expression*)parameter)->evaluate();
            parameter = param;
            if(!param) return false;

            switch(param->type){
                case AST::Base::Type::Literal:{
                    auto literal = (AST::Literal*)param;
                    switch(position){
                        case 0:
                            if(!literal->value.isInt()) return false;
                            width = round(literal->value.getReal());
                            break;

                        case 1:
                            explicitFullScale = true;
                            fullScale = literal->value;
                            break;

                        default: // Too many parameters
                            delete literal;
                            return false;
                    }
                    break;
                }

                default:
                    param->printError("Parameters must be pure scripting expressions");
                    return false;
            }

        }else if(parameter->isAssignment()){
            position = -1;
            error("Not yet implemented");

        }else{
            return false;
        }
        if(position >= 0) position++;
    }
    if(width == 0) width = 1;

    if(!explicitFullScale){
        fullScale = 1;
        if(width > 0) fullScale.binScale( width);
        else          fullScale.binScale(-width);
    }
    setFixedPoint(width, fullScale);

    return true;
}
//------------------------------------------------------------------------------

void Synthesisable::displayParameters(int indent)
{
    debug.print("%s\n", name.c_str());

    debug.indent(indent);
    debug.print("used       = %s\n", used ? "true" : "false");
    debug.indent(indent);
    debug.print("width      = %u\n", width());
    debug.indent(indent);
    debug.print("Full-scale = %s\n", fullScale().display());
    debug.indent(indent);
    debug.print("Signed     = %s\n", isSigned() ? "true" : "false");

    debug.indent(indent);
    debug.print("Direction  = ");
    switch(direction){
        case AST::Definition::Direction::Inferred     : debug.print("Inferred\n"     ); break;
        case AST::Definition::Direction::Input        : debug.print("Input\n"        ); break;
        case AST::Definition::Direction::Output       : debug.print("Output\n"       ); break;
        case AST::Definition::Direction::Bidirectional: debug.print("Bidirectional\n"); break;
        default                                       : debug.print("Invalid\n"      ); break;
    }
}
//------------------------------------------------------------------------------

Base* Synthesisable::getAttribute(const std::string& name)
{
    if(name == "width"    ) return format.width;
    if(name == "fullscale") return format.fullScale;
    if(name == "signed"   ) return format.isSigned;

    return Base::getAttribute(name);
}
//------------------------------------------------------------------------------

AST::Expression* Synthesisable::getBuiltInAttributeValue(const std::string& name)
{
    if(name == "width"){
        auto result = new AST::Literal(0, "");
        result->value = width();
        return result;
    }
    if(name == "fullscale"){
        auto result = new AST::Literal(0, "");
        result->value = fullScale();
        return result;
    }
    if(name == "signed"){
        auto result = new AST::Literal(0, "");
        result->value = isSigned() ? 1 : 0;
        return result;
    }
    return Base::getBuiltInAttributeValue(name);
}
//------------------------------------------------------------------------------

void Synthesisable::validate()
{
    assert(format.width    ->value  > 0);
    assert(format.fullScale->value  > 0);
    assert(format.isSigned ->value == 0 || format.isSigned->value == 1);

    Base::validate();
}
//------------------------------------------------------------------------------

