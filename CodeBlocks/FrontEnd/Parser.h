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

#include "Scanner.h"
#include "Dictionary.h"

#include "AST_RTL.h"
#include "AST_FSM.h"
#include "AST_HDL.h"
#include "AST_Jump.h"
#include "AST_Fence.h"
#include "AST_Import.h"
#include "AST_Switch.h"
#include "AST_ForLoop.h"
#include "AST_LoopLoop.h"
#include "AST_WhileLoop.h"
#include "AST_Parameter.h"
#include "AST_Definition.h"
#include "AST_Assignment.h"
#include "AST_IfStatement.h"
#include "AST_NamespacePush.h"
#include "AST_ClassDefinition.h"
#include "AST_TargetDefinition.h"
//------------------------------------------------------------------------------

class PARSER{
 private:
  bool error;
  void Error(const char* Message);

  TOKEN    Token;    // The current token
  SCANNER* Scanner;  // The scanner (linked-list based stack)

  void GetToken();

  bool AttributeAssignment(DICTIONARY* Attributes);
  bool AttributeList      (DICTIONARY* Attributes);

  AST_Expression* String    ();
  AST_Expression* Literal   ();
  AST_Expression* Identifier();

  AST_Expression* ParameterList();

  AST_Expression* Primary       ();
  AST_Expression* Postfix       ();
  AST_Expression* Unary         ();
  AST_Expression* Range         ();
  AST_Expression* Reduction     ();
  AST_Expression* FP_Cast       (AST_Expression* Node);
  AST_Expression* Cast          ();
  AST_Expression* Concatenation ();
  AST_Expression* Replication   ();
  AST_Expression* Array         ();
  AST_Expression* Exponential   ();
  AST_Expression* Multiplicative();
  AST_Expression* Additive      ();
  AST_Expression* Shift         ();
  AST_Expression* Relational    ();
  AST_Expression* Equality      ();
  AST_Expression* BitwiseAND    ();
  AST_Expression* BitwiseXOR    ();
  AST_Expression* BitwiseOR     ();
  AST_Expression* LogicalAND    ();
  AST_Expression* LogicalOR     ();
  AST_Expression* Expression    (); // AKA Conditional

  AST_Assignment            * Initialiser    (byte* Identifier);
  AST_Definition::ARRAY     * ArrayDefinition();
  AST_Definition::IDENTIFIER* IdentifierList ();

  // Define function parameters
  AST_Parameter* DefParameter    ();
  AST_Parameter* DefParameterList();

  bool ValidNamespaceSpecifier(AST_Expression* Node); // Used by Other()
  bool ValidTypeSpecifier     (AST_Expression* Node); // Used by Other()
  bool ValidLHS               (AST_Expression* Node); // Used by Other()

  AST_TargetDefinition* TargetDefinition();
  AST_ClassDefinition * ClassDefinition();
  AST_IfStatement     * IfStatement();
  AST_Definition      * Definition();
  AST_WhileLoop       * WhileLoop();
  AST_LoopLoop        * LoopLoop();
  AST_ForLoop         * ForLoop();
  AST_Import          * Import();
  AST_Switch          * Switch();
  AST_Jump            * Jump();
  AST_Base            * Other();
  AST_RTL             * RTL();
  AST_FSM             * FSM();
  AST_HDL             * HDL();

  AST_Base* Statement     ();
  AST_Base* Statements    ();
  AST_Base* StatementBlock();

 public:
  PARSER();
 ~PARSER();

  AST_Base* Run(const byte* Filename);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
