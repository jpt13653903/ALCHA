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
#include "NameSpace/Module.h"
#include "AST/Expression/Identifier.h"
//------------------------------------------------------------------------------

using std::string;
using std::to_string;
using namespace Netlist;
//------------------------------------------------------------------------------

Base::Base(int line, const string& filename, const char* name, Type type)
{
    source.line      = line;
    source.filename  = filename;
    this->type       = type;
    this->nameSpace  = 0;

    static unsigned genNameCount = 0;
    if(name){
        temporary = false;
        this->name = name;
    }else{
        temporary = true;
        switch(type){
            case Type::Net   : this->name = "w.."; break;
            case Type::Module: this->name = "m.."; break;
            default          : this->name = "t.."; break;
        }
        if(genNameCount < 1000000) this->name += '0';
        if(genNameCount <  100000) this->name += '0';
        if(genNameCount <   10000) this->name += '0';
        if(genNameCount <    1000) this->name += '0';
        if(genNameCount <     100) this->name += '0';
        if(genNameCount <      10) this->name += '0';
        this->name += to_string(genNameCount++);
    }

    if(!nameSpaceStack.empty()) nameSpace = nameSpaceStack.front();
}
//------------------------------------------------------------------------------

Base::~Base()
{
    for(auto attribute: attributes) delete attribute.second;
}
//------------------------------------------------------------------------------

void Base::printError(const char* message)
{
    ::printError(source.line, source.filename, message);
}
//------------------------------------------------------------------------------

void Base::printWarning(const char* message)
{
    ::printWarning(source.line, source.filename.c_str(), message);
}
//------------------------------------------------------------------------------

bool Base::isSynthesisable()
{
    return false;
}
//------------------------------------------------------------------------------

bool Base::isNamespace()
{
    return false;
}
//------------------------------------------------------------------------------

bool Base::isTemporary()
{
    return temporary;
}
//------------------------------------------------------------------------------

bool Base::applyAttributes(AST::Assignment* attributeList)
{
    while(attributeList){
        assert(attributeList->left , return false);
        assert(attributeList->right, return false);
        assert(attributeList->left->type == AST::Base::Type::Identifier, return false);

        auto name  = ((AST::Identifier*)attributeList->left)->name;
        auto value = attributeList->right;
        attributeList->right = 0;

        switch(value->type){
            case AST::Base::Type::String:
            case AST::Base::Type::Literal:
                break;

            case AST::Base::Type::Array:
                // TODO Make sure that the array only contains strings or literals
                break;

            default:
                attributeList->printError("attribute values must be strings, literals or arrays");
                delete value;
                return false;
        }
        auto attribute = attributes[name];
        if(!attribute){
            attribute = new Attribute(value->source.line, value->source.filename, name.c_str());
            attributes[name] = attribute;
        }
        attribute->assign(value);

        attributeList = (AST::Assignment*)attributeList->next;
    }
    return true;
}
//------------------------------------------------------------------------------

string& Base::hdlName()
{
    static string result;
    string safeName; // Unicode converted to hex

    int n;
    for(n = 0; name[n]; n++){
        if(name[n] & 0x80) break;
    }
    if(name[n]){ // Escape Unicode characters
        char s[16];
        safeName = "u..";
        for(int n = 0; name[n]; n++){
            sprintf(s, "%02X", (unsigned)((uint8_t)name[n]));
            safeName += s;
        }
    }else{
        safeName = name;
    }

    if(nameSpace){
        switch(nameSpace->type){
            case Type::Module: result = safeName; break;
            case Type::Group:  result = nameSpace->hdlName() + "." + safeName; break;
            default:;
        }
    }else{
        result = safeName;
    }
    return result;
}
//------------------------------------------------------------------------------

string& Base::escapedName()
{
    static string result;

    string name(hdlName());

    for(int n = 0; name[n]; n++){
        if(name[n] >= '0' && name[n] <= '9' && n > 0) continue;
        if(name[n] >= 'a' && name[n] <= 'z') continue;
        if(name[n] >= 'A' && name[n] <= 'Z') continue;
        if(name[n] == '_') continue;
        result = "\\" + name + " ";
        return result;
    }
    result = name;
    return result;
}
//------------------------------------------------------------------------------

void Base::displayLongName()
{
    if(nameSpace != (NameSpace*)&global){
        nameSpace->displayLongName();
        debug.print("::");
    }
    debug.print("%s", name.c_str());
}
//------------------------------------------------------------------------------

void Base::displayAttributes(int indent)
{
    debug.indent(indent);
    debug.print("Line       = %d\n", source.line);

    debug.indent(indent);
    debug.print("Filename   = \"%s\"\n", source.filename);

    debug.indent(indent);
    debug.print("Attributes:\n");

    indent++;
    for(auto attribute: attributes){
        assert(attribute.second, break);
        attribute.second->display(indent);
    }
}
//------------------------------------------------------------------------------

Base* Base::getAttribute(const std::string& name)
{
    auto attribute = attributes.find(name);
    if(attribute != attributes.end()) return attribute->second;
    return 0;
}
//------------------------------------------------------------------------------

Base* Base::getMember(const std::string& name)
{
    return 0; // By default, objects do not have members
}
//------------------------------------------------------------------------------

AST::Expression* Base::getAttribValue(const string& name)
{
    auto attribute = getAttribute(name);
    if(attribute){
        // Check for accidental built-in attribute access
        assert(attribute->type == Type::Attribute, return 0);
        return ((Attribute*)attribute)->value;
    }
    if(nameSpace) return nameSpace->getAttribValue(name);
    return 0;
}
//------------------------------------------------------------------------------

AST::Expression* Base::getBuiltInAttributeValue(const string& name)
{
    // TODO: Build stuff like line, filename, etc.
    return 0;
}
//------------------------------------------------------------------------------

int Base::width()
{
    return 0;
}
//------------------------------------------------------------------------------

Number& Base::fullScale()
{
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool Base::isSigned()
{
    return false;
}
//------------------------------------------------------------------------------

void Base::populateUsed(bool setUsed){}
//------------------------------------------------------------------------------

void Base::validate()
{
    // Don't verify nameSpace, it's circular
    for(auto attrib: attributes){
        assert(attrib.first == attrib.second->name);
        attrib.second->validate();
    }
}
//------------------------------------------------------------------------------

