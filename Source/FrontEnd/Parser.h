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
#include "AST/ClassDefinition.h"
#include "AST/EnumDefinition.h"
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

#include "AST/Assignment/AddAssign.h"
#include "AST/Assignment/AndAssign.h"
#include "AST/Assignment/AppendAssign.h"
#include "AST/Assignment/Assign.h"
#include "AST/Assignment/DivideAssign.h"
#include "AST/Assignment/ExponentialAssign.h"
#include "AST/Assignment/ModulusAssign.h"
#include "AST/Assignment/MultiplyAssign.h"
#include "AST/Assignment/OrAssign.h"
#include "AST/Assignment/RawAssign.h"
#include "AST/Assignment/ShiftLeftAssign.h"
#include "AST/Assignment/ShiftRightAssign.h"
#include "AST/Assignment/SubtractAssign.h"
#include "AST/Assignment/XorAssign.h"

#include "AST/Definition/AutoDefinition.h"
#include "AST/Definition/ByteDefinition.h"
#include "AST/Definition/CharDefinition.h"
#include "AST/Definition/ClassInstance.h"
#include "AST/Definition/FuncPtrDefinition.h"
#include "AST/Definition/NetDefinition.h"
#include "AST/Definition/NumDefinition.h"
#include "AST/Definition/PinDefinition.h"
#include "AST/Definition/VoidDefinition.h"

#include "AST/Expression/AccessAttribute.h"
#include "AST/Expression/AccessMember.h"
#include "AST/Expression/AccessMemberSafe.h"
#include "AST/Expression/Add.h"
#include "AST/Expression/AndReduce.h"
#include "AST/Expression/Array.h"
#include "AST/Expression/ArrayConcatenate.h"
#include "AST/Expression/BitAnd.h"
#include "AST/Expression/BitNand.h"
#include "AST/Expression/BitNor.h"
#include "AST/Expression/BitNot.h"
#include "AST/Expression/BitOR.h"
#include "AST/Expression/BitXnor.h"
#include "AST/Expression/BitXor.h"
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
#include "AST/Expression/LessEqual.h"
#include "AST/Expression/Literal.h"
#include "AST/Expression/LogicalAnd.h"
#include "AST/Expression/LogicalNot.h"
#include "AST/Expression/LogicalOr.h"
#include "AST/Expression/Modulus.h"
#include "AST/Expression/Multiply.h"
#include "AST/Expression/NandReduce.h"
#include "AST/Expression/Negate.h"
#include "AST/Expression/NorReduce.h"
#include "AST/Expression/NotEqual.h"
#include "AST/Expression/Object.h"
#include "AST/Expression/OrReduce.h"
#include "AST/Expression/Range.h"
#include "AST/Expression/Raw.h"
#include "AST/Expression/Replicate.h"
#include "AST/Expression/ShiftLeft.h"
#include "AST/Expression/ShiftRight.h"
#include "AST/Expression/Slice.h"
#include "AST/Expression/String.h"
#include "AST/Expression/Subtract.h"
#include "AST/Expression/VectorConcatenate.h"
#include "AST/Expression/XnorReduce.h"
#include "AST/Expression/XorReduce.h"
//------------------------------------------------------------------------------

class Parser{
    private:
        bool error;
        void printError(const char* message);

        Token   token; // The current token
        Scanner scanner;

        void getToken();

        AST::Assignment* attributeAssignment();
        AST::Assignment* attributeList      ();

        AST::Expression* string    ();
        AST::Expression* literal   ();
        AST::Expression* identifier();

        bool             expressionList (std::vector<AST::Expression*>& list);
        AST::Base*       parameter      ();
        bool             parameterList  (std::list<AST::Base*>& parameters);
        AST::Expression* array          ();
        AST::Expression* arrayConcat    ();
        AST::Expression* vectorConcat   ();

        AST::Expression* primary        ();
        AST::Expression* castEpr        (AST::Expression* node);
        AST::Expression* cast           ();
        AST::Expression* postfix        ();
        AST::Expression* unary          ();
        AST::Expression* range          ();
        AST::Expression* reduction      ();
        AST::Expression* stringification();
        AST::Expression* replication    ();
        AST::Expression* exponential    ();
        AST::Expression* multiplicative ();
        AST::Expression* additive       ();
        AST::Expression* shift          ();
        AST::Expression* relational     ();
        AST::Expression* equality       ();
        AST::Expression* bitwiseAND     ();
        AST::Expression* bitwiseXOR     ();
        AST::Expression* bitwiseOR      ();
        AST::Expression* expression     (); // AKA conditional
        AST::Expression* typeIdentifier ();

        AST::Assignment            * initialiser    (std::string& identifier);
        AST::Definition::Array     * arrayDefinition();
        AST::Definition::Identifier* identifierList ();

        // Define function parameters
        AST::Definition* defParameter    ();
        AST::Definition* defParameterList();

        bool validNamespaceSpecifier(AST::Expression* node); // Used by other()
        bool validTypeSpecifier     (AST::Expression* node); // Used by other()
        bool validLHS               (AST::Expression* node); // Used by other()

        AST::ClassDefinition * classDefinition();
        AST::EnumDefinition  * enumDefinition();
        AST::IfStatement     * ifStatement();
        AST::Definition      * definition();
        AST::WhileLoop       * whileLoop();
        AST::LoopLoop        * loopLoop();
        AST::ForLoop         * forLoop();
        AST::Import          * import();
        AST::Switch          * switchStatement();
        AST::Alias           * alias();
        AST::Group           * group();
        AST::Base            * other();
        AST::Jump            * jump();
        AST::Rtl             * rtl();
        AST::Fsm             * fsm();
        AST::Hdl             * hdl();

        AST::Base* statement     ();
        AST::Base* statements    ();
        AST::Base* statementBlock();

    public:
        Parser();
       ~Parser();

        AST::Base* run(const char* filename);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
