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

#include "Logger.h"
#include "Messages.h"

#include "Scanner.h"
#include "Dictionary.h"

#include "AST/RTL.h"
#include "AST/FSM.h"
#include "AST/HDL.h"
#include "AST/Jump.h"
#include "AST/Fence.h"
#include "AST/Alias.h"
#include "AST/Group.h"
#include "AST/Import.h"
#include "AST/Switch.h"
#include "AST/ForLoop.h"
#include "AST/LoopLoop.h"
#include "AST/WhileLoop.h"
#include "AST/Parameter.h"
#include "AST/Definition.h"
#include "AST/Assignment.h"
#include "AST/IfStatement.h"
#include "AST/NamespacePush.h"
#include "AST/EnumDefinition.h"
#include "AST/ClassDefinition.h"
//------------------------------------------------------------------------------

class PARSER{
  private:
    bool error;
    void Error(const char* Message);

    TOKEN   Token; // The current token
    SCANNER Scanner;

    void GetToken();

    AST::ASSIGNMENT* AttributeAssignment();
    AST::ASSIGNMENT* AttributeList      ();

    AST::EXPRESSION* String    ();
    AST::EXPRESSION* Literal   ();
    AST::EXPRESSION* Identifier();

    AST::EXPRESSION* ExpressionList ();
    AST::BASE*       Parameter      ();
    AST::BASE*       ParameterList  ();
    AST::EXPRESSION* Array          ();
    AST::EXPRESSION* ArrayConcat    ();
    AST::EXPRESSION* VectorConcat   ();

    AST::EXPRESSION* Primary        ();
    AST::EXPRESSION* CastEpr        (AST::EXPRESSION* Node);
    AST::EXPRESSION* Cast           ();
    AST::EXPRESSION* Postfix        ();
    AST::EXPRESSION* Unary          ();
    AST::EXPRESSION* Range          ();
    AST::EXPRESSION* Reduction      ();
    AST::EXPRESSION* Stringification();
    AST::EXPRESSION* Replication    ();
    AST::EXPRESSION* Exponential    ();
    AST::EXPRESSION* Multiplicative ();
    AST::EXPRESSION* Additive       ();
    AST::EXPRESSION* Shift          ();
    AST::EXPRESSION* Relational     ();
    AST::EXPRESSION* Equality       ();
    AST::EXPRESSION* BitwiseAND     ();
    AST::EXPRESSION* BitwiseXOR     ();
    AST::EXPRESSION* BitwiseOR      ();
    AST::EXPRESSION* Expression     (); // AKA Conditional
    AST::EXPRESSION* TypeIdentifier ();

    AST::ASSIGNMENT            * Initialiser    (std::string& Identifier);
    AST::DEFINITION::ARRAY     * ArrayDefinition();
    AST::DEFINITION::IDENTIFIER* IdentifierList ();

    // Define function parameters
    AST::PARAMETER* DefParameter    ();
    AST::PARAMETER* DefParameterList();

    bool ValidNamespaceSpecifier(AST::EXPRESSION* Node); // Used by Other()
    bool ValidTypeSpecifier     (AST::EXPRESSION* Node); // Used by Other()
    bool ValidLHS               (AST::EXPRESSION* Node); // Used by Other()

    AST::CLASS_DEFINITION * ClassDefinition();
    AST::ENUM_DEFINITION  * EnumDefinition();
    AST::IF_STATEMENT     * IfStatement();
    AST::DEFINITION       * Definition();
    AST::WHILE_LOOP       * WhileLoop();
    AST::LOOP_LOOP        * LoopLoop();
    AST::FOR_LOOP         * ForLoop();
    AST::IMPORT           * Import();
    AST::SWITCH           * Switch();
    AST::ALIAS            * Alias();
    AST::GROUP            * Group();
    AST::BASE             * Other();
    AST::JUMP             * Jump();
    AST::RTL              * RTL();
    AST::FSM              * FSM();
    AST::HDL              * HDL();

    AST::BASE* Statement     ();
    AST::BASE* Statements    ();
    AST::BASE* StatementBlock();

  public:
    PARSER();
   ~PARSER();

    AST::BASE* Run(const char* Filename);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
