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

#include "Scanner.h"
//------------------------------------------------------------------------------

static bool Initialised = false;

static TOKEN_TREE Spaces;
static TOKEN_TREE Keywords;
static TOKEN_TREE Operators;
//------------------------------------------------------------------------------

SCANNER::SCANNER(){
 if(!Initialised){
  Initialised = true;

  Spaces.Add("\x20"        , TOKEN::Space); // U+0020: Space
  Spaces.Add("\x09"        , TOKEN::Space); // U+0009: Character Tabulation
  Spaces.Add("\xA0"        , TOKEN::Space); // U+00A0: No-break Space
  Spaces.Add("\xC2\xA0"    , TOKEN::Space); // U+00A0: No-break Space
  Spaces.Add("\xE1\x9A\x80", TOKEN::Space); // U+1680: Ogham Space Mark
  Spaces.Add("\xE1\xA0\x8E", TOKEN::Space); // U+180E: Mongolian Vowel Separator
  Spaces.Add("\xE2\x80\x80", TOKEN::Space); // U+2000: En Quad
  Spaces.Add("\xE2\x80\x81", TOKEN::Space); // U+2001: Em Quad
  Spaces.Add("\xE2\x80\x82", TOKEN::Space); // U+2002: En Space
  Spaces.Add("\xE2\x80\x83", TOKEN::Space); // U+2003: Em Space
  Spaces.Add("\xE2\x80\x84", TOKEN::Space); // U+2004: Three-per-em Space
  Spaces.Add("\xE2\x80\x85", TOKEN::Space); // U+2005: Four-per-em Space
  Spaces.Add("\xE2\x80\x86", TOKEN::Space); // U+2006: Six-per-em Space
  Spaces.Add("\xE2\x80\x87", TOKEN::Space); // U+2007: Figure Space
  Spaces.Add("\xE2\x80\x88", TOKEN::Space); // U+2008: Punctuation Space
  Spaces.Add("\xE2\x80\x89", TOKEN::Space); // U+2009: Thin Space
  Spaces.Add("\xE2\x80\x8A", TOKEN::Space); // U+200A: Hair Space
  Spaces.Add("\xE2\x80\xAF", TOKEN::Space); // U+202F: Narrow No-break Space
  Spaces.Add("\xE2\x80\x8B", TOKEN::Space); // U+200B: Zero Width Space
  Spaces.Add("\xE2\x80\x8C", TOKEN::Space); // U+200C: Zero Width Non-joiner
  Spaces.Add("\xE2\x80\x8D", TOKEN::Space); // U+200D: Zero Width Joiner
  Spaces.Add("\x81\x9F"    , TOKEN::Space); // U+205F: Medium Mathematical Space
  Spaces.Add("\x81\xA0"    , TOKEN::Space); // U+2060: Word Joiner
  Spaces.Add("\xE3\x80\x80", TOKEN::Space); // U+3000: Ideographic Space
  Spaces.Add("\xEF\xBB\xBF", TOKEN::Space); // U+FEFF: Zero Width Non-breaking Space

  Spaces.Add("\n"          , TOKEN::Newline);
  Spaces.Add("\r"          , TOKEN::Newline);
  Spaces.Add("\r\n"        , TOKEN::Newline);
  Spaces.Add("\n\r"        , TOKEN::Newline);
  Spaces.Add("\x0B"        , TOKEN::Newline); // Vertical Tab
  Spaces.Add("\x0C"        , TOKEN::Newline); // Form Feed
  Spaces.Add("\x85"        , TOKEN::Newline); // Next Line (NEL)
  Spaces.Add("\xC2\x85"    , TOKEN::Newline); // U+85: NEL
  Spaces.Add("\xE2\x80\xA8", TOKEN::Newline); // U+2028: Line Separator
  Spaces.Add("\xE2\x80\xA9", TOKEN::Newline); // U+2029: Paragraph Separator

  Keywords.Add("target" , TOKEN::Target );
  Keywords.Add("pin"    , TOKEN::Pin    );
  Keywords.Add("sig"    , TOKEN::Sig    );
  Keywords.Add("clk"    , TOKEN::Clk    );
  Keywords.Add("int"    , TOKEN::Int    );
  Keywords.Add("rat"    , TOKEN::Rat    );
  Keywords.Add("float"  , TOKEN::Float  );
  Keywords.Add("complex", TOKEN::Complex);
  Keywords.Add("in"     , TOKEN::In     );
  Keywords.Add("out"    , TOKEN::Out    );
  Keywords.Add("signed" , TOKEN::Signed );
  Keywords.Add("group"  , TOKEN::Group  );
  Keywords.Add("class"  , TOKEN::Class  );
  Keywords.Add("if"     , TOKEN::If     );
  Keywords.Add("else"   , TOKEN::Else   );
  Keywords.Add("for"    , TOKEN::For    );
  Keywords.Add("while"  , TOKEN::While  );
  Keywords.Add("loop"   , TOKEN::Loop   );
  Keywords.Add("switch" , TOKEN::Switch );
  Keywords.Add("case"   , TOKEN::Case   );
  Keywords.Add("default", TOKEN::Default);
  Keywords.Add("import" , TOKEN::Import );
  Keywords.Add("as"     , TOKEN::As     );

  Operators.Add("++" , TOKEN::Increment         );
  Operators.Add("--" , TOKEN::Decrement         );
  Operators.Add("'"  , TOKEN::FP_Cast           );
  Operators.Add("->" , TOKEN::To                );
  Operators.Add("#"  , TOKEN::Hash              );
  Operators.Add("."  , TOKEN::Dot               );
  Operators.Add(".{" , TOKEN::Dot_Curly         );
  Operators.Add( "&" , TOKEN::Bit_AND           );
  Operators.Add("~&" , TOKEN::Bit_NAND          );
  Operators.Add( "|" , TOKEN::Bit_OR            );
  Operators.Add("~|" , TOKEN::Bit_NOR           );
  Operators.Add( "^" , TOKEN::Bit_XOR           );
  Operators.Add("~^" , TOKEN::Bit_XNOR          );
  Operators.Add("~"  , TOKEN::Bit_NOT           );
  Operators.Add(":"  , TOKEN::Bit_Concatenate   );
  Operators.Add("\\" , TOKEN::Bit_Repeat        );
  Operators.Add("@{" , TOKEN::Array_Concatenate );
  Operators.Add("+"  , TOKEN::Add               );
  Operators.Add("-"  , TOKEN::Subtract          );
  Operators.Add("*"  , TOKEN::Multiply          );
  Operators.Add("/"  , TOKEN::Divide            );
  Operators.Add("%"  , TOKEN::Modulus           );
  Operators.Add("<<" , TOKEN::Shift_Left        );
  Operators.Add(">>" , TOKEN::Shift_Right       );
  Operators.Add("<"  , TOKEN::Less              );
  Operators.Add(">"  , TOKEN::Greater           );
  Operators.Add("<=" , TOKEN::Less_Equal        );
  Operators.Add(">=" , TOKEN::Greater_Equal     );
  Operators.Add("==" , TOKEN::Equal             );
  Operators.Add("!=" , TOKEN::Not_Equal         );
  Operators.Add("!"  , TOKEN::Logical_NOT       );
  Operators.Add("&&" , TOKEN::Logical_AND       );
  Operators.Add("||" , TOKEN::Logical_OR        );
  Operators.Add("?"  , TOKEN::Conditional       );
  Operators.Add(  "=", TOKEN::Assign            );
  Operators.Add( ":=", TOKEN::Raw_Assign        );
  Operators.Add( "@=", TOKEN::Append_Assign     );
  Operators.Add( "+=", TOKEN::Add_Assign        );
  Operators.Add( "-=", TOKEN::Subtract_Assign   );
  Operators.Add( "*=", TOKEN::Multiply_Assign   );
  Operators.Add( "/=", TOKEN::Divide_Assign     );
  Operators.Add( "&=", TOKEN::AND_Assign        );
  Operators.Add( "|=", TOKEN::OR_Assign         );
  Operators.Add( "^=", TOKEN::XOR_Assign        );
  Operators.Add("<<=", TOKEN::Shift_Left_Assign );
  Operators.Add(">>=", TOKEN::Shift_Right_Assign);
  Operators.Add("("  , TOKEN::OpenRound         );
  Operators.Add(")"  , TOKEN::CloseRound        );
  Operators.Add("["  , TOKEN::OpenSquare        );
  Operators.Add("]"  , TOKEN::CloseSquare       );
  Operators.Add("{"  , TOKEN::OpenCurly         );
  Operators.Add("}"  , TOKEN::CloseCurly        );
  Operators.Add(","  , TOKEN::Comma             );
  Operators.Add(";"  , TOKEN::Semicolon         );

  Spaces   .Balance();
  Keywords .Balance();
  Operators.Balance();
 }

 Line   = 1;
 Index  = 0;
 error  = false;
 Buffer = 0;
}
//------------------------------------------------------------------------------

