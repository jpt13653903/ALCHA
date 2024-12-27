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

        AST::AST* expressionList();
        AST::AST* parameterList();
        AST::AST* parameter();
        AST::AST* attributeList();
        AST::AST* attributeAssignment();

        AST::AST* identifierStatement();
        AST::AST* functionDef(Token::Type type, std::string& identifier);
        AST::AST* variableDefList(Token::Type type);
        AST::AST* definition();
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
