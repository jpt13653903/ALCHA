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

 PREPROCESSOR Processor;

 if(!Processor.Open("../../Test Cases\\FrontEnd\\Scanner.alc")) return -1;

 STRING s;
 KEYWORD Keywords;
 PREPROCESSOR::TOKEN Token;

 while(Processor.GetToken(&Token)){
  printf("%05d: ", Token.Line);
  switch(Token.Type){
   case PREPROCESSOR::tIdentifier:
    printf("Identifier:  \"%s\"", Token.String.String());
    break;

   case PREPROCESSOR::tKeyword:
    Keywords.GetName(Token.Keyword, s);
    printf("Keyword %2d:  \"%s\"", Token.Keyword, s.String());
    break;

   case PREPROCESSOR::tOperator:
    Operators.GetName(Token.Operator, s);
    printf("Operator %2d: \"%s\"", Token.Operator, s.String());
    break;

   case PREPROCESSOR::tString:
    printf("String:      \"%s\"", Token.String.String());
    break;

   case PREPROCESSOR::tFixedPoint:
    printf("Fixed-point");
    break;

   case PREPROCESSOR::tFloat:
    printf("Float:       %lg", (double)Token.Float);
    break;

   case PREPROCESSOR::tEOF:
    printf("End of file\n");
    break;

   default:
    printf("Unknown:     \"%s\"", Token.String.String());
    break;
  }
  if(Token.Comment.Length()){
   printf(" Associated comment: %s", Token.Comment.String());
  }
  printf("\n");
 }

 return 0;
}
//------------------------------------------------------------------------------
