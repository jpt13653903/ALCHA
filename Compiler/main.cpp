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

 mpq_t MyFraction;
 mpq_init(MyFraction);
 mpq_set_ui(MyFraction, 57, 912);
 mpq_canonicalize(MyFraction);
 char* String = mpq_get_str(0, 10, MyFraction);
 printf("MyFraction = %s\n", String);
 free(String);
 mpq_clear(MyFraction);
 return 0;

 PARSER Parser;
// if(!Parser.Run("../../Test Cases\\FrontEnd\\Scanner.alc")) return -1;
// if(!Parser.Run("../../Test Cases\\Target\\Cyclone V GX Starter Kit.ah")){
//  return -1;
// }
// if(!Parser.Run("../../Test Cases\\Target\\BeMicro Max 10.ah")){
//  return -1;
// }
 if(!Parser.Run("../../Test Cases\\Target\\Minimal BeMicro Max 10.ah")){
  return -1;
 }
// if(!Parser.Run("../../Test Cases/Buttons to LEDs/main.alc")){
//  return -1;
// }

// ALTERA Altera;
// if(!Altera.WriteProject("../../Test Cases/Output", "Testing")) return -2;

 return 0;
}
//------------------------------------------------------------------------------
