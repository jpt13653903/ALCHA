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

#include "Definition.h"

#include "Netlist/Namespace/Module.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

DEFINITION::ARRAY::ARRAY()
{
    Size = 0;
    Next = 0;
}
//------------------------------------------------------------------------------

DEFINITION::ARRAY::ARRAY(const ARRAY& Array)
{
    Size = 0;
    Next = 0;

    if(Array.Size) Size = (decltype(Array.Size))Array.Size->Copy();
    if(Array.Next) Next = new ARRAY(*Array.Next);
}
//------------------------------------------------------------------------------

DEFINITION::ARRAY::~ARRAY()
{
    if(Size) delete Size;
    if(Next) delete Next;
}
//------------------------------------------------------------------------------

DEFINITION::IDENTIFIER::IDENTIFIER()
{
    Next        = 0;
    Array       = 0;
    Initialiser = 0;

    Function     = false;
    Parameters   = 0;
    FunctionBody = 0;
}
//------------------------------------------------------------------------------

DEFINITION::IDENTIFIER::IDENTIFIER(const IDENTIFIER& Identifier)
{
    Next        = 0;
    Array       = 0;
    Initialiser = 0;

    Function     = Identifier.Function;
    Parameters   = 0;
    FunctionBody = 0;

    if(Identifier.Next       ) Next        = new IDENTIFIER(*Identifier.Next );
    if(Identifier.Array      ) Array       = new ARRAY     (*Identifier.Array);
    if(Identifier.Initialiser) Initialiser = (decltype(Identifier.Initialiser))Identifier.Initialiser->Copy();

    FunctionBody =              CopyList(Identifier.FunctionBody);
    Parameters   = (DEFINITION*)CopyList(Identifier.Parameters  );
}
//------------------------------------------------------------------------------

DEFINITION::IDENTIFIER::~IDENTIFIER()
{
    if(Next        ) delete Next;
    if(Array       ) delete Array;
    if(Parameters  ) delete Parameters;
    if(Initialiser ) delete Initialiser;
    if(FunctionBody) delete FunctionBody;
}
//------------------------------------------------------------------------------

DEFINITION::DEFINITION(
    int         Line,
    const char* Filename,
    TYPE        DefinitionType
): BASE(Line, Filename, DefinitionType)
{
    Direction = DIRECTION::Inferred;

    Attributes  = 0;
    Identifiers = 0;
}
//------------------------------------------------------------------------------

DEFINITION::~DEFINITION()
{
    if(Attributes ) delete Attributes;
    if(Identifiers) delete Identifiers;

    foreach(Parameter, Parameters){
        if(*Parameter) delete *Parameter;
    }
}
//------------------------------------------------------------------------------

bool DEFINITION::IsDefinition()
{
    return true;
}
//------------------------------------------------------------------------------

void DEFINITION::CopyMembers(DEFINITION* Copy)
{
    Copy->Direction = Direction;

    if(Attributes ) Copy->Attributes  = (decltype(Attributes))Attributes->Copy();
    if(Identifiers) Copy->Identifiers = new IDENTIFIER(*Identifiers);

    foreach(Parameter, Parameters){
        if(*Parameter) Copy->Parameters.push_back((*Parameter)->Copy());
    }
}
//------------------------------------------------------------------------------

bool DEFINITION::VerifyNotDefined(IDENTIFIER* Identifier)
{
    auto Symbol = NETLIST::NamespaceStack.front()->Symbols.find(Identifier->Identifier);
    if(Symbol != NETLIST::NamespaceStack.front()->Symbols.end()){
        Error();
        printf("Symbol \"%s\" already defined in the current namespace\n",
                      Identifier->Identifier.c_str());
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------

void DEFINITION::DisplayParameters()
{
    Debug.Print(" Direction = ");
    switch(Direction){
        case DIRECTION::Input : Debug.Print("Input\n"   ); break;
        case DIRECTION::Output: Debug.Print("Output\n"  ); break;
        default               : Debug.Print("Inferred\n"); break;
    }

    Debug.Print(" Parameters: ");
    if(Parameters.empty()){
        Debug.Print("none / default\n");
    }else{
        foreach(Parameter, Parameters){
            if(*Parameter) (*Parameter)->Display();
            Debug.Print("\n");
        }
    }
}
//------------------------------------------------------------------------------

void DEFINITION::DisplayAttributes()
{
    Debug.Print(" Attributes: ");
    if(Attributes){
        Attributes->Display();
        Debug.Print("\n");
    }
}
//------------------------------------------------------------------------------

void DEFINITION::DisplayIdentifiers()
{
    Debug.Print(" Identifiers:\n");
    IDENTIFIER* Identifier = Identifiers;
    ARRAY     * Array;
    while(Identifier){
        Debug.Print(" - %s", Identifier->Identifier.c_str());
        Array = Identifier->Array;
        while(Array){
            Debug.Print("[");
            if(Array->Size) Array->Size->Display();
            Debug.Print("]");
            Array = Array->Next;
        }

        if(Identifier->Function){
            Debug.Print(" -- function:\n  Parameters: (\n");
            if(Identifier->Parameters) Identifier->Parameters->Display();
            Debug.Print(" )\n  Body:{\n");
            if(Identifier->FunctionBody) Identifier->FunctionBody->Display();
            Debug.Print("  }\n");
        }
        if(Identifier->Initialiser){
            Debug.Print(" -- initialiser:");
            Identifier->Initialiser->Display();
        }

        Debug.Print("\n");
        Identifier = Identifier->Next;
    }
}
//------------------------------------------------------------------------------

void DEFINITION::DisplayDefinition(const char* Type)
{
    DisplayInfo();
    Debug.Print("Definition (%s):\n", Type);

    DisplayParameters ();
    DisplayAttributes ();
    DisplayIdentifiers();

    if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void DEFINITION::ValidateMembers()
{
    if(!Parameters.empty()){
        foreach(Parameter, Parameters){
            if(*Parameter) (*Parameter)->Validate();
        }
    }

    if(Attributes) Attributes ->Validate();

    IDENTIFIER* Identifier = Identifiers;
    ARRAY     * Array;

    while(Identifier){
        Array = Identifier->Array;
        while(Array){
            if(Array->Size) Array->Size->Validate();
            Array = Array->Next;
        }

        if(Identifier->Function){
            assert(Identifier              == Identifiers);
            assert(Identifier->Next        == 0);
            assert(Identifier->Initialiser == 0);

            if(Identifier->Parameters  ) Identifier->Parameters  ->Validate();
            if(Identifier->FunctionBody) Identifier->FunctionBody->Validate();

        }else{
            assert(Identifier->Parameters   == 0);
            assert(Identifier->FunctionBody == 0);

            if(Identifier->Initialiser) Identifier->Initialiser ->Validate();
        }

        Identifier = Identifier->Next;
    }
}
//------------------------------------------------------------------------------