SCANNER::~SCANNER(){
 if(Buffer) delete[] Buffer;
}
//------------------------------------------------------------------------------

void SCANNER::Error(const char* Message){
 error = true;
 printf(
  "Line %05d of %s\n  Error: %s\n",
  Line,
  Filename.String(),
  Message
 );
}
//------------------------------------------------------------------------------

bool SCANNER::LineComment(){
 int Count;

 if(Buffer[Index] != '/' || Buffer[Index+1] != '/') return false;
 Index += 2;

 while(Buffer[Index]){
  if(Spaces.Match(Buffer+Index, &Count) == TOKEN::Newline){
   Line  ++;
   Index += Count;
   return true;
  }

  if(Count) Index += Count;
  else      Index ++;
 }
 Error("Incomplete line comment");
 return false;
}
//------------------------------------------------------------------------------

bool SCANNER::BlockComment(){
 int Count;

 if(Buffer[Index] != '/' || Buffer[Index+1] != '*') return false;
 Index += 2;

 while(Buffer[Index]){
  if(Buffer[Index] == '*' && Buffer[Index+1] == '/'){
   Index += 2;
   return true;
  }
  if(Spaces.Match(Buffer+Index, &Count) == TOKEN::Newline){
   Line  ++;
   Index += Count;
  }else{
   Index++;
  }
 }
 Error("Incomplete block comment");
 return false;
}
//------------------------------------------------------------------------------

