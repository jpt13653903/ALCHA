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

PREPROCESSOR::SCANNER_STACK::SCANNER_STACK(){
 Scanner = new SCANNER;
 Next    = 0;
}
//------------------------------------------------------------------------------

PREPROCESSOR::SCANNER_STACK::~SCANNER_STACK(){
 delete Scanner;
}
//------------------------------------------------------------------------------

PREPROCESSOR::PREPROCESSOR(){
 error         = false;
 Scanner_Stack = 0;
}
//------------------------------------------------------------------------------

PREPROCESSOR::~PREPROCESSOR(){
 SCANNER_STACK* Temp;
 while(Scanner_Stack){
  Temp          = Scanner_Stack;
  Scanner_Stack = Scanner_Stack->Next;
  delete Temp;
 }
}
//------------------------------------------------------------------------------

void PREPROCESSOR::Error(const char* Message){
 error = true;
 printf(
  "Line %05d of %s\n  Error: %s\n",
  ppToken.Line,
  Scanner_Stack->Scanner->Filename.String(),
  Message
 );
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::Open(const char* Filename){
 SCANNER_STACK* Temp;
 while(Scanner_Stack){
  Temp          = Scanner_Stack;
  Scanner_Stack = Scanner_Stack->Next;
  delete Temp;
 }

 Scanner_Stack = new SCANNER_STACK;

 if(!Scanner_Stack->Scanner->Open(Filename)) return false;

 Path.Clear();
 int j, q = 0;
 for(j = 0; Filename[j]; j++){
  if(Filename[j] == '\\' || Filename[j] == '/'){
   for(; q < j; q++) Path << Filename[q];
  }
 }

 error = false;

 GetPPToken();
 return true;
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::TranslateEscapes(STRING& Result){
 int      j, u;
 unsigned UTF_32 = 0;

 Result.Clear();

 for(j = 0; ppToken.Body[j]; j++){
  if(ppToken.Body[j] == '\\'){
   j++;
   switch(ppToken.Body[j]){
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
      if(ppToken.Body[j] >= '0' && ppToken.Body[j] <= '9'){
       UTF_32 = UTF_32*0x10 + (ppToken.Body[j] - '0');
      }else if(ppToken.Body[j] >= 'a' && ppToken.Body[j] <='f'){
       UTF_32 = UTF_32*0x10 + (ppToken.Body[j] - 'a' + 0xA);
      }else if(ppToken.Body[j] >= 'A' && ppToken.Body[j] <='F'){
       UTF_32 = UTF_32*0x10 + (ppToken.Body[j] - 'A' + 0xA);
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
      if(ppToken.Body[j] >= '0' && ppToken.Body[j] <= '9'){
       UTF_32 = UTF_32*0x10 + (ppToken.Body[j] - '0');
      }else if(ppToken.Body[j] >= 'a' && ppToken.Body[j] <='f'){
       UTF_32 = UTF_32*0x10 + (ppToken.Body[j] - 'a' + 0xA);
      }else if(ppToken.Body[j] >= 'A' && ppToken.Body[j] <='F'){
       UTF_32 = UTF_32*0x10 + (ppToken.Body[j] - 'A' + 0xA);
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
      if(ppToken.Body[j] >= '0' && ppToken.Body[j] <= '9'){
       UTF_32 = UTF_32*0x10 + (ppToken.Body[j] - '0');
      }else if(ppToken.Body[j] >= 'a' && ppToken.Body[j] <='f'){
       UTF_32 = UTF_32*0x10 + (ppToken.Body[j] - 'a' + 0xA);
      }else if(ppToken.Body[j] >= 'A' && ppToken.Body[j] <='F'){
       UTF_32 = UTF_32*0x10 + (ppToken.Body[j] - 'A' + 0xA);
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
      if(ppToken.Body[j] >= '0' && ppToken.Body[j] <= '7'){
       UTF_32 = UTF_32*8 + (ppToken.Body[j] - '0');
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
   Result << ppToken.Body[j];
  }
 }
 return true;
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::TranslateHex(NUMBER& Number){
 const char* Buffer = ppToken.Body.String() + 2;

 Number = 0;

 while(*Buffer){
  if(*Buffer == '.') break;
  if(*Buffer == 'p' || *Buffer == 'P') break;

  if      (*Buffer >=  '0' && *Buffer <= '9'){
   Number.MultiplyNumerator(0x10);
   Number.AddToNumerator(*Buffer - '0');
  }else if(*Buffer >=  'a' && *Buffer <= 'f'){
   Number.MultiplyNumerator(0x10);
   Number.AddToNumerator(*Buffer - 'a' + 0xA);
  }else if(*Buffer >=  'A' && *Buffer <= 'F'){
   Number.MultiplyNumerator(0x10);
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
    Number.MultiplyNumerator  (0x10);
    Number.MultiplyDenominator(0x10);
    Number.AddToNumerator(*Buffer - '0');
   }else if(*Buffer >=  'a' && *Buffer <= 'f'){
    Number.MultiplyNumerator  (0x10);
    Number.MultiplyDenominator(0x10);
    Number.AddToNumerator(*Buffer - 'a' + 0xA);
   }else if(*Buffer >=  'A' && *Buffer <= 'F'){
    Number.MultiplyNumerator  (0x10);
    Number.MultiplyDenominator(0x10);
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
  Number.MultiplyNumerator(2);
  Exponent--;
 }
 while(Exponent < 0){
  Number.MultiplyDenominator(2);
  Exponent++;
 }
 return true;
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::TranslateOctal(NUMBER& Number){
 const char* Buffer = ppToken.Body.String() + 2;

 Number = 0;

 while(*Buffer){
  if(*Buffer == '.') break;
  if(*Buffer == 'p' || *Buffer == 'P') break;
  if(*Buffer <  '0' || *Buffer >  '7'){
   Error("Invalid number format");
   return false;
  }
  Number.MultiplyNumerator(8);
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
   Number.MultiplyNumerator  (8);
   Number.MultiplyDenominator(8);
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
  Number.MultiplyNumerator(2);
  Exponent--;
 }
 while(Exponent < 0){
  Number.MultiplyDenominator(2);
  Exponent++;
 }
 return true;
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::TranslateBinary(NUMBER& Number){
 const char* Buffer = ppToken.Body.String() + 2;

 Number = 0;

 while(*Buffer){
  if(*Buffer == '.') break;
  if(*Buffer == 'p' || *Buffer == 'P') break;
  if(*Buffer <  '0' || *Buffer >  '1'){
   Error("Invalid number format");
   return false;
  }
  Number.MultiplyNumerator(2);
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
   Number.MultiplyNumerator  (2);
   Number.MultiplyDenominator(2);
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
  Number.MultiplyNumerator(2);
  Exponent--;
 }
 while(Exponent < 0){
  Number.MultiplyDenominator(2);
  Exponent++;
 }
 return true;
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::TranslateDecimal(NUMBER& Number){
 const char* Buffer = ppToken.Body.String();

 Number = 0;

 while(*Buffer){
  if(*Buffer == '.') break;
  if(*Buffer == 'e' || *Buffer == 'E') break;
  if(*Buffer <  '0' || *Buffer >  '9'){
   Error("Invalid number format");
   return false;
  }
  Number.MultiplyNumerator(10);
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
   Number.MultiplyNumerator  (10);
   Number.MultiplyDenominator(10);
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
  Number.MultiplyNumerator(10);
  Exponent--;
 }
 while(Exponent < 0){
  Number.MultiplyDenominator(10);
  Exponent++;
 }
 return true;
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::TranslateNumber(NUMBER& Number){
 const char* Buffer = ppToken.Body.String();

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
 const char* Buffer = ppToken.Body.String();

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

/// @todo For macro expansion, add another layer above this one to translate
///       macros
bool PREPROCESSOR::GetPPToken(){
 Scanner_Stack->Scanner->GetToken(&ppToken);

 while(!error){
  if(ppToken.Type == SCANNER::tEOF){
   if(Scanner_Stack->Next){
    SCANNER_STACK* Temp = Scanner_Stack;
    Scanner_Stack       = Scanner_Stack->Next;
    delete Temp;

    Scanner_Stack->Scanner->GetToken(&ppToken);
    if(ppToken.Type != SCANNER::tNewline){
     Error("Newline expected");
     break;
    }
   }else{
    break;
   }

  }else if(ppToken.Type == SCANNER::tNewline){
   Scanner_Stack->Scanner->GetToken(&ppToken);

  }else if(ppToken.Type == SCANNER::tDirective){
   if(!ppToken.Body.Compare("include")){
    Scanner_Stack->Scanner->GetToken(&ppToken);
    if(ppToken.Type != SCANNER::tString) break;

    STRING Filename;
    Filename << Path << "/" << ppToken.Body;

    SCANNER_STACK* Temp = new SCANNER_STACK;
    Temp->Next          = Scanner_Stack;
    Scanner_Stack       = Temp;

    if(!Scanner_Stack->Scanner->Open(Filename.String())) return false;
    GetPPToken();

   }else{ // Ignore the directive line
    /// @todo Run directives (and macro expansion) in a Get_ppToken() function
    while(Scanner_Stack->Scanner->GetToken(&ppToken)){ // Just ignore them for now...
     if(ppToken.Type == SCANNER::tNewline){
      Scanner_Stack->Scanner->GetToken(&ppToken);
      break;
     }
    }
   }

  }else if(ppToken.Type == SCANNER::tIdentifier){
   /// @todo Run macro-expansion
   return !error && ppToken.Type != SCANNER::tEOF;

  }else{
   return !error && ppToken.Type != SCANNER::tEOF;
  }
 }
 ppToken.Type = SCANNER::tEOF;
 return false;
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::GetToken(TOKEN* Token){
 STRING Translated;

 Token->String .Clear();
 Token->Comment.Clear();
 Token->Type = tEOF;

 if(!Scanner_Stack) return false;

 while(!error && ppToken.Type != SCANNER::tEOF){
  Token->File    =  Scanner_Stack->Scanner->Filename;
  Token->Line    =  ppToken.Line;
  Token->Comment << ppToken.Comment;

  if(ppToken.Type == SCANNER::tString){
   if(!TranslateEscapes(Translated)) break;
   Token->Type   =  tString;
   Token->String << Translated;
   GetPPToken();
   continue;
  }
  if(Token->Type == tString) return true;

  if(ppToken.Type == SCANNER::tIdentifier){
   Token->Keyword = Keywords.GetCode(ppToken.Body.String());
   if(Token->Keyword){
    Token->Type = tKeyword;
   }else{
    Token->Type   = tIdentifier;
    Token->String = ppToken.Body;
   }
   GetPPToken();
   return true;
  }

  if(ppToken.Type == SCANNER::tNumber){
   if(!TranslateNumber(Token->Number)) break;
   Token->Number.Simplify();
   Token->Type  = tNumber;
   GetPPToken();
   return true;
  }

  if(ppToken.Type == SCANNER::tFixedPointCast){
   if(!TranslateFixedPointCast(Token)) break;
   Token->Type  = tFixedPointCast;
   GetPPToken();
   return true;
  }

  if(ppToken.Type == SCANNER::tCharacter){
   unsigned CodeLength;
   if(!TranslateEscapes(Translated)) break;
   Token->Number = Translated.GetUTF_32(0, &CodeLength);
   if(CodeLength < Translated.Length()){
    Error("Character too long");
    break;
   }
   Token->Type = tNumber;
   GetPPToken();
   return true;
  }

  if(ppToken.Type == SCANNER::tOperator){
   Token->Operator = Operators.GetCode(ppToken.Body.String());
   if(Token->Operator){
    Token->Type = tOperator;
    GetPPToken();
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
