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

#include "Switch.h"
//------------------------------------------------------------------------------

using namespace AST;
//------------------------------------------------------------------------------

SWITCH::CASE::CASE(){
  Next        = 0;
  Statements  = 0;
  Expressions = 0;
}
//------------------------------------------------------------------------------

SWITCH::CASE::~CASE(){
  if(Next       ) delete Next;
  if(Statements ) delete Statements;
  if(Expressions) delete Expressions;
}
//------------------------------------------------------------------------------

SWITCH::SWITCH(int Line, const char* Filename):
BASE(Line, Filename, TYPE::Switch){
  Cases      = 0;
  Default    = 0;
  Expression = 0;
}
//------------------------------------------------------------------------------

SWITCH::~SWITCH(){
  if(Cases     ) delete Cases;
  if(Default   ) delete Default;
  if(Expression) delete Expression;
}
//------------------------------------------------------------------------------

void SWITCH::Display(){
  printf("\n%s:%d -- switch(", Filename.c_str(), Line);
    if(Expression) Expression->Display();
  printf("){\n");
    CASE* Temp = Cases;
    while(Temp){
      printf(" case(");
        if(Temp->Expressions) Temp->Expressions->Display();
      printf("){\n");
        if(Temp->Statements) Temp->Statements->Display();
      printf(" }\n");
      Temp = Temp->Next;
    }
    printf(" default{\n");
      if(Default) Default->Display();
    printf(" }\n");
  printf("}\n");

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------
