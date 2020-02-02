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

#include "Jump.h"
//------------------------------------------------------------------------------

using namespace AST;
//------------------------------------------------------------------------------

JUMP::JUMP(int Line, const char* Filename, JUMP_TYPE JumpType):
BASE(Line, Filename, TYPE::Jump){
  this->JumpType = JumpType;

  Expression = 0;
}
//------------------------------------------------------------------------------

JUMP::~JUMP(){
  if(Expression) delete Expression;
}
//------------------------------------------------------------------------------

void JUMP::Display(){
  DisplayInfo();
  Debug.print("jump(");
  switch(JumpType){
    case JUMP_TYPE::Return  : Debug.print("return) "           ); break;
    case JUMP_TYPE::Break   : Debug.print("break) "            ); break;
    case JUMP_TYPE::Continue: Debug.print("continue) "         ); break;
    default                 : Debug.print("Unknown jump type) "); break;
  }
  if(Expression) Expression->Display();
  else           Debug.print("{default}");
  Debug.print("\n");

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------
