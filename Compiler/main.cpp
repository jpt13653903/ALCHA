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
 SCANNER::TOKEN* Token;

 Token = Scanner.GetDirective();
 while(Token){
  printf("%05d: ", Token->Line);
  switch(Token->Type){
   case SCANNER::tSpace:
    printf("Space\n");
    break;

   case SCANNER::tNewline:
    printf("Newline\n");
    break;

   case SCANNER::tComment:
    printf("Comment:   \"%s\"\n", Token->Token.String());
    break;

   case SCANNER::tDirective:
    printf("Directive: \"%s\"\n", Token->Token.String());
    break;

   case SCANNER::tIdentifier:
    printf("Identifier: \"%s\"\n", Token->Token.String());
    break;

   case SCANNER::tNumber:
    printf("Number:     \"%s\"\n", Token->Token.String());
    break;

   case SCANNER::tCharacter:
    printf("Character:  \"%s\"\n", Token->Token.String());
    break;

   case SCANNER::tString:
    printf("String:     \"%s\"\n", Token->Token.String());
    break;

   case SCANNER::tPunctuator:
    printf("Punctuator: \"%s\"\n", Token->Token.String());
    break;

   case SCANNER::tOther:
    printf("Other:      \"%s\"\n", Token->Token.String());
    break;

   case SCANNER::tEOF:
    printf("End of file\n");
    break;

   default:
    printf("Unknown:    \"%s\"\n", Token->Token.String());
    break;
  }
  delete Token;
  Token = Scanner.GetDirective();
 }

 return 0;
}
//------------------------------------------------------------------------------
