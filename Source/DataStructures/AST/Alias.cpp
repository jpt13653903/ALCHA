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

#include "Alias.h"
#include "Netlist/Alias.h"
#include "Netlist/Namespace/Module.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

ALIAS::ALIAS(int Line, std::string& Filename): ALIAS(Line, Filename.c_str())
{}
//------------------------------------------------------------------------------

ALIAS::ALIAS(int Line, const char* Filename): BASE(Line, Filename, TYPE::Alias)
{
    Expression = 0;
}
//------------------------------------------------------------------------------

ALIAS::~ALIAS()
{
    if(Expression) delete Expression;
}
//------------------------------------------------------------------------------

BASE* ALIAS::Copy()
{
    ALIAS* Copy = new ALIAS(Source.Line, Source.Filename.c_str());

    Copy->Identifier = Identifier;

    if(Expression) Copy->Expression = (decltype(Expression))Expression->Copy();

    return Copy;
}
//------------------------------------------------------------------------------

bool ALIAS::RunAST()
{
    auto Symbol = NETLIST::NamespaceStack.front()->Symbols.find(Identifier);
    if(Symbol != NETLIST::NamespaceStack.front()->Symbols.end()){
        Error();
        printf("Symbol \"%s\" already defined in the current namespace\n",
                      Identifier.c_str());
        return false;
    }

    auto Object = new NETLIST::ALIAS(Source.Line, Source.Filename, Identifier.c_str(), Expression);
    Expression = 0; // It's been moved to Object, so remove it from this node.

    NETLIST::NamespaceStack.front()->Symbols[Object->Name] = Object;

    return true;
}
//------------------------------------------------------------------------------

bool ALIAS::GetVerilog(string& Body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void ALIAS::Display()
{
    DisplayInfo();
    Debug.Print("Alias (%s):\n", Identifier.c_str());

    if(Expression) Expression->Display();
    else           Debug.Print("{Moved Expression}\n");

    if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void ALIAS::ValidateMembers()
{
    assert(Type == TYPE::Alias);
    if(Expression) Expression->Validate();
}
//------------------------------------------------------------------------------

