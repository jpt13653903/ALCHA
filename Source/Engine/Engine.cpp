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

#include "Engine.h"
#include "Netlist/NameSpace/Module.h"
//------------------------------------------------------------------------------

Engine* engine;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

Engine::Engine()
{
    engine = this; // There is only one instance, created in ALCHA.cpp::main()
}
//------------------------------------------------------------------------------

Engine::~Engine(){}
//------------------------------------------------------------------------------

AST::Base* Engine::runAST(const char* filename)
{
    AST::Base* ast = parser.run(filename);
    if(!ast) return 0;

    auto node = ast;
    while(node){
        if(!node->runAST()){
            delete ast;
            return 0;
        }
        node = node->next;
    }
    return ast;
}
//------------------------------------------------------------------------------

bool Engine::run(const char* filename)
{
    assert(Netlist::nameSpaceStack.empty());

    Netlist::nameSpaceStack.push_front(&Netlist::global);

    Netlist::global.ast = runAST(filename);
    if(!Netlist::global.ast) return 0;

    #ifdef DEBUG
        logger.print(ANSI_FG_GREEN "\nDisplaying global AST ");
        logger.print(filename);
        logger.print(" -------------------------------------\n\n" ANSI_RESET);
        if(Netlist::global.ast) Netlist::global.ast->display();
        else                    logger.print("AST is empty\n");

        logger.print(ANSI_FG_GREEN "\nDisplaying global Symbols ");
        logger.print(filename);
        logger.print(" -------------------------------------\n\n" ANSI_RESET);
        Netlist::global.display();

        for(auto nameSpace: Netlist::nameSpaceStack){
            if(nameSpace->ast) nameSpace->ast->validate();
        }
        Netlist::global.validate();
    #endif

    return Netlist::global.ast;
}
//------------------------------------------------------------------------------

