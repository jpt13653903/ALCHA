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

#ifndef Assignment_h
#define Assignment_h
//------------------------------------------------------------------------------

#include "Expression.h"
//------------------------------------------------------------------------------

namespace AST{
  struct ASSIGNMENT: public BASE{
      enum ASSIGNMENT_TYPE{
        Assign,             //   =
        Raw_Assign,         //  :=
        Append_Assign,      //  @=
        Add_Assign,         //  +=
        Subtract_Assign,    //  -=
        Multiply_Assign,    //  *=
        Divide_Assign,      //  /=
        Modulus_Assign,     //  %=
        Exponential_Assign, //  ^=
        AND_Assign,         //  &=
        OR_Assign,          //  |=
        XOR_Assign,         //  #=
        Shift_Left_Assign,  // <<=
        Shift_Right_Assign  // >>=
      } AssignmentType;

      bool Fence; // Assignment terminates in a ";" and not a ","

      // Left and Right operands
      EXPRESSION* Left;
      EXPRESSION* Right;

      ASSIGNMENT(int             Line,
                 const char*     Filename,
                 ASSIGNMENT_TYPE AssignmentType);
     ~ASSIGNMENT();

      void Display();
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

