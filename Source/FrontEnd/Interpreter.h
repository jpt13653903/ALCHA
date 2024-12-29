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

#ifndef Interpreter_h
#define Interpreter_h
//------------------------------------------------------------------------------

#include "AST.h"
#include "NameSpace.h"
//------------------------------------------------------------------------------

class Interpreter{
    private: // Local Objects
        AST::AST* ast;

    private: // Interpreter State
        bool error = false;

    private: // Interpreter Functions
        void printError(const char* message);

        bool variableDef();
        bool functionDef();
        bool operatorOverload();
        bool assignment();
        bool functionCall();

    public: // Testable structures
        Symbols::NameSpace global;

    public:
        Interpreter();
       ~Interpreter();

       bool run(AST::AST* ast);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
