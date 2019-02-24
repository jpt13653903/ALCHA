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

#include "Synthesisable.h"
//------------------------------------------------------------------------------

using namespace OBJECTS;
//------------------------------------------------------------------------------

SYNTHESISABLE::SYNTHESISABLE(const char* Name, TYPE Type) : BASE(Name, Type){
  Signed    = false;
  Width     = 1;
  FullScale = 2;
}
//------------------------------------------------------------------------------

SYNTHESISABLE::~SYNTHESISABLE(){
}
//------------------------------------------------------------------------------

void SYNTHESISABLE::Display(){
  printf("  %s\n", Name.c_str());
  printf("    Width      = %u\n", Width);
  printf("    Full-scale = "); FullScale.Display(); printf("\n");
  printf("    Signed     = %s\n", Signed ? "true" : "false");
}
//------------------------------------------------------------------------------

