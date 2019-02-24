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
#include "CharacterNames.h"
//------------------------------------------------------------------------------

using namespace std;
//------------------------------------------------------------------------------

static bool Initialised = false;

static TOKEN_TREE Spaces;
static TOKEN_TREE Keywords;
static TOKEN_TREE Operators;

typedef map<string, const byte*> CHARACTERS;
static CHARACTERS Characters;
//------------------------------------------------------------------------------

SCANNER::SCANNER(){
  if(!Initialised){
    Initialised = true;

    Spaces.Insert("\x20"        , TOKEN::Space); // U+0020: Space
    Spaces.Insert("\x09"        , TOKEN::Space); // U+0009: Character Tabulation
    Spaces.Insert("\xA0"        , TOKEN::Space); // U+00A0: No-break Space
    Spaces.Insert("\xC2\xA0"    , TOKEN::Space); // U+00A0: No-break Space
    Spaces.Insert("\xE1\x9A\x80", TOKEN::Space); // U+1680: Ogham Space Mark
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
    Spaces.Insert("\xE2\x81\x9F", TOKEN::Space); // U+205F: Medium Mathematical Space
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

    Keywords.Insert("__FILE__"     , TOKEN::FILE);
    Keywords.Insert("__LINE__"     , TOKEN::LINE);
    Keywords.Insert("__DATE__"     , TOKEN::DATE);
    Keywords.Insert("__TIME__"     , TOKEN::TIME);
    // __CLASS__, __FUNCTION__, etc. are implemented as special variables

    Keywords.Insert("void"         , TOKEN::Void);
    Keywords.Insert("auto"         , TOKEN::Auto);

    Keywords.Insert("pin"          , TOKEN::Pin);
    Keywords.Insert("net"          , TOKEN::Net);

    Keywords.Insert("byte"         , TOKEN::Byte);
    Keywords.Insert("char"         , TOKEN::Char);
    Keywords.Insert("num"          , TOKEN::Num );

    Keywords.Insert("true"         , TOKEN::True );
    Keywords.Insert("false"        , TOKEN::False);

    Keywords.Insert("class"        , TOKEN::Class );
    Keywords.Insert("enum"         , TOKEN::Enum  );
    Keywords.Insert("struct"       , TOKEN::Struct);
    Keywords.Insert("group"        , TOKEN::Group );
    Keywords.Insert("alias"        , TOKEN::Alias );

    Keywords.Insert("input"        , TOKEN::Input );
    Keywords.Insert("output"       , TOKEN::Output);

    Keywords.Insert("public"       , TOKEN::Public   );
    Keywords.Insert("private"      , TOKEN::Private  );
    Keywords.Insert("protected"    , TOKEN::Protected);

    Keywords.Insert("if"           , TOKEN::If   );
    Keywords.Insert("else"         , TOKEN::Else );
    Keywords.Insert("for"          , TOKEN::For  );
    Keywords.Insert("in"           , TOKEN::In   );
    Keywords.Insert("while"        , TOKEN::While);
    Keywords.Insert("loop"         , TOKEN::Loop );

    Keywords.Insert("switch"       , TOKEN::Switch );
    Keywords.Insert("case"         , TOKEN::Case   );
    Keywords.Insert("default"      , TOKEN::Default);

    Keywords.Insert("import"       , TOKEN::Import);
    Keywords.Insert("as"           , TOKEN::As    );

    Keywords.Insert("return"       , TOKEN::Return  );
    Keywords.Insert("break"        , TOKEN::Break   );
    Keywords.Insert("continue"     , TOKEN::Continue);
    Keywords.Insert("goto"         , TOKEN::Goto    );

    Keywords.Insert("func"         , TOKEN::Func  );
    Keywords.Insert("inline"       , TOKEN::Inline);

    Keywords.Insert("rtl"          , TOKEN::RTL);
    Keywords.Insert("fsm"          , TOKEN::FSM);
    Keywords.Insert("hdl"          , TOKEN::HDL);

    Keywords.Insert("stimulus"     , TOKEN::Stimulus);
    Keywords.Insert("emulate"      , TOKEN::Emulate );
    Keywords.Insert("sequence"     , TOKEN::Sequence);
    Keywords.Insert("assert"       , TOKEN::Assert  );
    Keywords.Insert("wait"         , TOKEN::Wait    );

    // Conditional Expression
    Operators.Insert("?"  , TOKEN::TernaryIf  );
    Operators.Insert(":"  , TOKEN::TernaryElse);
    Operators.Insert("?:" , TOKEN::Elvis      );

    // Bit-wise
    Operators.Insert("|"  , TOKEN::Bit_OR     );
    Operators.Insert("~|" , TOKEN::Bit_NOR    );
    Operators.Insert("^"  , TOKEN::Bit_XOR    );
    Operators.Insert("~^" , TOKEN::Bit_XNOR   );
    Operators.Insert("&"  , TOKEN::Bit_AND    );
    Operators.Insert("~&" , TOKEN::Bit_NAND   );

    // Relational
    Operators.Insert("==" , TOKEN::Equal        );
    Operators.Insert("!=" , TOKEN::Not_Equal    );
    Operators.Insert("<"  , TOKEN::Less         );
    Operators.Insert(">"  , TOKEN::Greater      );
    Operators.Insert("<=" , TOKEN::Less_Equal   );
    Operators.Insert(">=" , TOKEN::Greater_Equal);

    // Shift
    Operators.Insert("<<" , TOKEN::Shift_Left );
    Operators.Insert(">>" , TOKEN::Shift_Right);

    // Arithmetic
    Operators.Insert("+"  , TOKEN::Add        );
    Operators.Insert("-"  , TOKEN::Subtract   );
    Operators.Insert("*"  , TOKEN::Multiply   );
    Operators.Insert("/"  , TOKEN::Divide     );
    Operators.Insert("%"  , TOKEN::Modulus    );
    Operators.Insert("**" , TOKEN::Exponential);
    Operators.Insert("!"  , TOKEN::Factorial  );

    // Concatenation of vectors and arrays
    Operators.Insert("`"  , TOKEN::Replicate       );
    Operators.Insert(":(" , TOKEN::Concatenate     );
    Operators.Insert(":[" , TOKEN::ArrayConcatenate);

    // Stringification
    Operators.Insert("$"  , TOKEN::Stringify          );
    Operators.Insert("$(" , TOKEN::StringifyExpression);

    // Reduction
    Operators.Insert("&"  , TOKEN::AND_Reduce );
    Operators.Insert("~&" , TOKEN::NAND_Reduce);
    Operators.Insert("|"  , TOKEN::OR_Reduce  );
    Operators.Insert("~|" , TOKEN::NOR_Reduce );
    Operators.Insert("^"  , TOKEN::XOR_Reduce );
    Operators.Insert("~^" , TOKEN::XNOR_Reduce);
    Operators.Insert("!"  , TOKEN::Logical_NOT);

    // Array
    Operators.Insert(".." , TOKEN::To  );
    Operators.Insert(":"  , TOKEN::Step);

    // Unary
    Operators.Insert("-"  , TOKEN::Negate   );
    Operators.Insert("~"  , TOKEN::Bit_NOT  );
    Operators.Insert(":"  , TOKEN::RawBits  );
    Operators.Insert("++" , TOKEN::Increment);
    Operators.Insert("--" , TOKEN::Decrement);

    // Accessors
    Operators.Insert("."  , TOKEN::AccessMember    );
    Operators.Insert("?." , TOKEN::AccessMemberSafe);
    Operators.Insert(".{" , TOKEN::AccessMemberPush);
    Operators.Insert("'"  , TOKEN::AccessAttribute );

    // Cast
    Operators.Insert("@"  , TOKEN::CastOp);

    // Assignment
    Operators.Insert("="  , TOKEN::Assign            );
    Operators.Insert(":=" , TOKEN::Raw_Assign        );
    Operators.Insert("~=" , TOKEN::Append_Assign     );
    Operators.Insert("+=" , TOKEN::Add_Assign        );
    Operators.Insert("-=" , TOKEN::Subtract_Assign   );
    Operators.Insert("*=" , TOKEN::Multiply_Assign   );
    Operators.Insert("/=" , TOKEN::Divide_Assign     );
    Operators.Insert("%=" , TOKEN::Modulus_Assign    );
    Operators.Insert("^=" , TOKEN::XOR_Assign        );
    Operators.Insert("&=" , TOKEN::AND_Assign        );
    Operators.Insert("|=" , TOKEN::OR_Assign         );
    Operators.Insert("**=", TOKEN::Exponential_Assign);
    Operators.Insert("<<=", TOKEN::Shift_Left_Assign );
    Operators.Insert(">>=", TOKEN::Shift_Right_Assign);

    // Punctuators
    Operators.Insert("("  , TOKEN::OpenRound  );
    Operators.Insert(")"  , TOKEN::CloseRound );
    Operators.Insert("["  , TOKEN::OpenSquare );
    Operators.Insert("]"  , TOKEN::CloseSquare);
    Operators.Insert("{"  , TOKEN::OpenCurly  );
    Operators.Insert("}"  , TOKEN::CloseCurly );
    Operators.Insert("<"  , TOKEN::OpenAngle  );
    Operators.Insert(">"  , TOKEN::CloseAngle );
    Operators.Insert(","  , TOKEN::Comma      );
    Operators.Insert(":"  , TOKEN::Colon      );
    Operators.Insert(";"  , TOKEN::Semicolon  );

    // Simulation Operators
    Operators.Insert("#"  , TOKEN::WaitFor               );
    Operators.Insert("@"  , TOKEN::WaitUntil             );
    Operators.Insert("##" , TOKEN::WaitCycles            );
    Operators.Insert("[*" , TOKEN::SequenceConsecutive   );
    Operators.Insert("[->", TOKEN::SequenceGoto          );
    Operators.Insert("[=" , TOKEN::SequenceNonConsecutive);
    Operators.Insert("|->", TOKEN::AssertImplies         );
    Operators.Insert("|=>", TOKEN::AssertImpliesNext     );
    Operators.Insert("||" , TOKEN::Or                    );
    Operators.Insert("&&" , TOKEN::And                   );
    Operators.Insert("&&&", TOKEN::Intersect             );

    Spaces   .Balance();
    Keywords .Balance();
    Operators.Balance();

    for(int j = 0; CharacterNames[j]; j += 2){
      Characters[CharacterNames[j]] = (const byte*)CharacterNames[j+1];
    }
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
    ANSI_FG_BRIGHT_BLACK "Line "
    ANSI_FG_CYAN         "%05d "
    ANSI_FG_BRIGHT_BLACK "of "
    ANSI_FG_YELLOW       "%s\n"
    ANSI_FG_BRIGHT_RED   "  Error:"
    ANSI_RESET           " %s\n",
    Line,
    Filename.c_str(),
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

bool SCANNER::TodoComment(){
  int Count;

  if(Buffer[Index] != '!' || Buffer[Index+1] != '!') return false;
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
  Error("Incomplete todo comment");
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

bool SCANNER::NestedComment(){
  int Count;

  if(Buffer[Index] != '/' || Buffer[Index+1] != '+') return false;
  Index += 2;

  while(Buffer[Index]){
    if(Buffer[Index] == '+' && Buffer[Index+1] == '/'){
      Index += 2;
      return true;
    }

    if(NestedComment()) continue;
    if(error) return false;

    if(Spaces.Match(Buffer+Index, &Count) == TOKEN::Newline){
      Line  ++;
      Index += Count;
    }else{
      Index++;
    }
  }
  Error("Incomplete nested comment");
  return false;
}
//------------------------------------------------------------------------------

void SCANNER::WhiteSpace(){
  TOKEN::TYPE Type;
  int         Count;

  while(Buffer[Index]){
    while(LineComment() || TodoComment() || BlockComment() | NestedComment());

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
  time_t Timer;
  tm*    Time;

  if(!NonDigit()) return false;

  Token->Data += Buffer[Index++];
  while(Buffer[Index]){
    if(!Digit() && !NonDigit()) break;
    Token->Data += Buffer[Index++];
  }
  Token->Type = Keywords.Find((const byte*)Token->Data.c_str());

  switch(Token->Type){
    case 0:
      Token->Type = TOKEN::Identifier;
      break;

    case TOKEN::FILE:
      Token->Type = TOKEN::String;
      Token->Data = Filename.c_str();
      break;

    case TOKEN::LINE:
      Token->Type  = TOKEN::Literal;
      Token->Value = Token->Line;
      break;

    case TOKEN::DATE:
      Token->Type = TOKEN::String;
      Token->Data.clear();
      time(&Timer);
      Time = localtime(&Timer);
      Token->Data += Time->tm_year + 1900;
      Token->Data += "-";
      if(Time->tm_mon < 9) Token->Data += "0";
      Token->Data += Time->tm_mon + 1;
      Token->Data += "-";
      if(Time->tm_mday < 10) Token->Data += "0";
      Token->Data += Time->tm_mday;
      break;

    case TOKEN::TIME:
      Token->Type = TOKEN::String;
      Token->Data.clear();
      time(&Timer);
      Time = localtime(&Timer);
      if(Time->tm_hour < 10) Token->Data += "0";
      Token->Data += Time->tm_hour;
      Token->Data += ":";
      if(Time->tm_min < 10) Token->Data += "0";
      Token->Data += Time->tm_min;
      Token->Data += ":";
      if(Time->tm_sec < 10) Token->Data += "0";
      Token->Data += Time->tm_sec;
      break;

    default:
      break;
  }
  return true;
}
//------------------------------------------------------------------------------

bool SCANNER::Operator(TOKEN* Token){
  int Count;

  Token->Type = Operators.Match(Buffer+Index, &Count);
  if(Count){
    while(Count){
      Token->Data += Buffer[Index++];
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

  Token->Data += Buffer[Index++];
  while(Buffer[Index] == '_') Token->Data += Buffer[Index++];

  *Sign = false;
  if(Buffer[Index] == '-'){
    *Sign = true;
    Token->Data += Buffer[Index++];
  }else if(Buffer[Index] == '+'){
    Token->Data += Buffer[Index++];
  }

  while(Buffer[Index] == '_') Token->Data += Buffer[Index++];
  if(Buffer[Index] < '0' || Buffer[Index] > '9'){
    Error("Exponent digit expected");
    return 0;
  }

  while(Buffer[Index]){
    while(Buffer[Index] == '_') Token->Data += Buffer[Index++];

    if(Buffer[Index] < '0' || Buffer[Index] > '9') break;

    Exponent = 10*Exponent + Buffer[Index] - '0';
    Token->Data += Buffer[Index++];
  }
  return Exponent;
}
//------------------------------------------------------------------------------

bool SCANNER::GetNumber(TOKEN* Token, unsigned Base){
  unsigned Digit;

  while(Buffer[Index] == '_') Token->Data += Buffer[Index++];
  if(!GetDigit(&Digit, Base) && Buffer[Index] != '.'){
    Error("Illegal literal format");
    return false;
  }

  mpz_t num, den, exp;
  mpz_init_set_ui(num, 0);
  mpz_init_set_ui(den, 1);
  mpz_init_set_ui(exp, 1);

  while(Buffer[Index]){
    while(Buffer[Index] == '_') Token->Data += Buffer[Index++];

    if(!GetDigit(&Digit, Base)) break;

    mpz_mul_ui(num, num, Base);
    mpz_add_ui(num, num, Digit);
    Token->Data += Buffer[Index++];
  }

  if(Buffer[Index] == '.' && Buffer[Index+1] != '.'){
    Token->Data += Buffer[Index++];
    while(Buffer[Index]){
      while(Buffer[Index] == '_') Token->Data += Buffer[Index++];

      if(!GetDigit(&Digit, Base)) break;

      mpz_mul_ui(num, num, Base);
      mpz_mul_ui(den, den, Base);
      mpz_add_ui(num, num, Digit);
      Token->Data += Buffer[Index++];
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
    Token->Data += Buffer[Index++];
    while(Buffer[Index] == '_') Token->Data += Buffer[Index++];
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
        Token->Data += Buffer[Index++];
        Token->Data += Buffer[Index++];
        return GetNumber(Token, 2);

      case 'o':
        Token->Data += Buffer[Index++];
        Token->Data += Buffer[Index++];
        return GetNumber(Token, 8);

      case 'x':
        Token->Data += Buffer[Index++];
        Token->Data += Buffer[Index++];
        return GetNumber(Token, 16);

      default : break;
    }
  }
  return GetNumber(Token, 10);
}
//------------------------------------------------------------------------------

bool SCANNER::String(TOKEN* Token){
  int      j;
  unsigned Digit, UTF_32;
  CHARACTERS::iterator s;

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
        case 'n' : Token->Data += '\n'; Index++; break;
        case 't' : Token->Data += '\t'; Index++; break;
        case 'v' : Token->Data += '\v'; Index++; break;
        case 'b' : Token->Data += '\b'; Index++; break;
        case 'r' : Token->Data += '\r'; Index++; break;
        case 'f' : Token->Data += '\f'; Index++; break;
        case 'a' : Token->Data += '\a'; Index++; break;
        case '\\': Token->Data += '\\'; Index++; break;
        case '?' : Token->Data += '\?'; Index++; break;
        case '\'': Token->Data += '\''; Index++; break;
        case '"' : Token->Data += '\"'; Index++; break;

        case '&': // HTML character name
          Index++;
          for(j = Index; Buffer[j] && Buffer[j] != ';'; j++);
          if(!Buffer[j]){
            Error("Invalid \\& code");
            return false;
          }
          Buffer[j] = 0;
          s = Characters.find((const char*)(Buffer+Index));
          if(s == Characters.end()){
            Error("Invalid \\& code");
            return false;
          }
          Index = j+1;
          Token->Data += (const char*)s->second;
          break;

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
          Token->Data.append(UTF_Converter.UTF8((char32_t)UTF_32));
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
          Token->Data.append(UTF_Converter.UTF8((char32_t)UTF_32));
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
          Token->Data.append(UTF_Converter.UTF8((char32_t)UTF_32));
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
          Token->Data.append(UTF_Converter.UTF8((char32_t)UTF_32));
          break;
      }
    }else{
      if(Spaces.Match(Buffer+Index, &j) == TOKEN::Newline){
        Line++;
        while(j){
          Token->Data += Buffer[Index++];
          j--;
        }
      }else{
        Token->Data += Buffer[Index++];
      }
    }
  }
  Error("Incomplete string");
  return false;
}
//------------------------------------------------------------------------------

bool SCANNER::Open(const char* Filename){
  FILE_WRAPPER fs;

  error = false;

  if(Buffer) delete[] Buffer;

  this->Filename = Filename;

  Buffer = (byte*)fs.ReadAll(Filename);
  if(!Buffer){
    printf("Error reading file: %s\n", Filename);
    return false;
  }

  return true;
}
//------------------------------------------------------------------------------

bool SCANNER::GetToken(TOKEN* Token){
  Token->Line = Line;
  Token->Type = TOKEN::Unknown;
  Token->Data.clear();

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
