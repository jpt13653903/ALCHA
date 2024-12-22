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

#include "Attribute.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

ATTRIBUTE::ATTRIBUTE(int Line, const string& Filename, const char* Name):
BASE(Line, Filename, Name, TYPE::Attribute)
{
    Value = 0;
}
//------------------------------------------------------------------------------

ATTRIBUTE::~ATTRIBUTE()
{
    if(Value) delete Value;
}
//------------------------------------------------------------------------------

AST::EXPRESSION* ATTRIBUTE::GetExpression(int Line, const string& Filename)
{
    if(Value) return (AST::EXPRESSION*)Value->Copy();
    ::Error(Line, Filename, "Operate-assign on empty object");
    return 0;
}
//------------------------------------------------------------------------------

bool ATTRIBUTE::Assign(AST::EXPRESSION* Expression)
{
    return RawAssign(Expression);
}
//------------------------------------------------------------------------------

bool ATTRIBUTE::RawAssign(AST::EXPRESSION* Expression)
{
    if(Value){
        Expression->Warning();
        printf("Overwriting attribute %s\n", Name.c_str());
        delete Value;
    }
    Value = Expression;
    return true;
}
//------------------------------------------------------------------------------

bool ATTRIBUTE::HasCircularReference(BASE* Object)
{
    if(this == Object) return true;
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void ATTRIBUTE::Display(int Indent)
{
    Debug.Indent(Indent);
    Debug.Print(Name);
    Debug.Print(" = ");
    if(Value) Value->Display();
    else      Debug.Print("{null}");
    Debug.Print("\n");
}
//------------------------------------------------------------------------------

void ATTRIBUTE::Validate()
{
    assert(Type == TYPE::Attribute);

    assert(Attributes.size() == 0);

    BASE::Validate();
}
//------------------------------------------------------------------------------

