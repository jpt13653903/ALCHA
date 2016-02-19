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

#include "AST_Jump.h"
//------------------------------------------------------------------------------

AST_Jump::AST_Jump(int Line, JUMP_TYPE JumpType): AST_Base(Line){
 this->Type     = Jump;
 this->JumpType = JumpType;

 Expression = 0;
}
//------------------------------------------------------------------------------

AST_Jump::~AST_Jump(){
 if(Expression) delete Expression;
}
//------------------------------------------------------------------------------

void AST_Jump::Display(){
 printf("\nLine %d -- jump(", Line);
 switch(JumpType){
  case Return  : printf("return) "           ); break;
  case Break   : printf("break) "            ); break;
  case Continue: printf("continue) "         ); break;
  default      : printf("Unknown jump type) "); break;
 }
 if(Expression) Expression->Display();
 else           printf("{default}");
 printf("\n");

 if(Next) Next->Display();
}
//------------------------------------------------------------------------------
