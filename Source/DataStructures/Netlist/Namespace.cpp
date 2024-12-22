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

#include "Namespace.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

NAMESPACE::NAMESPACE(int Line, const string& Filename,
                                          const char* Name, TYPE NamespaceType):
BASE(Line, Filename, Name, NamespaceType)
{
    Ast = 0;
}
//------------------------------------------------------------------------------

NAMESPACE::~NAMESPACE()
{
    if(Ast) delete Ast;
    foreach(s, Symbols) delete s->second;
}
//------------------------------------------------------------------------------

AST::EXPRESSION* NAMESPACE::GetExpression(int Line, const string& Filename)
{
    ::Error(Line, Filename, "Cannot obtain an expression from a namespace");
    return 0;
}
//------------------------------------------------------------------------------

bool NAMESPACE::Assign(AST::EXPRESSION* Expression)
{
    return RawAssign(Expression);
}
//------------------------------------------------------------------------------

bool NAMESPACE::RawAssign(AST::EXPRESSION* Expression)
{
    Expression->Error("Cannot assign to a namespace");
    return false;
}
//------------------------------------------------------------------------------

bool NAMESPACE::HasCircularReference(BASE* Object)
{
    if(this == Object) return true;
    return false;
}
//------------------------------------------------------------------------------

bool NAMESPACE::IsNamespace()
{
    return true;
}
//------------------------------------------------------------------------------

BASE* NAMESPACE::GetMember(const std::string& Name)
{
    auto Member = Symbols.find(Name);
    if(Member != Symbols.end()) return Member->second;
    return 0;
}
//------------------------------------------------------------------------------

void NAMESPACE::Validate()
{
    BASE::Validate();

    if(Ast) Ast->Validate();
    foreach(Symbol, Symbols){
        assert(Symbol->second, return);
        Symbol->second->Validate();
    }
}
//------------------------------------------------------------------------------

