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

#include "Number.h"
//------------------------------------------------------------------------------

#include "AST/AccessDirectionGroup.h"
#include "AST/Alias.h"
#include "AST/Array.h"
#include "AST/Assert.h"
#include "AST/Assignment.h"
#include "AST/ClassDefinition.h"
#include "AST/ClockedConstruct.h"
#include "AST/Concatenate.h"
#include "AST/ControlStatement.h"
#include "AST/EnumDefinition.h"
#include "AST/Expression.h"
#include "AST/Fence.h"
#include "AST/ForkJoin.h"
#include "AST/FunctionCall.h"
#include "AST/FunctionDef.h"
#include "AST/GroupDefinition.h"
#include "AST/HdlConstruct.h"
#include "AST/Identifier.h"
#include "AST/Import.h"
#include "AST/InterpolatedString.h"
#include "AST/Jump.h"
#include "AST/Label.h"
#include "AST/Literal.h"
#include "AST/NameSpacePush.h"
#include "AST/OperatorOverload.h"
#include "AST/ParameterDef.h"
#include "AST/Slice.h"
#include "AST/StimulusOrEmulate.h"
#include "AST/AST_String.h"
#include "AST/Stringify.h"
#include "AST/VariableDef.h"
#include "AST/Wait.h"
//------------------------------------------------------------------------------

#include "Symbols/Num.h"
#include "Symbols/NameSpace.h"
//------------------------------------------------------------------------------

class Interpreter{
    private: // Local Objects

    private: // Interpreter State
        bool error = false;

    private: // Helper functions
        tm* time();

    private: // Interpreter Functions
        void printError(AST::AST* node, const char* message);

        bool print(AST::AST*        node);
        bool print(AST::Literal*    node);
        bool print(AST::String*     node);
        bool print(AST::Expression* node);
        bool print(AST::Identifier* node);

        bool assign(AST::AST*        target, AST::AST* expression);
        bool assign(AST::Identifier* target, AST::AST* expression);
        bool assign(Symbols::Num*    target, AST::AST* expression);

        Number evaluate(AST::AST*        expression);
        Number evaluate(AST::Literal*    expression);
        Number evaluate(AST::Expression* expression);
        Number evaluate(AST::Identifier* expression);

        bool variableDef     (AST::VariableDef*      node);
        bool functionDef     (AST::FunctionDef*      node);
        bool operatorOverload(AST::OperatorOverload* node);
        bool assignment      (AST::Assignment*       node);
        bool functionCall    (AST::FunctionCall*     node);

        bool globalSpace(AST::AST* node);

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
