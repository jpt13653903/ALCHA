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

#include "AST_Assignment.h"
//------------------------------------------------------------------------------

AST_Assignment::AST_Assignment(int Line, ASSIGNMENT_TYPE AssignmentType):
AST_Base(Line){
 this->Type           = Assignment;
 this->AssignmentType = AssignmentType;

 Left = Right = 0;
}
//------------------------------------------------------------------------------

AST_Assignment::~AST_Assignment(){
 if(Left ) delete Left;
 if(Right) delete Right;
}
//------------------------------------------------------------------------------

void AST_Assignment::Display(){
 printf("\nLine %d -- Assignment: ", Line);

 if(Left){
  if(Left->Left || Left->Right) printf("(");
  Left->Display();
  if(Left->Left || Left->Right) printf(")");
 }

 switch(AssignmentType){
  case Assign            : printf(  " = "); break;
  case Raw_Assign        : printf( " := "); break;
  case Append_Assign     : printf( " @= "); break;
  case Add_Assign        : printf( " += "); break;
  case Subtract_Assign   : printf( " -= "); break;
  case Multiply_Assign   : printf( " *= "); break;
  case Divide_Assign     : printf( " /= "); break;
  case Modulus_Assign    : printf(" %%= "); break;
  case AND_Assign        : printf( " &= "); break;
  case OR_Assign         : printf( " |= "); break;
  case XOR_Assign        : printf( " ^= "); break;
  case Shift_Left_Assign : printf(" <<= "); break;
  case Shift_Right_Assign: printf(" >>= "); break;

  default: printf("(Unknown assignment type)");
 }

 if(Right){
  if(Right->Left || Right->Right) printf("(");
  Right->Display();
  if(Right->Left || Right->Right) printf(")");
 }

 printf("\n");
 if(Next) Next->Display();
}
//------------------------------------------------------------------------------
