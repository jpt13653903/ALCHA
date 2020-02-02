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

    Spaces.Insert("\x20"        , TOKEN::TYPE::Space); // U+0020: Space
    Spaces.Insert("\x09"        , TOKEN::TYPE::Space); // U+0009: Character Tabulation
    Spaces.Insert("\xC2\xA0"    , TOKEN::TYPE::Space); // U+00A0: No-break Space
    Spaces.Insert("\xE1\x9A\x80", TOKEN::TYPE::Space); // U+1680: Ogham Space Mark
    Spaces.Insert("\xE2\x80\x80", TOKEN::TYPE::Space); // U+2000: En Quad
    Spaces.Insert("\xE2\x80\x81", TOKEN::TYPE::Space); // U+2001: Em Quad
    Spaces.Insert("\xE2\x80\x82", TOKEN::TYPE::Space); // U+2002: En Space
    Spaces.Insert("\xE2\x80\x83", TOKEN::TYPE::Space); // U+2003: Em Space
    Spaces.Insert("\xE2\x80\x84", TOKEN::TYPE::Space); // U+2004: Three-per-em Space
    Spaces.Insert("\xE2\x80\x85", TOKEN::TYPE::Space); // U+2005: Four-per-em Space
    Spaces.Insert("\xE2\x80\x86", TOKEN::TYPE::Space); // U+2006: Six-per-em Space
    Spaces.Insert("\xE2\x80\x87", TOKEN::TYPE::Space); // U+2007: Figure Space
    Spaces.Insert("\xE2\x80\x88", TOKEN::TYPE::Space); // U+2008: Punctuation Space
    Spaces.Insert("\xE2\x80\x89", TOKEN::TYPE::Space); // U+2009: Thin Space
    Spaces.Insert("\xE2\x80\x8A", TOKEN::TYPE::Space); // U+200A: Hair Space
    Spaces.Insert("\xE2\x80\xAF", TOKEN::TYPE::Space); // U+202F: Narrow No-break Space
    Spaces.Insert("\xE2\x80\x8B", TOKEN::TYPE::Space); // U+200B: Zero Width Space
    Spaces.Insert("\xE2\x81\x9F", TOKEN::TYPE::Space); // U+205F: Medium Mathematical Space
    Spaces.Insert("\xE3\x80\x80", TOKEN::TYPE::Space); // U+3000: Ideographic Space
    Spaces.Insert("\xEF\xBB\xBF", TOKEN::TYPE::Space); // U+FEFF: Zero Width Non-breaking Space

    Spaces.Insert("\n"          , TOKEN::TYPE::Newline);
    Spaces.Insert("\r"          , TOKEN::TYPE::Newline);
    Spaces.Insert("\r\n"        , TOKEN::TYPE::Newline);
    Spaces.Insert("\n\r"        , TOKEN::TYPE::Newline);
    Spaces.Insert("\x0B"        , TOKEN::TYPE::Newline); // Vertical Tab
    Spaces.Insert("\x0C"        , TOKEN::TYPE::Newline); // Form Feed
    Spaces.Insert("\xC2\x85"    , TOKEN::TYPE::Newline); // U+0085: NEL
    Spaces.Insert("\xE2\x80\xA8", TOKEN::TYPE::Newline); // U+2028: Line Separator
    Spaces.Insert("\xE2\x80\xA9", TOKEN::TYPE::Newline); // U+2029: Paragraph Separator

    Keywords.Insert("__FILE__"     , TOKEN::TYPE::FILE);
    Keywords.Insert("__LINE__"     , TOKEN::TYPE::LINE);
    Keywords.Insert("__DATE__"     , TOKEN::TYPE::DATE);
    Keywords.Insert("__TIME__"     , TOKEN::TYPE::TIME);
    // __CLASS__, __FUNCTION__, etc. are implemented as special variables

    Keywords.Insert("void"         , TOKEN::TYPE::Void);
    Keywords.Insert("auto"         , TOKEN::TYPE::Auto);

    Keywords.Insert("pin"          , TOKEN::TYPE::Pin);
    Keywords.Insert("net"          , TOKEN::TYPE::Net);

    Keywords.Insert("byte"         , TOKEN::TYPE::Byte);
    Keywords.Insert("char"         , TOKEN::TYPE::Char);
    Keywords.Insert("num"          , TOKEN::TYPE::Num );

    Keywords.Insert("true"         , TOKEN::TYPE::True );
    Keywords.Insert("false"        , TOKEN::TYPE::False);

    Keywords.Insert("class"        , TOKEN::TYPE::Class );
    Keywords.Insert("enum"         , TOKEN::TYPE::Enum  );
    Keywords.Insert("struct"       , TOKEN::TYPE::Struct);
    Keywords.Insert("group"        , TOKEN::TYPE::Group );
    Keywords.Insert("alias"        , TOKEN::TYPE::Alias );

    Keywords.Insert("input"        , TOKEN::TYPE::Input );
    Keywords.Insert("output"       , TOKEN::TYPE::Output);

    Keywords.Insert("public"       , TOKEN::TYPE::Public   );
    Keywords.Insert("private"      , TOKEN::TYPE::Private  );
    Keywords.Insert("protected"    , TOKEN::TYPE::Protected);

    Keywords.Insert("if"           , TOKEN::TYPE::If   );
    Keywords.Insert("else"         , TOKEN::TYPE::Else );
    Keywords.Insert("for"          , TOKEN::TYPE::For  );
    Keywords.Insert("in"           , TOKEN::TYPE::In   );
    Keywords.Insert("while"        , TOKEN::TYPE::While);
    Keywords.Insert("loop"         , TOKEN::TYPE::Loop );

    Keywords.Insert("switch"       , TOKEN::TYPE::Switch );
    Keywords.Insert("case"         , TOKEN::TYPE::Case   );
    Keywords.Insert("default"      , TOKEN::TYPE::Default);

    Keywords.Insert("import"       , TOKEN::TYPE::Import);
    Keywords.Insert("as"           , TOKEN::TYPE::As    );

    Keywords.Insert("return"       , TOKEN::TYPE::Return  );
    Keywords.Insert("break"        , TOKEN::TYPE::Break   );
    Keywords.Insert("continue"     , TOKEN::TYPE::Continue);
    Keywords.Insert("goto"         , TOKEN::TYPE::Goto    );

    Keywords.Insert("func"         , TOKEN::TYPE::Func  );
    Keywords.Insert("inline"       , TOKEN::TYPE::Inline);

    Keywords.Insert("rtl"          , TOKEN::TYPE::RTL);
    Keywords.Insert("fsm"          , TOKEN::TYPE::FSM);
    Keywords.Insert("hdl"          , TOKEN::TYPE::HDL);

    Keywords.Insert("stimulus"     , TOKEN::TYPE::Stimulus);
    Keywords.Insert("emulate"      , TOKEN::TYPE::Emulate );
    Keywords.Insert("sequence"     , TOKEN::TYPE::Sequence);
    Keywords.Insert("assert"       , TOKEN::TYPE::Assert  );
    Keywords.Insert("wait"         , TOKEN::TYPE::Wait    );

    // Conditional Expression
    Operators.Insert("?"  , TOKEN::TYPE::TernaryIf  );
    Operators.Insert(":"  , TOKEN::TYPE::TernaryElse);
    Operators.Insert("?:" , TOKEN::TYPE::Elvis      );

    // Bit-wise
    Operators.Insert("|"  , TOKEN::TYPE::Bit_OR     );
    Operators.Insert("~|" , TOKEN::TYPE::Bit_NOR    );
    Operators.Insert("^"  , TOKEN::TYPE::Bit_XOR    );
    Operators.Insert("~^" , TOKEN::TYPE::Bit_XNOR   );
    Operators.Insert("&"  , TOKEN::TYPE::Bit_AND    );
    Operators.Insert("~&" , TOKEN::TYPE::Bit_NAND   );

    // Relational
    Operators.Insert("==" , TOKEN::TYPE::Equal        );
    Operators.Insert("!=" , TOKEN::TYPE::Not_Equal    );
    Operators.Insert("<"  , TOKEN::TYPE::Less         );
    Operators.Insert(">"  , TOKEN::TYPE::Greater      );
    Operators.Insert("<=" , TOKEN::TYPE::Less_Equal   );
    Operators.Insert(">=" , TOKEN::TYPE::Greater_Equal);

    // Shift
    Operators.Insert("<<" , TOKEN::TYPE::Shift_Left );
    Operators.Insert(">>" , TOKEN::TYPE::Shift_Right);

    // Arithmetic
    Operators.Insert("+"  , TOKEN::TYPE::Add        );
    Operators.Insert("-"  , TOKEN::TYPE::Subtract   );
    Operators.Insert("*"  , TOKEN::TYPE::Multiply   );
    Operators.Insert("/"  , TOKEN::TYPE::Divide     );
    Operators.Insert("%"  , TOKEN::TYPE::Modulus    );
    Operators.Insert("**" , TOKEN::TYPE::Exponential);
    Operators.Insert("!"  , TOKEN::TYPE::Factorial  );

    // Concatenation of vectors and arrays
    Operators.Insert("`"  , TOKEN::TYPE::Replicate       );
    Operators.Insert(":(" , TOKEN::TYPE::Concatenate     );
    Operators.Insert(":[" , TOKEN::TYPE::ArrayConcatenate);

    // Stringification
    Operators.Insert("$"  , TOKEN::TYPE::Stringify          );
    Operators.Insert("$(" , TOKEN::TYPE::StringifyExpression);

    // Reduction
    Operators.Insert("&"  , TOKEN::TYPE::AND_Reduce );
    Operators.Insert("~&" , TOKEN::TYPE::NAND_Reduce);
    Operators.Insert("|"  , TOKEN::TYPE::OR_Reduce  );
    Operators.Insert("~|" , TOKEN::TYPE::NOR_Reduce );
    Operators.Insert("^"  , TOKEN::TYPE::XOR_Reduce );
    Operators.Insert("~^" , TOKEN::TYPE::XNOR_Reduce);
    Operators.Insert("!"  , TOKEN::TYPE::Logical_NOT);

    // Array
    Operators.Insert(".." , TOKEN::TYPE::To  );
    Operators.Insert(":"  , TOKEN::TYPE::Step);

    // Unary
    Operators.Insert("-"  , TOKEN::TYPE::Negate   );
    Operators.Insert("~"  , TOKEN::TYPE::Bit_NOT  );
    Operators.Insert(":"  , TOKEN::TYPE::RawBits  );
    Operators.Insert("++" , TOKEN::TYPE::Increment);
    Operators.Insert("--" , TOKEN::TYPE::Decrement);

    // Accessors
    Operators.Insert("."  , TOKEN::TYPE::AccessMember    );
    Operators.Insert("?." , TOKEN::TYPE::AccessMemberSafe);
    Operators.Insert(".{" , TOKEN::TYPE::AccessMemberPush);
    Operators.Insert("'"  , TOKEN::TYPE::AccessAttribute );

    // Cast
    Operators.Insert("@"  , TOKEN::TYPE::CastOp);

    // Assignment
    Operators.Insert("="  , TOKEN::TYPE::Assign            );
    Operators.Insert(":=" , TOKEN::TYPE::Raw_Assign        );
    Operators.Insert("~=" , TOKEN::TYPE::Append_Assign     );
    Operators.Insert("+=" , TOKEN::TYPE::Add_Assign        );
    Operators.Insert("-=" , TOKEN::TYPE::Subtract_Assign   );
    Operators.Insert("*=" , TOKEN::TYPE::Multiply_Assign   );
    Operators.Insert("/=" , TOKEN::TYPE::Divide_Assign     );
    Operators.Insert("%=" , TOKEN::TYPE::Modulus_Assign    );
    Operators.Insert("^=" , TOKEN::TYPE::XOR_Assign        );
    Operators.Insert("&=" , TOKEN::TYPE::AND_Assign        );
    Operators.Insert("|=" , TOKEN::TYPE::OR_Assign         );
    Operators.Insert("**=", TOKEN::TYPE::Exponential_Assign);
    Operators.Insert("<<=", TOKEN::TYPE::Shift_Left_Assign );
    Operators.Insert(">>=", TOKEN::TYPE::Shift_Right_Assign);

    // Punctuators
    Operators.Insert("("  , TOKEN::TYPE::OpenRound  );
    Operators.Insert(")"  , TOKEN::TYPE::CloseRound );
    Operators.Insert("["  , TOKEN::TYPE::OpenSquare );
    Operators.Insert("]"  , TOKEN::TYPE::CloseSquare);
    Operators.Insert("{"  , TOKEN::TYPE::OpenCurly  );
    Operators.Insert("}"  , TOKEN::TYPE::CloseCurly );
    Operators.Insert("<"  , TOKEN::TYPE::OpenAngle  );
    Operators.Insert(">"  , TOKEN::TYPE::CloseAngle );
    Operators.Insert(","  , TOKEN::TYPE::Comma      );
    Operators.Insert(":"  , TOKEN::TYPE::Colon      );
    Operators.Insert(";"  , TOKEN::TYPE::Semicolon  );

    // Simulation Operators
    Operators.Insert("#"  , TOKEN::TYPE::WaitFor               );
    Operators.Insert("@"  , TOKEN::TYPE::WaitUntil             );
    Operators.Insert("##" , TOKEN::TYPE::WaitCycles            );
    Operators.Insert("[*" , TOKEN::TYPE::SequenceConsecutive   );
    Operators.Insert("[->", TOKEN::TYPE::SequenceGoto          );
    Operators.Insert("[=" , TOKEN::TYPE::SequenceNonConsecutive);
    Operators.Insert("|->", TOKEN::TYPE::AssertImplies         );
    Operators.Insert("|=>", TOKEN::TYPE::AssertImpliesNext     );
    Operators.Insert("||" , TOKEN::TYPE::Or                    );
    Operators.Insert("&&" , TOKEN::TYPE::And                   );
    Operators.Insert("&&&", TOKEN::TYPE::Intersect             );

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
  ::Error(Line, Filename, Message);
}
//------------------------------------------------------------------------------

