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

  Spaces.Insert("\x20"        , TOKEN::Space); // U+0020: Space
  Spaces.Insert("\x09"        , TOKEN::Space); // U+0009: Character Tabulation
  Spaces.Insert("\xA0"        , TOKEN::Space); // U+00A0: No-break Space
  Spaces.Insert("\xC2\xA0"    , TOKEN::Space); // U+00A0: No-break Space
  Spaces.Insert("\xE1\x9A\x80", TOKEN::Space); // U+1680: Ogham Space Mark
  Spaces.Insert("\xE1\xA0\x8E", TOKEN::Space); // U+180E: Mongolian Vowel Separator
  Spaces.Insert("\xE2\x80\x80", TOKEN::Space); // U+2000: En Quad
  Spaces.Insert("\xE2\x80\x81", TOKEN::Space); // U+2001: Em Quad
  Spaces.Insert("\xE2\x80\x82", TOKEN::Space); // U+2002: En Space
  Spaces.Insert("\xE2\x80\x83", TOKEN::Space); // U+2003: Em Space
  Spaces.Insert("\xE2\x80\x84", TOKEN::Space); // U+2004: Three-per-em Space
  Spaces.Insert("\xE2\x80\x85", TOKEN::Space); // U+2005: Four-per-em Space
  Spaces.Insert("\xE2\x80\x86", TOKEN::Space); // U+2006: Six-per-em Space
  Spaces.Insert("\xE2\x80\x87", TOKEN::Space); // U+2007: Figure Space
  Spaces.Insert("\xE2\x80\x88", TOKEN::Space); // U+2008: Punctuation Space
  Spaces.Insert("\xE2\x80\x89", TOKEN::Space); // U+2009: Thin Space
  Spaces.Insert("\xE2\x80\x8A", TOKEN::Space); // U+200A: Hair Space
  Spaces.Insert("\xE2\x80\xAF", TOKEN::Space); // U+202F: Narrow No-break Space
  Spaces.Insert("\xE2\x80\x8B", TOKEN::Space); // U+200B: Zero Width Space
  Spaces.Insert("\xE2\x80\x8C", TOKEN::Space); // U+200C: Zero Width Non-joiner
  Spaces.Insert("\xE2\x80\x8D", TOKEN::Space); // U+200D: Zero Width Joiner
  Spaces.Insert("\x81\x9F"    , TOKEN::Space); // U+205F: Medium Mathematical Space
  Spaces.Insert("\x81\xA0"    , TOKEN::Space); // U+2060: Word Joiner
  Spaces.Insert("\xE3\x80\x80", TOKEN::Space); // U+3000: Ideographic Space
  Spaces.Insert("\xEF\xBB\xBF", TOKEN::Space); // U+FEFF: Zero Width Non-breaking Space

  Spaces.Insert("\n"          , TOKEN::Newline);
  Spaces.Insert("\r"          , TOKEN::Newline);
  Spaces.Insert("\r\n"        , TOKEN::Newline);
  Spaces.Insert("\n\r"        , TOKEN::Newline);
  Spaces.Insert("\x0B"        , TOKEN::Newline); // Vertical Tab
  Spaces.Insert("\x0C"        , TOKEN::Newline); // Form Feed
  Spaces.Insert("\x85"        , TOKEN::Newline); // Next Line (NEL)
  Spaces.Insert("\xC2\x85"    , TOKEN::Newline); // U+0085: NEL
  Spaces.Insert("\xE2\x80\xA8", TOKEN::Newline); // U+2028: Line Separator
  Spaces.Insert("\xE2\x80\xA9", TOKEN::Newline); // U+2029: Paragraph Separator

  Keywords.Insert("target" , TOKEN::Target );
  Keywords.Insert("void"   , TOKEN::Void   );
  Keywords.Insert("pin"    , TOKEN::Pin    );
  Keywords.Insert("sig"    , TOKEN::Sig    );
  Keywords.Insert("clk"    , TOKEN::Clk    );
  Keywords.Insert("int"    , TOKEN::Int    );
  Keywords.Insert("rat"    , TOKEN::Rat    );
  Keywords.Insert("float"  , TOKEN::Float  );
  Keywords.Insert("complex", TOKEN::Complex);
  Keywords.Insert("in"     , TOKEN::In     );
  Keywords.Insert("out"    , TOKEN::Out    );
  Keywords.Insert("signed" , TOKEN::Signed );
  Keywords.Insert("group"  , TOKEN::Group  );
  Keywords.Insert("class"  , TOKEN::Class  );
  Keywords.Insert("if"     , TOKEN::If     );
  Keywords.Insert("else"   , TOKEN::Else   );
  Keywords.Insert("for"    , TOKEN::For    );
  Keywords.Insert("while"  , TOKEN::While  );
  Keywords.Insert("loop"   , TOKEN::Loop   );
  Keywords.Insert("switch" , TOKEN::Switch );
  Keywords.Insert("case"   , TOKEN::Case   );
  Keywords.Insert("default", TOKEN::Default);
  Keywords.Insert("import" , TOKEN::Import );
  Keywords.Insert("as"     , TOKEN::As     );

  Operators.Insert("++" , TOKEN::Increment         );
  Operators.Insert("--" , TOKEN::Decrement         );
  Operators.Insert("'"  , TOKEN::FP_Cast           );
  Operators.Insert("->" , TOKEN::To                );
  Operators.Insert("#"  , TOKEN::Step              );
  Operators.Insert("."  , TOKEN::Dot               );
  Operators.Insert(".{" , TOKEN::Dot_Curly         );
  Operators.Insert( "&" , TOKEN::Bit_AND           );
  Operators.Insert("~&" , TOKEN::Bit_NAND          );
  Operators.Insert( "|" , TOKEN::Bit_OR            );
  Operators.Insert("~|" , TOKEN::Bit_NOR           );
  Operators.Insert( "^" , TOKEN::Bit_XOR           );
  Operators.Insert("~^" , TOKEN::Bit_XNOR          );
  Operators.Insert("~"  , TOKEN::Bit_NOT           );
  Operators.Insert("\\" , TOKEN::Bit_Repeat        );
  Operators.Insert("@{" , TOKEN::Array_Concatenate );
  Operators.Insert("+"  , TOKEN::Add               );
  Operators.Insert("-"  , TOKEN::Subtract          );
  Operators.Insert("*"  , TOKEN::Multiply          );
  Operators.Insert("/"  , TOKEN::Divide            );
  Operators.Insert("%"  , TOKEN::Modulus           );
  Operators.Insert("<<" , TOKEN::Shift_Left        );
  Operators.Insert(">>" , TOKEN::Shift_Right       );
  Operators.Insert("<"  , TOKEN::Less              );
  Operators.Insert(">"  , TOKEN::Greater           );
  Operators.Insert("<=" , TOKEN::Less_Equal        );
  Operators.Insert(">=" , TOKEN::Greater_Equal     );
  Operators.Insert("==" , TOKEN::Equal             );
  Operators.Insert("!=" , TOKEN::Not_Equal         );
  Operators.Insert("!"  , TOKEN::Logical_NOT       );
  Operators.Insert("&&" , TOKEN::Logical_AND       );
  Operators.Insert("||" , TOKEN::Logical_OR        );
  Operators.Insert("?"  , TOKEN::Conditional       );
  Operators.Insert(  "=", TOKEN::Assign            );
  Operators.Insert( ":=", TOKEN::Raw_Assign        );
  Operators.Insert( "@=", TOKEN::Append_Assign     );
  Operators.Insert( "+=", TOKEN::Add_Assign        );
  Operators.Insert( "-=", TOKEN::Subtract_Assign   );
  Operators.Insert( "*=", TOKEN::Multiply_Assign   );
  Operators.Insert( "/=", TOKEN::Divide_Assign     );
  Operators.Insert( "%=", TOKEN::Modulus_Assign    );
  Operators.Insert( "&=", TOKEN::AND_Assign        );
  Operators.Insert( "|=", TOKEN::OR_Assign         );
  Operators.Insert( "^=", TOKEN::XOR_Assign        );
  Operators.Insert("<<=", TOKEN::Shift_Left_Assign );
  Operators.Insert(">>=", TOKEN::Shift_Right_Assign);
  Operators.Insert("("  , TOKEN::OpenRound         );
  Operators.Insert(")"  , TOKEN::CloseRound        );
  Operators.Insert("["  , TOKEN::OpenSquare        );
  Operators.Insert("]"  , TOKEN::CloseSquare       );
  Operators.Insert("{"  , TOKEN::OpenCurly         );
  Operators.Insert("}"  , TOKEN::CloseCurly        );
  Operators.Insert(","  , TOKEN::Comma             );
  Operators.Insert(":"  , TOKEN::Colon             );
  Operators.Insert(";"  , TOKEN::Semicolon         );

  Spaces   .Balance();
  Keywords .Balance();
  Operators.Balance();
 }

 Next   = 0;
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
 if(!Token->Type){
  Token->ID   = IdentifierTree.GetID(Token->Data.String());
  Token->Type = TOKEN::Identifier;
 }
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

