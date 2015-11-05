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
  SetConsoleOutputCP(CP_UTF8); // UTF-8 console
 #endif

 SCANNER Scanner;

 if(!Scanner.Open("../../Test Cases\\FrontEnd\\Scanner.alc")) return -1;

 STRING s;
 SCANNER::CHAR Char;
 while(Scanner.Get(Char)){
  s << (char*)Char.Char;
  printf("%05d: %s\n", Char.Line, Char.Char);
 }
 printf("\n\n%s\n", s.String());

 return 0;
}
//------------------------------------------------------------------------------
