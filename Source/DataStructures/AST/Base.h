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

#ifndef AST_Base_h
#define AST_Base_h
//------------------------------------------------------------------------------

/* Description:
 *
 * This is the base class of the abstract syntax tree.  It is used in two 
 * major data structures: the output of the parser and output of the engine.
 *
 * These two structures are essentially independent, but have a significant 
 * commonality, as is therefore based on the same C++ class hierarchy.
------------------------------------------------------------------------------*/

#include <string>
//------------------------------------------------------------------------------

#include "Logger.h"
#include "General.h"
#include "Utilities.h"
//------------------------------------------------------------------------------

namespace AST{
  class BASE{ // The base type for AST nodes
    public:
      void Error  (const char* Message = 0);
      void Warning(const char* Message = 0);
    //--------------------------------------------------------------------------

    public:
      enum class TYPE{
        Fence, // Empty statement, but also "next-cycle" specifier in FSMs
        Import,
        Group,
        Alias,

        Parameter,  // Parameter definition, not function call

        NamespacePush,
        IfStatement,
        ForLoop,
        LoopLoop,
        WhileLoop,
        Switch,
        Jump,
        RTL,
        FSM,
        HDL,

        // Definition
          Void_Definition,
          Auto_Definition,
          Pin_Definition,
          Net_Definition,
          Byte_Definition,
          Char_Definition,
          Num_Definition,
          FuncPtr_Definition,
          Enum_Definition,
          Class_Definition,
          Class_Instance,

        // Assignment
          Assign,             //   =
          Raw_Assign,         //  :=
          Append_Assign,      //  ~=
          Add_Assign,         //  +=
          Subtract_Assign,    //  -=
          Multiply_Assign,    //  *=
          Divide_Assign,      //  /=
          Modulus_Assign,     //  %=
          Exponential_Assign, // **=
          AND_Assign,         //  &=
          OR_Assign,          //  |=
          XOR_Assign,         //  ^=
          Shift_Left_Assign,  // <<=
          Shift_Right_Assign, // >>=

        // Expression
          String,
          Literal,
          Array,
          Identifier,
          Object,

          VectorConcatenate,
          ArrayConcatenate,

          FunctionCall, // Left is the function name; right is the parameter list
          Slice,
          AccessMember,
          AccessMemberSafe,
          AccessAttribute,
          Range,

          Increment, // If child is on the left, post-increment
          Decrement, // If child is on the left, post-decrement
          Factorial,

          Negate,
          Bit_NOT,
          Raw,     // Unary operator to cast to "raw bits", or "unsigned int"

          AND_Reduce,
          NAND_Reduce,
          OR_Reduce,
          NOR_Reduce,
          XOR_Reduce,
          XNOR_Reduce,
          Logical_NOT,

          Cast,

          Replicate,

          Multiply,
          Divide,
          Modulus,
          Exponential,

          Add,
          Subtract,

          Shift_Left,
          Shift_Right,

          Less,
          Greater,
          Less_Equal,
          Greater_Equal,

          Equal,
          Not_Equal,

          Bit_AND,
          Bit_NAND,
          Bit_OR,
          Bit_NOR,
          Bit_XOR,
          Bit_XNOR,

          Logical_AND,
          Logical_OR,

          Conditional
      } Type;

      virtual bool IsAssignment();
      virtual bool IsDefinition();
      virtual bool IsExpression();
    //--------------------------------------------------------------------------

    public:
      struct{
        int         Line;
        std::string Filename;
      } Source;
      BASE* Next; // Next instruction
      BASE* Prev; // Previous instruction
    //--------------------------------------------------------------------------

    protected:
      static BASE* CopyList(BASE* Source);
    //--------------------------------------------------------------------------

    public:
               BASE(int Line, const char* Filename, TYPE Type);
      virtual ~BASE(); // Also deletes the rest of the linked list

      // Returns a copy of this instance
      virtual BASE* Copy() = 0;

      // Runs through the AST:
      // - Builds the namespaces
      // - Creates object instances (including temporary wires)
      // - Evaluates scripting expressions
      // - Runs import statements
      virtual bool RunAST() = 0;

      // Appends the Verilog code of this node to Body
      virtual bool GetVerilog(std::string& Body) = 0;

              void DisplayInfo();
      virtual void Display() = 0;

      // Runs assertions on the Next and Prev pointers, and then validates the
      // members.  Run this function on the head of the list.
      void         Validate();
      virtual void ValidateMembers() = 0;
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

