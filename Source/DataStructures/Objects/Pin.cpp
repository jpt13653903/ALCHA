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

#include "Pin.h"
//------------------------------------------------------------------------------

using namespace OBJECTS;
//------------------------------------------------------------------------------

PIN::PIN(const char* Name) : SYNTHESISABLE(Name, TYPE::Pin){
  Driver  = 0;
  Enabled = 0;
}
//------------------------------------------------------------------------------

PIN::~PIN(){
  if(Driver ) delete Driver;
  if(Enabled) delete Enabled;
}
//------------------------------------------------------------------------------

void PIN::Display(){
  SYNTHESISABLE::Display();

  printf("    Direction  = ");
  switch(Direction){
    case AST::DEFINITION::Inferred: printf("Inferred\n"); break;
    case AST::DEFINITION::Input   : printf("Input\n"   ); break;
    case AST::DEFINITION::Output  : printf("Output\n"  ); break;
    default                       : printf("Invalid\n" ); break;
  }
  printf("    Driver     = ");
  if(Driver) Driver->Display();
  else       printf("{open}");
  printf("\n");

  printf("    Enabled    = ");
  if(Enabled) Enabled->Display();
  else        printf("{open}");
  printf("\n");

  DisplayAttributes(4);
}
//------------------------------------------------------------------------------

