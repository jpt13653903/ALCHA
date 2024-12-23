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

#include "Identifier.h"
#include "Object.h"
#include "Literal.h"

#include "Netlist/Alias.h"
#include "Netlist/NameSpace/Module.h"
#include "Netlist/Synthesisable.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Identifier::Identifier(int line, const string& filename): Identifier(line, filename.c_str()){}
//------------------------------------------------------------------------------

Identifier::Identifier(int line, const char* filename): Expression(line, filename, Type::Identifier){}
//------------------------------------------------------------------------------

Identifier::~Identifier(){}
//------------------------------------------------------------------------------

Base* Identifier::copy()
{
    Identifier* copy = new Identifier(source.line, source.filename.c_str());

    copy->name = name;

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool Identifier::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

Expression* Identifier::evaluate()
{
    for(auto nameSpace: Netlist::nameSpaceStack){
        while(nameSpace){
            auto object = nameSpace->getMember(name);
            if(object){
                auto objResult = new Object(source.line, source.filename);
                objResult->objectRef = object;
                // It might be an alias, or evaluate further to a literal
                auto result = objResult->evaluate();
                if(result){ // The source reference gets messed up during alias expansion
                    result->source.line     = source.line;
                    result->source.filename = source.filename;
                }
                delete this;
                return result;
            }
            nameSpace = nameSpace->nameSpace;
        }
    }

    // Identifier not found, so try the constants table
    Number constant;
    if(constants.getConstant(name, &constant)){
        auto result = new Literal(source.line, source.filename);
        result->value = constant;
        delete this;
        return result;
    }

    printError();
    printf("Undefined identifier: \"%s\"\n", name.c_str());

    delete this;
    return 0;
}
//------------------------------------------------------------------------------

int Identifier::getWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

Number& Identifier::getFullScale()
{
    error("Not yet implemented");
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool Identifier::getSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool Identifier::hasCircularReference(Netlist::Base* object)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Identifier::populateUsed()
{
    error("Not yet implemented");
}
//------------------------------------------------------------------------------

Expression* Identifier::removeTempNet(int width, bool isSigned)
{
    error("Not yet implemented");
    return this;
}
//------------------------------------------------------------------------------

void Identifier::display()
{
    displayStart();

    if(name.empty()) error ("(Identifier node has no name)");
    else             debug.print("%s", name.c_str());

    displayEnd();
}
//------------------------------------------------------------------------------

void Identifier::validateMembers()
{
    assert(type == Type::Identifier);

    assert(!next);
    assert(!prev);

    assert(!left );
    assert(!right);
}
//------------------------------------------------------------------------------

