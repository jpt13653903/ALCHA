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

#include "Base.h"
#include "Attribute.h"
#include "Namespace/Module.h"
#include "AST/Expression/Identifier.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

BASE::BASE(int Line, const string& Filename, const char* Name, TYPE Type)
{
    Source.Line      = Line;
    Source.Filename  = Filename;
    this->Type       = Type;
    this->Namespace  = 0;

    static unsigned GenNameCount = 0;
    if(Name){
        Temporary = false;
        this->Name = Name;
    }else{
        Temporary = true;
        switch(Type){
            case TYPE::Net   : this->Name = "w.."; break;
            case TYPE::Module: this->Name = "m.."; break;
            default          : this->Name = "t.."; break;
        }
        if(GenNameCount < 1000000) this->Name += '0';
        if(GenNameCount <  100000) this->Name += '0';
        if(GenNameCount <   10000) this->Name += '0';
        if(GenNameCount <    1000) this->Name += '0';
        if(GenNameCount <     100) this->Name += '0';
        if(GenNameCount <      10) this->Name += '0';
        this->Name += to_string(GenNameCount++);
    }

    if(!NamespaceStack.empty()) Namespace = NamespaceStack.front();
}
//------------------------------------------------------------------------------

BASE::~BASE()
{
    foreach(Attribute, Attributes) delete Attribute->second;
}
//------------------------------------------------------------------------------

void BASE::Error(const char* Message)
{
    ::Error(Source.Line, Source.Filename, Message);
}
//------------------------------------------------------------------------------

void BASE::Warning(const char* Message)
{
    ::Warning(Source.Line, Source.Filename.c_str(), Message);
}
//------------------------------------------------------------------------------

bool BASE::IsSynthesisable()
{
    return false;
}
//------------------------------------------------------------------------------

bool BASE::IsNamespace()
{
    return false;
}
//------------------------------------------------------------------------------

bool BASE::IsTemporary()
{
    return Temporary;
}
//------------------------------------------------------------------------------

bool BASE::ApplyAttributes(AST::ASSIGNMENT* AttributeList)
{
    while(AttributeList){
        assert(AttributeList->Left , return false);
        assert(AttributeList->Right, return false);
        assert(AttributeList->Left->Type == AST::BASE::TYPE::Identifier, return false);

        auto Name  = ((AST::IDENTIFIER*)AttributeList->Left)->Name;
        auto Value = AttributeList->Right;
        AttributeList->Right = 0;

        switch(Value->Type){
            case AST::BASE::TYPE::String:
            case AST::BASE::TYPE::Literal:
                break;

            case AST::BASE::TYPE::Array:
                // TODO Make sure that the array only contains strings or literals
                break;

            default:
                AttributeList->Error("Attribute values must be strings, literals or arrays");
                delete Value;
                return false;
        }
        auto Attribute = Attributes[Name];
        if(!Attribute){
            Attribute = new ATTRIBUTE(Value->Source.Line, Value->Source.Filename, Name.c_str());
            Attributes[Name] = Attribute;
        }
        Attribute->Assign(Value);

        AttributeList = (AST::ASSIGNMENT*)AttributeList->Next;
    }
    return true;
}
//------------------------------------------------------------------------------

string& BASE::HDL_Name()
{
    static string Result;
    string SafeName; // Unicode converted to hex

    int n;
    for(n = 0; Name[n]; n++){
        if(Name[n] & 0x80) break;
    }
    if(Name[n]){ // Escape Unicode characters
        char s[16];
        SafeName = "u..";
        for(int n = 0; Name[n]; n++){
            sprintf(s, "%02X", (unsigned)((uint8_t)Name[n]));
            SafeName += s;
        }
    }else{
        SafeName = Name;
    }

    if(Namespace){
        switch(Namespace->Type){
            case TYPE::Module: Result = SafeName; break;
            case TYPE::Group:  Result = Namespace->HDL_Name() + "." + SafeName; break;
            default:;
        }
    }else{
        Result = SafeName;
    }
    return Result;
}
//------------------------------------------------------------------------------

string& BASE::EscapedName()
{
    static string Result;

    string Name(HDL_Name());

    for(int n = 0; Name[n]; n++){
        if(Name[n] >= '0' && Name[n] <= '9' && n > 0) continue;
        if(Name[n] >= 'a' && Name[n] <= 'z') continue;
        if(Name[n] >= 'A' && Name[n] <= 'Z') continue;
        if(Name[n] == '_') continue;
        Result = "\\" + Name + " ";
        return Result;
    }
    Result = Name;
    return Result;
}
//------------------------------------------------------------------------------

void BASE::DisplayLongName()
{
    if(Namespace != (NAMESPACE*)&Global){
        Namespace->DisplayLongName();
        Debug.Print("::");
    }
    Debug.Print("%s", Name.c_str());
}
//------------------------------------------------------------------------------

void BASE::DisplayAttributes(int Indent)
{
    Debug.Indent(Indent);
    Debug.Print("Line       = %d\n", Source.Line);

    Debug.Indent(Indent);
    Debug.Print("Filename   = \"%s\"\n", Source.Filename);

    Debug.Indent(Indent);
    Debug.Print("Attributes:\n");

    Indent++;
    foreach(Attribute, Attributes){
        assert(Attribute->second, break);
        Attribute->second->Display(Indent);
    }
}
//------------------------------------------------------------------------------

BASE* BASE::GetAttribute(const std::string& Name)
{
    auto Attribute = Attributes.find(Name);
    if(Attribute != Attributes.end()) return Attribute->second;
    return 0;
}
//------------------------------------------------------------------------------

BASE* BASE::GetMember(const std::string& Name)
{
    return 0; // By default, objects do not have members
}
//------------------------------------------------------------------------------

AST::EXPRESSION* BASE::GetAttribValue(const string& Name)
{
    auto Attribute = GetAttribute(Name);
    if(Attribute){
        // Check for accidental built-in attribute access
        assert(Attribute->Type == TYPE::Attribute, return 0);
        return ((ATTRIBUTE*)Attribute)->Value;
    }
    if(Namespace) return Namespace->GetAttribValue(Name);
    return 0;
}
//------------------------------------------------------------------------------

AST::EXPRESSION* BASE::GetBuiltInAttributeValue(const string& Name)
{
    // TODO: Build stuff like Line, Filename, etc.
    return 0;
}
//------------------------------------------------------------------------------

int BASE::Width()
{
    return 0;
}
//------------------------------------------------------------------------------

NUMBER& BASE::FullScale()
{
    static NUMBER Zero = 0;
    return Zero;
}
//------------------------------------------------------------------------------

bool BASE::Signed()
{
    return false;
}
//------------------------------------------------------------------------------

void BASE::PopulateUsed(bool SetUsed)
{}
//------------------------------------------------------------------------------

void BASE::Validate()
{
    // Don't verify Namespace, it's circular
    foreach(Attrib, Attributes){
        assert(Attrib->first == Attrib->second->Name);
        Attrib->second->Validate();
    }
}
//------------------------------------------------------------------------------

