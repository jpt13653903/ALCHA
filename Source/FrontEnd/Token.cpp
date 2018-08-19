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

#include "Token.h"
//------------------------------------------------------------------------------

TOKEN::TOKEN(){
  Line = 0;
  ID   = 0;
  Type = Unknown;
}
//------------------------------------------------------------------------------

void TOKEN::Display(){
  printf(ANSI_FG_CYAN "Token:\n" ANSI_RESET);
  printf(ANSI_FG_BRIGHT_BLACK "  Type  = " ANSI_RESET "%d\n", (int)Type);
  printf(ANSI_FG_BRIGHT_BLACK "  Line  = " ANSI_RESET "%d\n", Line);
  printf(ANSI_FG_BRIGHT_BLACK "  ID    = " ANSI_RESET "%s\n", (char*)ID);
  printf(ANSI_FG_BRIGHT_BLACK "  Data  = " ANSI_RESET "%s\n", (char*)Data.String());
  printf(ANSI_FG_BRIGHT_BLACK "  Value = " ANSI_RESET ""); Value.Display(); printf("\n");
}
//------------------------------------------------------------------------------

