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

#include "main.h"
//------------------------------------------------------------------------------

int main(int argc, char** argv){
  #ifdef __WIN32
    // UTF-8 Console encoding
    SetConsoleOutputCP(CP_UTF8); // UTF-8 console
    // Read the current console size
    CONSOLE_SCREEN_BUFFER_INFO Info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
    // Make the console buffer as many lines as the system will allow
    Info.dwSize.Y = 0x7FFF;
    while(
      !SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), Info.dwSize)
    ) Info.dwSize.Y--;
  #endif

  SetCurrentDirectory(
//  L"C:\\JPT\\Projects\\15\\1509 - ALCHA\\Test Cases\\FrontEnd"
    L"C:\\JPT\\Projects\\15\\1509 - ALCHA\\Test Cases\\Scripting"
  );

  PARSER Parser;
// AST = Parser.Run((byte*)"Parser.alc");
  AST = Parser.Run((byte*)"Basic.alc");
  if(!AST) return -1;

// ALTERA Altera;
// if(!Altera.WriteProject()) return -2;

  if(AST) delete AST;
  return 0;
}
//------------------------------------------------------------------------------