void SCANNER::WhiteSpace(){
 TOKEN::TYPE Type;
 int         Count;

 while(Buffer[Index]){
  while(LineComment() || BlockComment());

  Type = Spaces.Match(Buffer+Index, &Count);
  switch(Type){
   case TOKEN::Space:
    Index += Count;
    break;

   case TOKEN::Newline:
    Line++;
    Index += Count;
    break;

   default: return;
  }
 }
}
//------------------------------------------------------------------------------

bool SCANNER::Open(const byte* Filename){
 FILE_SYSTEM fs;

 Line  = 1;
 Index = 0;
 error = false;

 if(Buffer) delete[] Buffer;

 this->Filename = Filename;

 Buffer = (byte*)fs.Read(Filename);
 if(!Buffer){
  printf("Error reading file: %s\n", Filename);
  return false;
 }

 return true;
}
//------------------------------------------------------------------------------

bool SCANNER::Digit(){
 return (Buffer[Index] >= '0' && Buffer[Index] <= '9');
}
//------------------------------------------------------------------------------

bool SCANNER::NonDigit(){
 int Count;
 if(Buffer[Index] >= 0x80){
  if(Spaces.Match(Buffer+Index, &Count)) return false;
 }

 return (Buffer[Index] >= 'a' && Buffer[Index] <= 'z' ) ||
        (Buffer[Index] >= 'A' && Buffer[Index] <= 'Z' ) ||
        (Buffer[Index] == '_' || Buffer[Index] >= 0x80);
}
//------------------------------------------------------------------------------

bool SCANNER::Identifier(TOKEN* Token){
 if(!NonDigit()) return false;

 Token->Data << Buffer[Index++];
 while(Buffer[Index]){
  if(!Digit() && !NonDigit()) break;
  Token->Data << Buffer[Index++];
 }
 Token->Type = Keywords.Find(Token->Data.String());
 if(!Token->Type) Token->Type = TOKEN::Identifier;
 return true;
}
//------------------------------------------------------------------------------