bool SCANNER::LineComment(){
  int Count;

  if(Buffer[Index] != '/' || Buffer[Index+1] != '/') return false;
  Index += 2;

  while(Buffer[Index]){
    if(Spaces.Match(Buffer+Index, &Count) == TOKEN::TYPE::Newline){
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
  int    Count;
  string Comment;

  if(Buffer[Index] != '!' || Buffer[Index+1] != '!') return false;
  Index += 2;

  while(Buffer[Index]){
    if(Spaces.Match(Buffer+Index, &Count) == TOKEN::TYPE::Newline){
      if(!Comment.empty()){
        printf(
          ANSI_FG_BRIGHT_BLACK   "Line "
          ANSI_FG_CYAN           "%05d "
          ANSI_FG_BRIGHT_BLACK   "of "
          ANSI_FG_YELLOW         "%s\n"
          ANSI_FG_BRIGHT_MAGENTA "  Todo:"
          ANSI_RESET             " %s\n",
          Line,
          Filename.c_str(),
          Comment .c_str()
        );
      }
      Line  ++;
      Index += Count;
      return true;
    }
    if(Count){
      for(int n = 0; n < Count; n++) Comment += Buffer[Index++];
    }else{
      Comment += Buffer[Index++];
    }
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
    if(Spaces.Match(Buffer+Index, &Count) == TOKEN::TYPE::Newline){
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

    if(Spaces.Match(Buffer+Index, &Count) == TOKEN::TYPE::Newline){
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
      case TOKEN::TYPE::Space:
        Index += Count;
        break;

      case TOKEN::TYPE::Newline:
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
    if(Spaces.Match(Buffer+Index, &Count) != TOKEN::TYPE::Unknown) return false;
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
    case TOKEN::TYPE::Unknown:
      Token->Type = TOKEN::TYPE::Identifier;
      break;

    case TOKEN::TYPE::FILE:
      Token->Type = TOKEN::TYPE::String;
      Token->Data = Filename.c_str();
      break;

    case TOKEN::TYPE::LINE:
      Token->Type  = TOKEN::TYPE::Literal;
      Token->Value = Token->Line;
      break;

    case TOKEN::TYPE::DATE:
      Token->Type = TOKEN::TYPE::String;
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

    case TOKEN::TYPE::TIME:
      Token->Type = TOKEN::TYPE::String;
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
  Token->Type = TOKEN::TYPE::Literal;

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

  Token->Type = TOKEN::TYPE::String;

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
      if(Spaces.Match(Buffer+Index, &j) == TOKEN::TYPE::Newline){
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

  Line  = 1;
  Index = 0;
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
  Token->Type = TOKEN::TYPE::Unknown;
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
  Token->Type = TOKEN::TYPE::Unknown;
  return false;
}
//------------------------------------------------------------------------------
