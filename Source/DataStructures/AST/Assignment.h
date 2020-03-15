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

#ifndef AST_Assignment_h
#define AST_Assignment_h
//------------------------------------------------------------------------------

#include <list>
//------------------------------------------------------------------------------

#include "Expression/Expression.h"
//------------------------------------------------------------------------------

namespace NETLIST{
  class BASE;
}
//------------------------------------------------------------------------------

namespace AST{
  class ASSIGNMENT: public BASE{
    public:
      enum class ASSIGNMENT_TYPE{
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
        Shift_Right_Assign  // >>=
      } AssignmentType;

      bool Fence; // Assignment terminates in a ";" and not a ","

      // Left and Right operands
      EXPRESSION* Left;
      EXPRESSION* Right;

    private:
      // Populates a list of existing expressions, except when the target is an
      // undefined attribute, in which case the attribute is created first.
      struct TARGET_LIST{
        bool           isAttribute;
        NETLIST::BASE* Object;
        EXPRESSION**   Expression;
        TARGET_LIST(){ isAttribute = false; }
      };
      typedef std::list<TARGET_LIST> target_list;
      bool    GetLHS_Object(NETLIST::BASE* Object, target_list& List, BASE* Ast);
      bool    GetLHS(EXPRESSION* Node, target_list& List);

    public:
      ASSIGNMENT(int             Line,
                 std::string&    Filename,
                 ASSIGNMENT_TYPE AssignmentType);
      ASSIGNMENT(int             Line,
                 const char*     Filename,
                 ASSIGNMENT_TYPE AssignmentType);
     ~ASSIGNMENT();

      // Returns a copy of this instance
      virtual BASE* Copy(bool CopyNext);

      // Runs scripting commands and creates instances in the namespace tree
      virtual bool RunScripting();
      virtual bool GetVerilog(std::string& Body);

      void Display();
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

