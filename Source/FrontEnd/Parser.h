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
#include "Utilities.h"

#include "Scanner.h"
#include "Dictionary.h"

#include "AST/Alias.h"
#include "AST/Class_Definition.h"
#include "AST/Enum_Definition.h"
#include "AST/Fence.h"
#include "AST/ForLoop.h"
#include "AST/FSM.h"
#include "AST/Group.h"
#include "AST/HDL.h"
#include "AST/IfStatement.h"
#include "AST/Import.h"
#include "AST/Jump.h"
#include "AST/LoopLoop.h"
#include "AST/NamespacePush.h"
#include "AST/RTL.h"
#include "AST/Switch.h"
#include "AST/WhileLoop.h"

#include "AST/Assignment/Add_Assign.h"
#include "AST/Assignment/AND_Assign.h"
#include "AST/Assignment/Append_Assign.h"
#include "AST/Assignment/Assign.h"
#include "AST/Assignment/Divide_Assign.h"
#include "AST/Assignment/Exponential_Assign.h"
#include "AST/Assignment/Modulus_Assign.h"
#include "AST/Assignment/Multiply_Assign.h"
#include "AST/Assignment/OR_Assign.h"
#include "AST/Assignment/Raw_Assign.h"
#include "AST/Assignment/Shift_Left_Assign.h"
#include "AST/Assignment/Shift_Right_Assign.h"
#include "AST/Assignment/Subtract_Assign.h"
#include "AST/Assignment/XOR_Assign.h"

#include "AST/Definition/Auto_Definition.h"
#include "AST/Definition/Byte_Definition.h"
#include "AST/Definition/Char_Definition.h"
#include "AST/Definition/Class_Instance.h"
#include "AST/Definition/FuncPtr_Definition.h"
#include "AST/Definition/Net_Definition.h"
#include "AST/Definition/Num_Definition.h"
#include "AST/Definition/Pin_Definition.h"
#include "AST/Definition/Void_Definition.h"

#include "AST/Expression/AccessAttribute.h"
#include "AST/Expression/AccessMember.h"
#include "AST/Expression/AccessMemberSafe.h"
#include "AST/Expression/Add.h"
#include "AST/Expression/AND_Reduce.h"
#include "AST/Expression/Array.h"
#include "AST/Expression/ArrayConcatenate.h"
#include "AST/Expression/Bit_AND.h"
#include "AST/Expression/Bit_NAND.h"
#include "AST/Expression/Bit_NOR.h"
#include "AST/Expression/Bit_NOT.h"
#include "AST/Expression/Bit_OR.h"
#include "AST/Expression/Bit_XNOR.h"
#include "AST/Expression/Bit_XOR.h"
#include "AST/Expression/Cast.h"
#include "AST/Expression/Conditional.h"
#include "AST/Expression/Decrement.h"
#include "AST/Expression/Divide.h"
#include "AST/Expression/Equal.h"
#include "AST/Expression/Exponential.h"
#include "AST/Expression/Factorial.h"
#include "AST/Expression/FunctionCall.h"
#include "AST/Expression/Greater.h"
#include "AST/Expression/Greater_Equal.h"
#include "AST/Expression/Identifier.h"
#include "AST/Expression/Increment.h"
#include "AST/Expression/Less.h"
#include "AST/Expression/Less_Equal.h"
#include "AST/Expression/Literal.h"
#include "AST/Expression/Logical_AND.h"
#include "AST/Expression/Logical_NOT.h"
#include "AST/Expression/Logical_OR.h"
#include "AST/Expression/Modulus.h"
#include "AST/Expression/Multiply.h"
#include "AST/Expression/NAND_Reduce.h"
#include "AST/Expression/Negate.h"
#include "AST/Expression/NOR_Reduce.h"
#include "AST/Expression/Not_Equal.h"
#include "AST/Expression/Object.h"
#include "AST/Expression/OR_Reduce.h"
#include "AST/Expression/Range.h"
#include "AST/Expression/Raw.h"
#include "AST/Expression/Replicate.h"
#include "AST/Expression/Shift_Left.h"
#include "AST/Expression/Shift_Right.h"
#include "AST/Expression/Slice.h"
#include "AST/Expression/String.h"
#include "AST/Expression/Subtract.h"
#include "AST/Expression/VectorConcatenate.h"
#include "AST/Expression/XNOR_Reduce.h"
#include "AST/Expression/XOR_Reduce.h"
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

    bool             ExpressionList (std::vector<AST::EXPRESSION*>& List);
    AST::BASE*       Parameter      ();
    bool             ParameterList  (std::list<AST::BASE*>& Parameters);
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
    AST::DEFINITION* DefParameter    ();
    AST::DEFINITION* DefParameterList();

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
