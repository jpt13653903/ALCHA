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

#include "Group.h"
#include "Netlist/Namespace/Group.h"
#include "Netlist/Namespace/Module.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

AST::GROUP::GROUP(int Line, std::string& Filename): AST::GROUP(Line, Filename.c_str())
{}
//------------------------------------------------------------------------------

AST::GROUP::GROUP(int Line, const char* Filename): BASE(Line, Filename, TYPE::Group)
{
    Attributes = 0;
    Body       = 0;
}
//------------------------------------------------------------------------------

AST::GROUP::~GROUP()
{
    if(Attributes) delete Attributes;
    if(Body      ) delete Body;
}
//------------------------------------------------------------------------------

BASE* AST::GROUP::Copy()
{
    GROUP* Copy = new AST::GROUP(Source.Line, Source.Filename.c_str());

    Copy->Identifier = Identifier;

    if(Attributes) Copy->Attributes = (decltype(Attributes))Attributes->Copy();
    if(Body      ) Copy->Body       = (decltype(Body      ))Body      ->Copy();

    return Copy;
}
//------------------------------------------------------------------------------

bool AST::GROUP::RunAST()
{
    if(Identifier.empty()){
        error("Anonymous groups not supported yet");
        return false;
    }

    auto Found = NETLIST::NamespaceStack.front()->Symbols.find(Identifier);
    if(Found != NETLIST::NamespaceStack.front()->Symbols.end()){
        Error();
        printf("Symbol \"%s\" already exists in the current namespace\n",
                      Identifier.c_str());
        return false;
    }
    auto Object = new NETLIST::GROUP(Source.Line, Source.Filename, Identifier.c_str());
    Object->ApplyAttributes(Attributes);
    NETLIST::NamespaceStack.front()->Symbols[Identifier] = Object;
    NETLIST::NamespaceStack.push_front(Object);

    bool Result  = true;
    auto Element = Body;
    while(Result && Element){
        Result  = Element->RunAST();
        Element = Element->Next;
    }

    NETLIST::NamespaceStack.pop_front();
    return Result;
}
//------------------------------------------------------------------------------

bool AST::GROUP::GetVerilog(string& Body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void AST::GROUP::Display()
{
    DisplayInfo();

    if(Identifier.empty()) Debug.Print("Group:\n");
    else                   Debug.Print("Group (%s):\n", Identifier.c_str());

    Debug.Print(" Attributes: ");
    if(Attributes){
        Attributes->Display();
        Debug.Print("\n");
    }

    Debug.Print(" {\n");
    if(Body) Body->Display();
    Debug.Print(" }\n");

    if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void AST::GROUP::ValidateMembers()
{
    assert(Type == TYPE::Group);

    if(Attributes) Attributes->Validate();
    if(Body      ) Body      ->Validate();
}
//------------------------------------------------------------------------------

