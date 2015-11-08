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

#include "Parser.h"
//------------------------------------------------------------------------------

PARSER::PARSER(){
}
//------------------------------------------------------------------------------

PARSER::~PARSER(){
}
//------------------------------------------------------------------------------

#include <math.h>
void DisplayFixedPoint(PREPROCESSOR::FIXED_POINT& FP){
 long double d = 0;
 int j;
 long double p = powl(2.0, FP.Exponent);
 for(j = 0; 32*j < FP.BitCount; j++){
  d += FP.Bits[j] * p;
  p *= 0x1p32;
 }

 int f;
 if(FP.Exponent < 0) f = ceil(log10(powl(2.0, -FP.Exponent)))+2;
 else                f = 0;

 char format[0x100];
 sprintf(format, "%%.%dlf `%%d.%%d", f);

 char s[0x100];
 sprintf(
  s,
  format,
  (double)d,
  FP.BitCount + (FP.Exponent < 0 ? FP.Exponent : 0),
  -FP.Exponent
 );

 int dp;
 for(dp = 0; s[dp] && s[dp] != '.' && s[dp] != ' '; dp++);

 for(j = 0; s[j]; j++){
  if(s[j] == ' '){
   printf("%s", s+j);
   break;
  }

  if(j && j < dp){
   if((dp - j) % 3 == 0) printf("_");
  }else if(j > dp+1){
   if((j - dp - 1) % 3 == 0) printf("_");
  }
  printf("%c", s[j]);
 }
}
//------------------------------------------------------------------------------

bool PARSER::Run(const char* Filename){
 if(!Preprocessor.Open(Filename)) return false;

 STRING s;
 KEYWORDS Keywords;
 PREPROCESSOR::TOKEN Token;

 int Lines = 0;
 while(Preprocessor.GetToken(&Token)){
  Lines++;
  if(Lines > 600) break;

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
    printf("Fixed-point: "); DisplayFixedPoint(Token.FixedPoint);
    break;

   case PREPROCESSOR::tFixedPointCast:
    printf("Fixed-point cast: %lg", (double)Token.Float);
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
 return true;
}
//------------------------------------------------------------------------------
