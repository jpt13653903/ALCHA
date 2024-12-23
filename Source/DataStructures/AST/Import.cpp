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

#include "Import.h"
#include "Engine.h"
#include "Netlist/NameSpace/Module.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Import::Import(int line, string& filename): Import(line, filename.c_str()){}
//------------------------------------------------------------------------------

Import::Import(int line, const char* filename): Base(line, filename, Type::Import)
{
    ast = 0;
}
//------------------------------------------------------------------------------

Import::~Import()
{
    if(ast) delete ast;
}
//------------------------------------------------------------------------------

Base* Import::copy()
{
    Import* copy = new Import(source.line, source.filename.c_str());

    copy->file      = file;
    copy->nameSpace = nameSpace;

    return copy;
}
//------------------------------------------------------------------------------

bool Import::runAST()
{
    string importFilename;
    bool   ownNameSpace = !nameSpace.empty();

    Netlist::Module* module = 0;

    if(ownNameSpace){
        auto found = Netlist::nameSpaceStack.front()->symbols.find(nameSpace);
        if(found != Netlist::nameSpaceStack.front()->symbols.end()){
            printError();
            printf("Symbol \"%s\" already exists in the current namespace\n",
                          nameSpace.c_str());
            return false;
        }
        module = new Netlist::Module(source.line, source.filename, nameSpace.c_str());
        Netlist::nameSpaceStack.front()->symbols[nameSpace] = module;
        Netlist::nameSpaceStack.push_front(module);
    }

    string& path = source.filename;
    int n;
    for(n = path.length()-1; n >= 0; n--){
        if(path[n] == '/') break;
    }
    if(n >= 0) importFilename = path.substr(0, n+1);
    importFilename += file;
    importFilename += ".alc";
    simplifyFilename(importFilename);
    logger.print("\nFilename = %s\n", importFilename);

    auto ast = engine->runAST(importFilename.c_str());
    if(!ast) return false;

    if(ownNameSpace){
        module->ast = ast;
        Netlist::nameSpaceStack.pop_front();

    }else{
        this->ast = ast;
    }
    return true;
}
//------------------------------------------------------------------------------

bool Import::getVerilog(string& body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Import::display()
{
    displayInfo();
    logger.print("import \"%s\"", file.c_str());
    if(nameSpace.empty()) logger.print("\n");
    else                  logger.print(" as %s\n", nameSpace.c_str());

    if(ast ) ast ->display();
    if(next) next->display();
}
//------------------------------------------------------------------------------

void Import::validateMembers()
{
    assert(type == Type::Import);

    if(ast) ast->validate();
}
//------------------------------------------------------------------------------

