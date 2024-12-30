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

#ifndef Parser_h
#define Parser_h
//------------------------------------------------------------------------------

#include "General.h"
#include "Scanner.h"
#include "AST.h"
#include "AST/Wait.h"
//------------------------------------------------------------------------------

class Parser{
    private: // Local Objects
        Token   token; // The current token
        Scanner scanner;
        int     astFilenameIndex = 0;

    private: // Parser State
        bool error    = false;
        bool isInline = false;

    private: // Parser Functions
        void printError(const char* message);

        bool getToken();

        AST::AST* expression();
        AST::AST* bitwiseOr();
        AST::AST* bitwiseXor();
        AST::AST* bitwiseAnd();
        AST::AST* equality();
        AST::AST* relational();
        AST::AST* shift();
        AST::AST* additive();
        AST::AST* multiplicative();
        AST::AST* exponential();
        AST::AST* replication();
        AST::AST* stringification();
        AST::AST* reduction();
        AST::AST* range();
        AST::AST* unary();
        AST::AST* postfix();
        AST::AST* accessor();
        AST::AST* castExpression();
        AST::AST* primary();
        AST::AST* concatenate();
        AST::AST* array();
        AST::AST* interpolatedString();

        AST::AST* expressionList();
        AST::AST* parameterList();
        AST::AST* parameter();
        AST::AST* attributeList();
        AST::AST* attributeAssignment();

        AST::AST* definition();
        AST::AST* typeIdentifier();
        AST::AST* classDefinition();
        AST::AST* accessDirectionGroup();
        AST::AST* variableDefList (bool         isInline,
                                   Token::Type  defType,
                                   AST::AST*    typeIdentifier,
                                   AST::AST*    parameters,
                                   AST::AST*    attributes);
        AST::AST* functionDef     (bool         isInline,
                                   Token::Type  returnType,
                                   AST::AST*    typeIdentifier,
                                   AST::AST*    parameters,
                                   AST::AST*    attributes,
                                   std::string& identifier,
                                   AST::AST*    arrayDefs);
        AST::AST* operatorOverload(bool         isInline,
                                   Token::Type  returnType,
                                   AST::AST*    typeIdentifier,
                                   AST::AST*    parameters,
                                   AST::AST*    attributes);
        AST::AST* operatorOverload(Token::Type type, std::string& identifier);
        AST::AST* arrayDefinition();
        AST::AST* parameterDefList();
        AST::AST* parameterDef();
        AST::AST* enumDefinition();

        AST::AST* alias();
        AST::AST* import();
        AST::AST* structDefinition();
        AST::AST* groupDefinition();
        AST::AST* structBody();
        AST::AST* ifStatement();
        AST::AST* forStatement();
        AST::AST* loopOrSwitchStatement();
        AST::AST* caseStatement();
        AST::AST* jumpStatement();
        AST::AST* clockedConstruct();
        AST::AST* hdlConstruct();
        AST::AST* hdlParameterList();
        AST::AST* hdlParameter();
        AST::AST* hdlBody();

        AST::AST* stimulusOrEmulate();
        AST::AST* forkJoin();
        AST::AST* wait();
        AST::Wait::SensitivityItem* sensitivityList();

        AST::AST* sequenceDef();
        AST::AST* assertion();
        AST::AST* assertBlock();
        AST::AST* assertStatement();
        AST::AST* assertIf();
        AST::AST* sequence();
        AST::AST* matchOr();
        AST::AST* matchAnd();
        AST::AST* cycleDelay();
        AST::AST* repetition();

        AST::AST* identifierStatement();
        AST::AST* statementBlock();
        AST::AST* statement();
        AST::AST* statements();

    public:
        Parser();
       ~Parser();

        AST::AST* parse(const char* filename);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
