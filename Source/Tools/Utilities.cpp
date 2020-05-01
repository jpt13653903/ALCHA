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

#include "Utilities.h"
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
  printf(ANSI_FG_MAGENTA "  Warning: " ANSI_RESET);

  if(Message) printf("%s\n", Message);
}
//------------------------------------------------------------------------------

void SimplifyFilename(string& Filename){
  int  LastSlash;
  int  n, c;
  bool Changes;

  do{
    Changes   = false;
    LastSlash = -1;

    for(n = 0; Filename[n]; n++){
      if(Filename[n] == '/'
        #ifdef WINVER
          || Filename[n] == '\\'
        #endif
      ){
        if(
          Filename[n+1] == '.' &&
          Filename[n+2] == '.' && (
            Filename[n+3] == '/'
            #ifdef WINVER
              || Filename[n+3] == '\\'
            #endif
          ) && (
            n > LastSlash+3 ||
            Filename[LastSlash+1] != '.' ||
            Filename[LastSlash+2] != '.'
          )
        ){
          n += 4;
          for(c = LastSlash+1; Filename[n]; c++, n++) Filename[c] = Filename[n];
          Filename[c] = 0;
          Changes = true;
          break;
        }else{
          LastSlash = n;
        }
      }
    }
  }while(Changes);

  Filename.resize(n);
}
//------------------------------------------------------------------------------

void Align(string& Body, int Column){
  Body += ' ';

  int n = Body.length()-1;
  while(n >= 0){
    if(Body[n] == '\n') break;
    n--;
  }

  n = Column - (Body.length() - n);
  while(n > 0){
    Body += ' ';
    n--;
  }
}
//------------------------------------------------------------------------------

