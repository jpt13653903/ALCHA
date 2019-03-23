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

#include "Assignment.h"
//------------------------------------------------------------------------------

using namespace AST;
//------------------------------------------------------------------------------

ASSIGNMENT::ASSIGNMENT(
  int             Line,
  const char*     Filename,
  ASSIGNMENT_TYPE AssignmentType
): BASE(Line, Filename, TYPE::Assignment){
  this->AssignmentType = AssignmentType;

  Fence = false;
  Left  = Right = 0;
}
//------------------------------------------------------------------------------

ASSIGNMENT::~ASSIGNMENT(){
  if(Left ) delete Left;
  if(Right) delete Right;
}
//------------------------------------------------------------------------------

void ASSIGNMENT::Display(){
  DisplayInfo();
  Debug.print("Assignment: ");

  if(Left){
    if(Left->Left || Left->Right) Debug.print("(");
    Left->Display();
    if(Left->Left || Left->Right) Debug.print(")");
  }

  switch(AssignmentType){
    case Assign            : Debug.print(  " = "); break;
    case Raw_Assign        : Debug.print( " := "); break;
    case Append_Assign     : Debug.print( " ~= "); break;
    case Add_Assign        : Debug.print( " += "); break;
    case Subtract_Assign   : Debug.print( " -= "); break;
    case Multiply_Assign   : Debug.print( " *= "); break;
    case Divide_Assign     : Debug.print( " /= "); break;
    case Modulus_Assign    : Debug.print(" %%= "); break;
    case Exponential_Assign: Debug.print( "**= "); break;
    case AND_Assign        : Debug.print( " &= "); break;
    case OR_Assign         : Debug.print( " |= "); break;
    case XOR_Assign        : Debug.print( " ^= "); break;
    case Shift_Left_Assign : Debug.print(" <<= "); break;
    case Shift_Right_Assign: Debug.print(" >>= "); break;

    default: Debug.print("(Unknown assignment type)");
  }

  if(Right){
    if(Right->Left || Right->Right) Debug.print("(");
    Right->Display();
    if(Right->Left || Right->Right) Debug.print(")");
  }

  if(Fence) Debug.print("{Fence}");

  Debug.print("\n");
  if(Next) Next->Display();
}
//------------------------------------------------------------------------------
