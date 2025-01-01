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
        const tm* time() const;

    private: // Interpreter Functions
        void printError(const AST::AST* node, const char* message);

        bool print(const AST::AST*        node);
        bool print(const AST::Literal*    node);
        bool print(const AST::String*     node);
        bool print(const AST::Expression* node);
        bool print(const AST::Identifier* node);

        bool assign(const AST::AST*        target, const AST::AST* expression);
        bool assign(const AST::Identifier* target, const AST::AST* expression);
        bool assign(      Symbols::Num*    target, const AST::AST* expression);

        Number evaluate(const AST::AST*        expression);
        Number evaluate(const AST::Literal*    expression);
        Number evaluate(const AST::Expression* expression);
        Number evaluate(const AST::Identifier* expression);

        bool variableDef     (const AST::VariableDef*      node);
        bool functionDef     (const AST::FunctionDef*      node);
        bool operatorOverload(const AST::OperatorOverload* node);
        bool import          (const AST::Import*           node);
        bool assignment      (const AST::Assignment*       node);
        bool functionCall    (const AST::FunctionCall*     node);

        bool globalSpace(const AST::AST* node);

    public: // Testable structures
        Symbols::NameSpace global;

    public:
        Interpreter();
       ~Interpreter();

       bool run(const AST::AST* ast);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