bool SCANNER::Operator(TOKEN* Token){
 int Count;

 Token->Type = Operators.Match(Buffer+Index, &Count);
 if(Count){
  while(Count){
   Token->Data << Buffer[Index++];
   Count--;
  }
  return true;
 }
 return false;
}
//------------------------------------------------------------------------------

bool SCANNER::GetDigit(unsigned* Digit, unsigned Base){
 if(Buffer[Index] >= '0' && Buffer[Index] <= '9'){
  *Digit = Buffer[Index] - '0';
  return *Digit < Base;
 }
 if(Buffer[Index] >= 'A' && Buffer[Index] <= 'Z'){
  *Digit = Buffer[Index] - 'A' + 0xA;
  return *Digit < Base;
 }
 if(Buffer[Index] >= 'a' && Buffer[Index] <= 'z'){
  *Digit = Buffer[Index] - 'a' + 0xA;
  return *Digit < Base;
 }
 return false;
}
//------------------------------------------------------------------------------

unsigned SCANNER::GetExponent(bool* Sign){
 unsigned Exponent = 0;

 Index++;
 while(Buffer[Index] == '_') Index++;

 *Sign = false;
 if(Buffer[Index] == '-'){
  *Sign = true;
  Index++;
 }else if(Buffer[Index] == '+'){
  Index++;
 }

 while(Buffer[Index]){
  while(Buffer[Index] == '_') Index++;

  if(Buffer[Index] < '0' || Buffer[Index] > '9') break;

  Exponent = 10*Exponent + Buffer[Index++] - '0';
 }
 return Exponent;
}
//------------------------------------------------------------------------------

bool SCANNER::GetNumber(TOKEN* Token, unsigned Base){
 unsigned Digit;

 while(Buffer[Index] == '_') Index++;
 if(!GetDigit(&Digit, Base) && Buffer[Index] != '.'){
  Error("Illegal literal format");
  return false;
 }

 mpz_t num, den, exp;
 mpz_init_set_ui(num, 0);
 mpz_init_set_ui(den, 1);
 mpz_init_set_ui(exp, 0);

 while(Buffer[Index]){
  while(Buffer[Index] == '_') Index++;

  if(!GetDigit(&Digit, Base)) break;

  mpz_mul_ui(num, num, Base);
  mpz_add_ui(num, num, Digit);
  Index++;
 }

 if(Buffer[Index] == '.'){
  Index++;
  while(Buffer[Index]){
   while(Buffer[Index] == '_') Index++;

   if(!GetDigit(&Digit, Base)) break;

   mpz_mul_ui(num, num, Base);
   mpz_mul_ui(den, den, Base);
   mpz_add_ui(num, num, Digit);
   Index++;
  }
 }

 bool     Sign     = false;
 unsigned Exponent = 0;

 if(Base == 10 && (Buffer[Index] == 'e' || Buffer[Index] == 'E')){
  Exponent = GetExponent(&Sign);
  mpz_ui_pow_ui(exp, 10, Exponent);

 }else if(Buffer[Index] == 'p' || Buffer[Index] == 'P'){
  Exponent = GetExponent(&Sign);
  mpz_ui_pow_ui(exp, 2, Exponent);
 }

 if(Sign) mpz_mul(den, den, exp);
 else     mpz_mul(num, num, exp);

 Token->Value.Set(num, den);

 if(Buffer[Index] == 'i' || Buffer[Index] == 'j'){
  Index++;
  while(Buffer[Index] == '_') Index++;
  Token->Value.Mul(0, 1);
 }

 mpz_clear(num);
 mpz_clear(den);
 mpz_clear(exp);

 return true;
}
//------------------------------------------------------------------------------