unsigned SCANNER::GetExponent(bool* Sign, TOKEN* Token){
 unsigned Exponent = 0;

 Token->Data << Buffer[Index++];
 while(Buffer[Index] == '_') Token->Data << Buffer[Index++];

 *Sign = false;
 if(Buffer[Index] == '-'){
  *Sign = true;
  Token->Data << Buffer[Index++];
 }else if(Buffer[Index] == '+'){
  Token->Data << Buffer[Index++];
 }

 while(Buffer[Index]){
  while(Buffer[Index] == '_') Token->Data << Buffer[Index++];

  if(Buffer[Index] < '0' || Buffer[Index] > '9') break;

  Exponent = 10*Exponent + Buffer[Index] - '0';
  Token->Data << Buffer[Index++];
 }
 return Exponent;
}
//------------------------------------------------------------------------------

bool SCANNER::GetNumber(TOKEN* Token, unsigned Base){
 unsigned Digit;

 while(Buffer[Index] == '_') Token->Data << Buffer[Index++];
 if(!GetDigit(&Digit, Base) && Buffer[Index] != '.'){
  Error("Illegal literal format");
  return false;
 }

 mpz_t num, den, exp;
 mpz_init_set_ui(num, 0);
 mpz_init_set_ui(den, 1);
 mpz_init_set_ui(exp, 1);

 while(Buffer[Index]){
  while(Buffer[Index] == '_') Token->Data << Buffer[Index++];

  if(!GetDigit(&Digit, Base)) break;

  mpz_mul_ui(num, num, Base);
  mpz_add_ui(num, num, Digit);
  Token->Data << Buffer[Index++];
 }

 if(Buffer[Index] == '.'){
  Token->Data << Buffer[Index++];
  while(Buffer[Index]){
   while(Buffer[Index] == '_') Token->Data << Buffer[Index++];

   if(!GetDigit(&Digit, Base)) break;

   mpz_mul_ui(num, num, Base);
   mpz_mul_ui(den, den, Base);
   mpz_add_ui(num, num, Digit);
   Token->Data << Buffer[Index++];
  }
 }

 bool     Sign     = false;
 unsigned Exponent = 0;

 if(Base == 10 && (Buffer[Index] == 'e' || Buffer[Index] == 'E')){
  Exponent = GetExponent(&Sign, Token);
  mpz_ui_pow_ui(exp, 10, Exponent);

 }else if(Buffer[Index] == 'p' || Buffer[Index] == 'P'){
  Exponent = GetExponent(&Sign, Token);
  mpz_ui_pow_ui(exp, 2, Exponent);
 }

 if(Sign) mpz_mul(den, den, exp);
 else     mpz_mul(num, num, exp);

 Token->Value.Set(num, den);

 if(Buffer[Index] == 'i' || Buffer[Index] == 'j'){
  Token->Data << Buffer[Index++];
  while(Buffer[Index] == '_') Token->Data << Buffer[Index++];
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
   case 'b':
    Token->Data << Buffer[Index++];
    Token->Data << Buffer[Index++];
    return GetNumber(Token, 2);

   case 'o':
    Token->Data << Buffer[Index++];
    Token->Data << Buffer[Index++];
    return GetNumber(Token, 8);

   case 'x':
    Token->Data << Buffer[Index++];
    Token->Data << Buffer[Index++];
    return GetNumber(Token, 16);

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
   WhiteSpace();
   if(Buffer[Index] == '"'){ // Concatenate the next string
    Index++;
    continue;
   }else{
    return true;
   }
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

bool SCANNER::Open(const byte* Filename){
 FILE_SYSTEM fs;

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

bool SCANNER::GetToken(TOKEN* Token){
 Token->Line = Line;
 Token->ID   = 0;
 Token->Type = TOKEN::Unknown;
 Token->Data.Clear();

 if(!Buffer[Index]) return false;
 if( error        ) return false;

 WhiteSpace();

 Token->Line = Line;

 if(String    (Token)) return true; // This is the least expensive match
 if(Literal   (Token)) return true;
 if(Identifier(Token)) return true;
 if(Operator  (Token)) return true; // This is the most expensive match

 if(Buffer[Index]){
  char s[0x100];
  sprintf(s, "Unknown token near \"%.5s\"", Buffer+Index);
  Error(s);
 }
 Token->Type = TOKEN::Unknown;
 return false;
}
//------------------------------------------------------------------------------
