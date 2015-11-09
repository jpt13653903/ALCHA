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

bool PREPROCESSOR::TranslateHex(NUMBER& Number){
 const char* Buffer = ppToken.Token.String() + 2;

 Number = 0;

 while(*Buffer){
  if(*Buffer == '.') break;
  if(*Buffer == 'p' || *Buffer == 'P') break;

  if      (*Buffer >=  '0' && *Buffer <= '9'){
   Number *= 0x10;
   Number.AddToNumerator(*Buffer - '0');
  }else if(*Buffer >=  'a' && *Buffer <= 'f'){
   Number *= 0x10;
   Number.AddToNumerator(*Buffer - 'a' + 0xA);
  }else if(*Buffer >=  'A' && *Buffer <= 'F'){
   Number *= 0x10;
   Number.AddToNumerator(*Buffer - 'A' + 0xA);
  }else{
   Error("Invalid number format");
   return false;
  }
  Buffer++;
 }

 if(*Buffer == '.'){
  Buffer++;
  while(*Buffer){
   if(*Buffer == 'p' || *Buffer == 'P') break;

   if      (*Buffer >=  '0' && *Buffer <= '9'){
    Number *= 0x10;
    Number /= 0x10;
    Number.AddToNumerator(*Buffer - '0');
   }else if(*Buffer >=  'a' && *Buffer <= 'f'){
    Number *= 0x10;
    Number /= 0x10;
    Number.AddToNumerator(*Buffer - 'a' + 0xA);
   }else if(*Buffer >=  'A' && *Buffer <= 'F'){
    Number *= 0x10;
    Number /= 0x10;
    Number.AddToNumerator(*Buffer - 'A' + 0xA);
   }else{
    Error("Invalid number format");
    return false;
   }
   Buffer++;
  }
 }

 bool NegExponent = false;
 int  Exponent    = 0;
 if(*Buffer == 'p' || *Buffer == 'P'){
  Buffer++;
  if(*Buffer == '+'){
   Buffer++;
  }else if(*Buffer == '-'){
   NegExponent = true;
   Buffer++;
  }
  while(*Buffer){
   if(*Buffer <  '0' || *Buffer >  '9'){
    Error("Invalid number format");
    return false;
   }
   Exponent = 10*Exponent + (*Buffer - '0');
   Buffer++;
  }
 }
 if(NegExponent) Exponent *= -1;

 while(Exponent > 0){
  Number *= 2;
  Exponent--;
 }
 while(Exponent < 0){
  Number /= 2;
  Exponent++;
 }
 return true;
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::TranslateOctal(NUMBER& Number){
 const char* Buffer = ppToken.Token.String() + 2;

 Number = 0;

 while(*Buffer){
  if(*Buffer == '.') break;
  if(*Buffer == 'p' || *Buffer == 'P') break;
  if(*Buffer <  '0' || *Buffer >  '7'){
   Error("Invalid number format");
   return false;
  }
  Number *= 8;
  Number.AddToNumerator(*Buffer - '0');
  Buffer++;
 }

 if(*Buffer == '.'){
  Buffer++;
  while(*Buffer){
   if(*Buffer == 'p' || *Buffer == 'P') break;
   if(*Buffer <  '0' || *Buffer >  '7'){
    Error("Invalid number format");
    return false;
   }
   Number *= 8; // Increases numerator
   Number /= 8; // Increases denominator
   Number.AddToNumerator(*Buffer - '0');
   Buffer++;
  }
 }

 bool NegExponent = false;
 int  Exponent    = 0;
 if(*Buffer == 'p' || *Buffer == 'P'){
  Buffer++;
  if(*Buffer == '+'){
   Buffer++;
  }else if(*Buffer == '-'){
   NegExponent = true;
   Buffer++;
  }
  while(*Buffer){
   if(*Buffer <  '0' || *Buffer >  '9'){
    Error("Invalid number format");
    return false;
   }
   Exponent = 10*Exponent + (*Buffer - '0');
   Buffer++;
  }
 }
 if(NegExponent) Exponent *= -1;

 while(Exponent > 0){
  Number *= 2;
  Exponent--;
 }
 while(Exponent < 0){
  Number /= 2;
  Exponent++;
 }
 return true;
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::TranslateBinary(NUMBER& Number){
 const char* Buffer = ppToken.Token.String() + 2;

 Number = 0;

 while(*Buffer){
  if(*Buffer == '.') break;
  if(*Buffer == 'p' || *Buffer == 'P') break;
  if(*Buffer <  '0' || *Buffer >  '1'){
   Error("Invalid number format");
   return false;
  }
  Number *= 2;
  Number.AddToNumerator(*Buffer - '0');
  Buffer++;
 }

 if(*Buffer == '.'){
  Buffer++;
  while(*Buffer){
   if(*Buffer == 'p' || *Buffer == 'P') break;
   if(*Buffer <  '0' || *Buffer >  '1'){
    Error("Invalid number format");
    return false;
   }
   Number *= 2; // Increases numerator
   Number /= 2; // Increases denominator
   Number.AddToNumerator(*Buffer - '0');
   Buffer++;
  }
 }

 bool NegExponent = false;
 int  Exponent    = 0;
 if(*Buffer == 'p' || *Buffer == 'P'){
  Buffer++;
  if(*Buffer == '+'){
   Buffer++;
  }else if(*Buffer == '-'){
   NegExponent = true;
   Buffer++;
  }
  while(*Buffer){
   if(*Buffer <  '0' || *Buffer >  '9'){
    Error("Invalid number format");
    return false;
   }
   Exponent = 10*Exponent + (*Buffer - '0');
   Buffer++;
  }
 }
 if(NegExponent) Exponent *= -1;

 while(Exponent > 0){
  Number *= 2;
  Exponent--;
 }
 while(Exponent < 0){
  Number /= 2;
  Exponent++;
 }
 return true;
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::TranslateDecimal(NUMBER& Number){
 const char* Buffer = ppToken.Token.String();

 Number = 0;

 while(*Buffer){
  if(*Buffer == '.') break;
  if(*Buffer == 'e' || *Buffer == 'E') break;
  if(*Buffer <  '0' || *Buffer >  '9'){
   Error("Invalid number format");
   return false;
  }
  Number *= 10;
  Number.AddToNumerator(*Buffer - '0');
  Buffer++;
 }

 if(*Buffer == '.'){
  Buffer++;
  while(*Buffer){
   if(*Buffer == 'e' || *Buffer == 'E') break;
   if(*Buffer <  '0' || *Buffer >  '9'){
    Error("Invalid number format");
    return false;
   }
   Number *= 10; // Increases numerator
   Number /= 10; // Increases denominator
   Number.AddToNumerator(*Buffer - '0');
   Buffer++;
  }
 }

 bool NegExponent = false;
 int  Exponent    = 0;
 if(*Buffer == 'e' || *Buffer == 'E'){
  Buffer++;
  if(*Buffer == '+'){
   Buffer++;
  }else if(*Buffer == '-'){
   NegExponent = true;
   Buffer++;
  }
  while(*Buffer){
   if(*Buffer <  '0' || *Buffer >  '9'){
    Error("Invalid number format");
    return false;
   }
   Exponent = 10*Exponent + (*Buffer - '0');
   Buffer++;
  }
 }
 if(NegExponent) Exponent *= -1;

 while(Exponent > 0){
  Number *= 10;
  Exponent--;
 }
 while(Exponent < 0){
  Number /= 10;
  Exponent++;
 }
 return true;
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::TranslateNumber(NUMBER& Number){
 const char* Buffer = ppToken.Token.String();

 if(Buffer[0] == '0'){
  switch(Buffer[1]){
   case 'x': return TranslateHex    (Number);
   case 'o': return TranslateOctal  (Number);
   case 'b': return TranslateBinary (Number);
   default : return TranslateDecimal(Number);
  }
 }
 return TranslateDecimal(Number);
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::TranslateFixedPointCast(TOKEN* Token){
 const char* Buffer = ppToken.Token.String();

 int  Integer  = 0;
 int  Fraction = 0;
 bool FractionSign = false;

 if(*Buffer != '`') return false;
 Buffer++;
 while(*Buffer){
  if(*Buffer == '.') break;
  if(*Buffer < '0' || *Buffer > '9'){
   Error("Invalid fixed-point cast");
   return false;
  }
  Integer = 10*Integer + (*Buffer - '0');
  Buffer++;
 }

 if(*Buffer == '.'){
  Buffer++;
  if(*Buffer == '-'){
   FractionSign = true;
   Buffer++;
  }
  while(*Buffer){
   if(*Buffer < '0' || *Buffer > '9'){
    Error("Invalid fixed-point cast");
    return false;
   }
   Fraction = 10*Fraction + (*Buffer - '0');
   Buffer++;
  }
  if(FractionSign) Fraction *= -1;
 }
 Token->FixedPointFormat.IntegerBits  = Integer;
 Token->FixedPointFormat.FractionBits = Fraction;

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
   if(!TranslateNumber(Token->Number)) break;
   Token->Type  = tNumber;
   Scanner->GetToken(&ppToken);
   return true;
  }

  if(ppToken.Type == SCANNER::tFixedPointCast){
   if(!TranslateFixedPointCast(Token)) break;
   Token->Type  = tFixedPointCast;
   Scanner->GetToken(&ppToken);
   return true;
  }

  if(ppToken.Type == SCANNER::tCharacter){
   unsigned CodeLength;
   if(!TranslateEscapes()) break;
   Token->Number = ppToken.Token.GetUTF_32(0, &CodeLength);
   if(CodeLength < ppToken.Token.Length()){
    Error("Character too long");
    break;
   }
   Token->Type = tNumber;
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