bool SCANNER::Literal(TOKEN* Token){
 Token->Type = TOKEN::Literal;

 if(
  (                        Buffer[Index  ] < '0' || Buffer[Index  ] > '9') &&
  (Buffer[Index] != '.' || Buffer[Index+1] < '0' || Buffer[Index+1] > '9')
 ) return false;

 if(Buffer[Index] == '0'){
  switch(Buffer[Index+1]){
   case 'b': Index += 2; return GetNumber(Token,  2);
   case 'o': Index += 2; return GetNumber(Token,  8);
   case 'x': Index += 2; return GetNumber(Token, 16);
   default : break;
  }
 }
 return GetNumber(Token, 10);
}
//------------------------------------------------------------------------------

bool SCANNER::String(TOKEN* Token){
 int j;
 unsigned Digit, UTF_32;

 if(Buffer[Index] != '"') return false;

 Token->Type = TOKEN::String;

 Index++;

 while(Buffer[Index]){
  if(Buffer[Index] == '"'){
   Index++;
   return true;
  }
  if(Buffer[Index] == '\\'){
   Index++;
   switch(Buffer[Index]){
    case 'n' : Token->Data << '\n'; Index++; break;
    case 't' : Token->Data << '\t'; Index++; break;
    case 'v' : Token->Data << '\v'; Index++; break;
    case 'b' : Token->Data << '\b'; Index++; break;
    case 'r' : Token->Data << '\r'; Index++; break;
    case 'f' : Token->Data << '\f'; Index++; break;
    case 'a' : Token->Data << '\a'; Index++; break;
    case '\\': Token->Data << '\\'; Index++; break;
    case '?' : Token->Data << '\?'; Index++; break;
    case '\'': Token->Data << '\''; Index++; break;
    case '"' : Token->Data << '\"'; Index++; break;

    case 'x' : // Hexadecimal number
     Index++;
     UTF_32 = 0;
     for(j = 0; j < 2; j++){
      if(!GetDigit(&Digit, 16)){
       Error("Invalid \\x code");
       return false;
      }
      UTF_32 = UTF_32*0x10 + Digit;
      Index++;
     }
     Token->Data.Append_UTF_32(UTF_32);
     break;

    case 'u' : // 16-bit Unicode
     Index++;
     UTF_32 = 0;
     for(j = 0; j < 4; j++){
      if(!GetDigit(&Digit, 16)){
       Error("Invalid \\u code");
       return false;
      }
      UTF_32 = UTF_32*0x10 + Digit;
      Index++;
     }
     Token->Data.Append_UTF_32(UTF_32);
     break;

    case 'U' : // 32-bit Unicode
     Index++;
     UTF_32 = 0;
     for(j = 0; j < 8; j++){
      if(!GetDigit(&Digit, 16)){
       Error("Invalid \\U code");
       return false;
      }
      UTF_32 = UTF_32*0x10 + Digit;
      Index++;
     }
     Token->Data.Append_UTF_32(UTF_32);
     break;

    default: // Could be an octal number...
     UTF_32 = 0;
     for(j = 0; j < 11; j++){
      if(Buffer[Index] < '0' || Buffer[Index] > '7'){
       if(j) break;
       Error("Invalid escape sequence");
       return false;
      }
      UTF_32 = UTF_32*8 + Buffer[Index++] - '0';
     }
     Token->Data.Append_UTF_32(UTF_32);
     break;
   }
  }else{
   if(Spaces.Match(Buffer+Index, &j) == TOKEN::Newline){
    Line++;
    while(j){
     Token->Data << Buffer[Index++];
     j--;
    }
   }else{
    Token->Data << Buffer[Index++];
   }
  }
 }
 Error("Incomplete string");
 return false;
}
//------------------------------------------------------------------------------

bool SCANNER::GetToken(TOKEN* Token){
 if(!Buffer[Index]) return false;
 if(error) return false;

 WhiteSpace();

 Token->Line = Line;
 Token->Type = TOKEN::Unknown;
 Token->Data.Clear();

 if(String(Token)){ // This is the least expensive match
  while(Buffer[Index]){
   WhiteSpace();
   if(!String(Token)) return !error;
  }
 }

 if(Literal   (Token)) return true;
 if(Identifier(Token)) return true;
 if(Operator  (Token)) return true; // This is the most expensive match

 return false;
}
//------------------------------------------------------------------------------
