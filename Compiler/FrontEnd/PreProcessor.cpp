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

#include "PreProcessor.h"
//------------------------------------------------------------------------------

PREPROCESSOR::FIXED_POINT::FIXED_POINT(){
 BitCount = 0;
 Exponent = 0;
 Bits     = 0;
}
//------------------------------------------------------------------------------

PREPROCESSOR::FIXED_POINT::~FIXED_POINT(){
 if(Bits) delete[] Bits;
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::FIXED_POINT::Init(
 int IntegerBits,
 int FractionBits
){
 if(Bits) delete[] Bits;

 BitCount = IntegerBits;
 if(FractionBits > 0) BitCount += FractionBits;

 Exponent = -FractionBits;

 int ArrayLength;
 if(BitCount & 0x1F) ArrayLength = (BitCount >> 5) + 1;
 else                ArrayLength = (BitCount >> 5);

 Bits = new unsigned[ArrayLength];

 return Bits;
}
//------------------------------------------------------------------------------

PREPROCESSOR::PREPROCESSOR(){
 error   = false;
 Scanner = new SCANNER;
}
//------------------------------------------------------------------------------

PREPROCESSOR::~PREPROCESSOR(){
 delete Scanner;
}
//------------------------------------------------------------------------------

void PREPROCESSOR::Error(const char* Message){
 error = true;
 printf(
  "Line %05d of %s\n  Error: %s",
  ppToken.Line,
  Filename.String(),
  Message
 );
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::Open(const char* Filename){
 if(!Scanner->Open(Filename)) return false;

 error = false;

 this->Filename = Filename;

 Scanner->GetToken(&ppToken);
 return true;
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::TranslateEscapes(){
 int      j, u;
 STRING   Result;
 unsigned UTF_32 = 0;

 for(j = 0; ppToken.Token[j]; j++){
  if(ppToken.Token[j] == '\\'){
   j++;
   switch(ppToken.Token[j]){
    case 'n' : Result << '\n'; break;
    case 't' : Result << '\t'; break;
    case 'v' : Result << '\v'; break;
    case 'b' : Result << '\b'; break;
    case 'r' : Result << '\r'; break;
    case 'f' : Result << '\f'; break;
    case 'a' : Result << '\a'; break;
    case '\\': Result << '\\'; break;
    case '?' : Result << '\?'; break;
    case '\'': Result << '\''; break;
    case '"' : Result << '\"'; break;

    case 'x' : // Hexadecimal number
     j++;
     for(u = 0; u < 8; u++, j++){
      if(ppToken.Token[j] >= '0' && ppToken.Token[j] <= '9'){
       UTF_32 = UTF_32*0x10 + (ppToken.Token[j] - '0');
      }else if(ppToken.Token[j] >= 'a' && ppToken.Token[j] <='f'){
       UTF_32 = UTF_32*0x10 + (ppToken.Token[j] - 'a' + 0xA);
      }else if(ppToken.Token[j] >= 'A' && ppToken.Token[j] <='F'){
       UTF_32 = UTF_32*0x10 + (ppToken.Token[j] - 'A' + 0xA);
      }else{
       break;
      }
     }
     if(!u){
      Error("Incomplete \\x code");
      return false;
     }
     j--;
     Result.Append_UTF_32(UTF_32);
     break;

    case 'u' : // 16-bit Unicode
     j++;
     for(u = 0; u < 4; u++, j++){
      if(ppToken.Token[j] >= '0' && ppToken.Token[j] <= '9'){
       UTF_32 = UTF_32*0x10 + (ppToken.Token[j] - '0');
      }else if(ppToken.Token[j] >= 'a' && ppToken.Token[j] <='f'){
       UTF_32 = UTF_32*0x10 + (ppToken.Token[j] - 'a' + 0xA);
      }else if(ppToken.Token[j] >= 'A' && ppToken.Token[j] <='F'){
       UTF_32 = UTF_32*0x10 + (ppToken.Token[j] - 'A' + 0xA);
      }else{
       Error("Incomplete \\u code");
       return false;
      }
     }
     j--;
     Result.Append_UTF_32(UTF_32);
     break;

    case 'U' : // 32-bit Unicode
     j++;
     for(u = 0; u < 8; u++, j++){
      if(ppToken.Token[j] >= '0' && ppToken.Token[j] <= '9'){
       UTF_32 = UTF_32*0x10 + (ppToken.Token[j] - '0');
      }else if(ppToken.Token[j] >= 'a' && ppToken.Token[j] <='f'){
       UTF_32 = UTF_32*0x10 + (ppToken.Token[j] - 'a' + 0xA);
      }else if(ppToken.Token[j] >= 'A' && ppToken.Token[j] <='F'){
       UTF_32 = UTF_32*0x10 + (ppToken.Token[j] - 'A' + 0xA);
      }else{
       Error("Incomplete \\U code");
       return false;
      }
     }
     j--;
     Result.Append_UTF_32(UTF_32);
     break;

    default: // Could be an octal number...
     for(u = 0; u < 11; u++, j++){
      if(ppToken.Token[j] >= '0' && ppToken.Token[j] <= '7'){
       UTF_32 = UTF_32*8 + (ppToken.Token[j] - '0');
      }else{
       break;
      }
     }
     if(!u){
      Error("Unknown escape sequence");
      return false;
     }
     j--;
     Result.Append_UTF_32(UTF_32);
     break;
   }
  }else{
   Result << ppToken.Token[j];
  }
 }
 ppToken.Token = Result;
 return true;
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::GetToken(TOKEN* Token){
 Token->String .Clear();
 Token->Comment.Clear();
 Token->Type = tEOF;

 while(!error && ppToken.Type != SCANNER::tEOF){
  Token->Line    =  ppToken.Line;
  Token->Comment << ppToken.Comment;

  if(ppToken.Type == SCANNER::tNewline){
   Scanner->GetToken(&ppToken);
   continue;
  }

  if(ppToken.Type == SCANNER::tDirective){
   /// @todo Run directives (and macro expansion) in a Get_ppToken() function
   while(Scanner->GetToken(&ppToken)){ // Just ignore them for now...
    if(ppToken.Type == SCANNER::tNewline){
     Scanner->GetToken(&ppToken);
     break;
    }
   }
   continue;
  }

  if(ppToken.Type == SCANNER::tString){
   if(!TranslateEscapes()) break;
   Token->Type   =  tString;
   Token->String << ppToken.Token;
   Scanner->GetToken(&ppToken);
   continue;
  }
  if(Token->Type == tString) return true;

  if(ppToken.Type == SCANNER::tIdentifier){
   Token->Keyword = Keywords.GetCode(ppToken.Token.String());
   if(Token->Keyword){
    Token->Type = tKeyword;
   }else{
    Token->Type   = tIdentifier;
    Token->String = ppToken.Token;
   }
   Scanner->GetToken(&ppToken);
   return true;
  }

  if(ppToken.Type == SCANNER::tNumber){
   /// @todo Scan numbers properly
   /// @todo Remember to look for operator oFixedPointCast so that the correct
   ///       fixed-point format is used.
   Token->Type  = tFloat;
   Token->Float = atof(ppToken.Token.String());
   Scanner->GetToken(&ppToken);
   return true;
  }

  if(ppToken.Type == SCANNER::tFixedPointCast){
   /// @todo (convert properly: to Token->FixedPoint)
   Token->Type  = tFixedPointCast;
   Token->Float = atof(ppToken.Token.String()+1);
   Scanner->GetToken(&ppToken);
   return true;
  }

  if(ppToken.Type == SCANNER::tCharacter){
   unsigned CodeLength;
   if(!TranslateEscapes()) break;
   Token->FixedPoint.Init(32, 0);
   Token->FixedPoint.Bits[0] = ppToken.Token.GetUTF_32(0, &CodeLength);
   if(CodeLength < ppToken.Token.Length()){
    Error("Character too long");
    break;
   }
   Token->Type = tFixedPoint;
   Scanner->GetToken(&ppToken);
   return true;
  }

  if(ppToken.Type == SCANNER::tOperator){
   Token->Operator = Operators.GetCode(ppToken.Token.String());
   if(Token->Operator){
    Token->Type = tOperator;
    Scanner->GetToken(&ppToken);
    return true;
   }
   Error("Unknown operator");
   break;
  }
  if(ppToken.Type == SCANNER::tOther){
   Error("Invalid token");
   break;
  }
 }
 Token->Type = tEOF;
 return false;
}
//------------------------------------------------------------------------------
