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

#include "General.h"
#include "Parser.h"
#include "IdentifierTree.h"
//------------------------------------------------------------------------------

// Function used to set up the Windows terminal
void SetupTerminal(){
  #ifdef WINVER
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING 
      #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif

    HANDLE TerminalHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if(TerminalHandle == INVALID_HANDLE_VALUE){
      error("Invalid terminal handle");
      return;
    }

    // Set output mode to handle ANSI terminal sequences
    DWORD dwMode = 0;
    if(!GetConsoleMode(TerminalHandle, &dwMode)){
      // This is OK, because the terminal in question is
      // probably a MinTTY (or similar)
      return;
    }
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if(!SetConsoleMode(TerminalHandle, dwMode)){
      error("Unable to set terminal mode");
      return;
    }

    // Set the console encoding to UTF-8
    if(!SetConsoleOutputCP(CP_UTF8)){
      error("Unable to set terminal to UTF-8");
      return;
    }

    // Make the console buffer as many lines as the system will allow
    CONSOLE_SCREEN_BUFFER_INFO Info;
    GetConsoleScreenBufferInfo(TerminalHandle, &Info);
    Info.dwSize.Y = 0x7FFF;
    while(!SetConsoleScreenBufferSize(TerminalHandle, Info.dwSize) &&
          GetLastError() == ERROR_INVALID_PARAMETER){
      Info.dwSize.Y--;
    }
  #endif
}
//------------------------------------------------------------------------------

int main(){
  SetupTerminal();

  const char* InputFile = "../Test Cases/FrontEnd/Parser.alc";

  PARSER Parser;
  AST = Parser.Run((byte*)InputFile);
  if(!AST){
    error("Error while parsing \"%s\"", InputFile);
    return -1;
  }
  if(AST) delete AST;

  return 0;
}
//------------------------------------------------------------------------------
