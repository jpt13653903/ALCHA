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

#include "AST_LoopLoop.h"
//------------------------------------------------------------------------------

AST_LoopLoop::AST_LoopLoop(int Line): AST_Base(Line){
 this->Type = LoopLoop;

 Count      = 0;
 Statements = 0;
}
//------------------------------------------------------------------------------

AST_LoopLoop::~AST_LoopLoop(){
 if(Count     ) delete Count;
 if(Statements) delete Statements;
}
//------------------------------------------------------------------------------

void AST_LoopLoop::Display(){
 printf("\nLine %d -- loop(", Line);
  if(Count) Count->Display();
  else      printf("{inf}");
 printf("){\n");
  if(Statements) Statements->Display();
 printf("}\n");

 if(Next) Next->Display();
}
//------------------------------------------------------------------------------
