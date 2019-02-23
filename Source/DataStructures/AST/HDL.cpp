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

#include "HDL.h"
//------------------------------------------------------------------------------

using namespace AST;
//------------------------------------------------------------------------------

HDL::HDL(int Line, const char* Filename): BASE(Line, Filename){
  this->Type = TYPE::HDL;

  Files      = 0;
  Ports      = 0;
  Parameters = 0;
}
//------------------------------------------------------------------------------

HDL::~HDL(){
  if(Files     ) delete Files;
  if(Ports     ) delete Ports;
  if(Parameters) delete Parameters;
}
//------------------------------------------------------------------------------

void HDL::Display(){
  printf("\n%s:%d -- hdl (%s):\n", Filename.c_str(), Line, Identifier.c_str());
  printf(" Files:\n  "     ); if(Files     ) Files     ->Display(); printf("\n");
  printf(" Parameters:\n  "); if(Parameters) Parameters->Display();
  printf(" Ports:\n  "     ); if(Ports     ) Ports     ->Display();

  if(Next) Next->Display();
}
//------------------------------------------------------------------------------
