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
    case ASSIGNMENT_TYPE::Assign            : Debug.print(  " = "); break;
    case ASSIGNMENT_TYPE::Raw_Assign        : Debug.print( " := "); break;
    case ASSIGNMENT_TYPE::Append_Assign     : Debug.print( " ~= "); break;
    case ASSIGNMENT_TYPE::Add_Assign        : Debug.print( " += "); break;
    case ASSIGNMENT_TYPE::Subtract_Assign   : Debug.print( " -= "); break;
    case ASSIGNMENT_TYPE::Multiply_Assign   : Debug.print( " *= "); break;
    case ASSIGNMENT_TYPE::Divide_Assign     : Debug.print( " /= "); break;
    case ASSIGNMENT_TYPE::Modulus_Assign    : Debug.print(" %%= "); break;
    case ASSIGNMENT_TYPE::Exponential_Assign: Debug.print( "**= "); break;
    case ASSIGNMENT_TYPE::AND_Assign        : Debug.print( " &= "); break;
    case ASSIGNMENT_TYPE::OR_Assign         : Debug.print( " |= "); break;
    case ASSIGNMENT_TYPE::XOR_Assign        : Debug.print( " ^= "); break;
    case ASSIGNMENT_TYPE::Shift_Left_Assign : Debug.print(" <<= "); break;
    case ASSIGNMENT_TYPE::Shift_Right_Assign: Debug.print(" >>= "); break;

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
