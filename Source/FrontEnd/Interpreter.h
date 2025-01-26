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

#include <list>
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
#include "AST/CoverBins.h"
#include "AST/CoverGroup.h"
#include "AST/CycleDelay.h"
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
#include "AST/Repetition.h"
#include "AST/SequenceDef.h"
#include "AST/Slice.h"
#include "AST/StimulusOrEmulate.h"
#include "AST/AST_String.h"
#include "AST/Stringify.h"
#include "AST/VariableDef.h"
#include "AST/Wait.h"
//------------------------------------------------------------------------------

#include "Objects/Num.h"
#include "Objects/NameSpace.h"
#include "Objects/Objects_String.h"
//------------------------------------------------------------------------------

class Interpreter{
    private: // Local stuff
        std::list<Objects::Object*> tempStack;
        void clearTempStack();


    private: // Interpreter State
        bool error = false;
        void printError(const AST::AST* node, const char* message);

    private: // Helper functions
        const tm* time() const;

    private: // Evaluate Infix
        Objects::Object* evaluateInfix(const AST::Expression* node, const Objects::Object* left, const Objects::Object* right);
        Objects::Object* evaluateInfix(const AST::Expression* node, const Objects::Num*    left, const Objects::Num*    right);

    private: // Evaluate Prefix
        Objects::Object* evaluatePrefix(const AST::Expression* node, const Objects::Object* right);
        Objects::Object* evaluatePrefix(const AST::Expression* node, const Objects::Num*    right);

    private: // Evaluate Postfix
        Objects::Object* evaluatePostfix(const AST::Expression* node, const Objects::Object* left);
        Objects::Object* evaluatePostfix(const AST::Expression* node, const Objects::Num*    left);

    private: // Evaluate
        Objects::Object* evaluate(const AST::AST*                node);
        Objects::Object* evaluate(const AST::Array*              node);
        Objects::Object* evaluate(const AST::Concatenate*        node);
        Objects::Object* evaluate(const AST::Expression*         node);
        Objects::Object* evaluate(const AST::Identifier*         node);
        Objects::Object* evaluate(const AST::InterpolatedString* node);
        Objects::Object* evaluate(const AST::Literal*            node);
        Objects::Object* evaluate(const AST::Repetition*         node);
        Objects::Object* evaluate(const AST::Slice*              node);
        Objects::Object* evaluate(const AST::String*             node);
        Objects::Object* evaluate(const AST::Stringify*          node);

    private: // Assign
        bool assign(const AST::AST*        target, const AST::AST* expression);
        bool assign(const AST::Identifier* target, const AST::AST* expression);
        bool assign(      Objects::Num*    target, const AST::AST* expression);

    private: // Print
        bool print(const AST::AST* node);

    private: // Execute
        bool execute(const AST::AST*                  node);
        bool execute(const AST::AccessDirectionGroup* node);
        bool execute(const AST::Alias*                node);
        bool execute(const AST::Assert*               node);
        bool execute(const AST::Assignment*           node);
        bool execute(const AST::ClassDefinition*      node);
        bool execute(const AST::ClockedConstruct*     node);
        bool execute(const AST::ControlStatement*     node);
        bool execute(const AST::CoverBins*            node);
        bool execute(const AST::CoverGroup*           node);
        bool execute(const AST::CycleDelay*           node);
        bool execute(const AST::EnumDefinition*       node);
        bool execute(const AST::Fence*                node);
        bool execute(const AST::ForkJoin*             node);
        bool execute(const AST::FunctionCall*         node);
        bool execute(const AST::FunctionDef*          node);
        bool execute(const AST::GroupDefinition*      node);
        bool execute(const AST::HdlConstruct*         node);
        bool execute(const AST::Import*               node);
        bool execute(const AST::Jump*                 node);
        bool execute(const AST::NameSpacePush*        node);
        bool execute(const AST::OperatorOverload*     node);
        bool execute(const AST::ParameterDef*         node);
        bool execute(const AST::SequenceDef*          node);
        bool execute(const AST::StimulusOrEmulate*    node);
        bool execute(const AST::VariableDef*          node);
        bool execute(const AST::Wait*                 node);

    public: // Testable structures
        Objects::NameSpace global;

    public:
        Interpreter();
       ~Interpreter();

       bool run(const AST::AST* ast);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
