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

#include "Base.h"
//------------------------------------------------------------------------------

using namespace AST;
//------------------------------------------------------------------------------

BASE::BASE(int Line, const char* Filename, TYPE Type){
  this->Type     = Type;
  this->Line     = Line;
  this->Filename = Filename;
  this->Next     = 0;
}
//------------------------------------------------------------------------------

BASE::~BASE(){
  // The list might be too long for a recursive formulation.
  BASE* Temp;
  while(Next){
    Temp       = Next;
    Next       = Temp->Next;
    Temp->Next = 0;
    delete Temp;
  }
}
//------------------------------------------------------------------------------

void BASE::DisplayInfo(){
  Debug.print("\n" ANSI_FG_BRIGHT_BLACK "%s:", Filename.c_str());
  Debug.print(ANSI_FG_CYAN "%05d" ANSI_FG_YELLOW " -- " ANSI_RESET, Line);
}
//------------------------------------------------------------------------------

