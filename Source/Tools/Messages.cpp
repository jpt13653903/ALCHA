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

#include "Messages.h"
//------------------------------------------------------------------------------

using namespace std;
//------------------------------------------------------------------------------

bool error = false;
//------------------------------------------------------------------------------

static void Header(int Line, const std::string& Filename){
  if(Line) printf(ANSI_FG_BRIGHT_BLACK "Line "
                  ANSI_FG_CYAN         "%05d "
                  ANSI_FG_BRIGHT_BLACK "of "
                  ANSI_FG_YELLOW       "%s\n"
                  ANSI_RESET,
                  Line,
                  Filename.c_str());
  else     printf(ANSI_FG_BRIGHT_BLACK "Global\n");
}
//------------------------------------------------------------------------------

void Error(int Line, const std::string& Filename, const char* Message){
  if(error) return;
  error = true;

  Header(Line, Filename);
  printf(ANSI_FG_BRIGHT_RED "  Error: " ANSI_RESET);

  if(Message) printf("%s\n", Message);
}
//------------------------------------------------------------------------------

void Warning(int Line, const std::string& Filename, const char* Message){
  Header(Line, Filename);
  printf( ANSI_FG_MAGENTA "  Warning: " ANSI_RESET);

  if(Message) printf("%s\n", Message);
}
//------------------------------------------------------------------------------

