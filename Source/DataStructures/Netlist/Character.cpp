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

#include "Character.h"

#include "AST/Expression/Literal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

CHARACTER::CHARACTER(int Line, const string& Filename, const char* Name) : BASE(Line, Filename, Name, TYPE::Character)
{
    Value = 0;
}
//------------------------------------------------------------------------------

CHARACTER::~CHARACTER()
{
}
//------------------------------------------------------------------------------

AST::EXPRESSION* CHARACTER::GetExpression(int Line, const string& Filename)
{
    AST::LITERAL* Result = new AST::LITERAL(Line, Filename);
    Result->Value = Value;
    Result->SetWidth(32);
    return Result;
}
//------------------------------------------------------------------------------

bool CHARACTER::Assign(AST::EXPRESSION* Expression)
{
    error("Not yet implemented");
    return RawAssign(Expression);
}
//------------------------------------------------------------------------------

bool CHARACTER::RawAssign(AST::EXPRESSION* Expression)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool CHARACTER::HasCircularReference(BASE* Object)
{
    if(this == Object) return true;
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void CHARACTER::Display(int Indent)
{
    Debug.Indent(Indent+1);
    Debug.Print("Character: %s\n", Name.c_str());

    Debug.Indent(Indent+2);
    Debug.Print("Value = '%s' ", UTF_Converter.UTF8(Value));
    Debug.Print("(%d)\n", Value);
}
//------------------------------------------------------------------------------

void CHARACTER::Validate()
{
    assert(Type == TYPE::Character);

    BASE::Validate();

    error("Not implemented yet");
}
//------------------------------------------------------------------------------

