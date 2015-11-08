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

SCANNER::TOKEN::TOKEN(){
 Type = tOther;
 Line = 0;
 Next = 0;
}
//------------------------------------------------------------------------------

SCANNER::SCANNER(){
 Line          = 1;
 Index         = 0;
 error         = false;
 PrevIsNewline = true;
}
//------------------------------------------------------------------------------

void SCANNER::Error(const char* Message){
 error = true;
 printf("Line %05d of %s\n  Error: %s", Char.Line, Filename.String(), Message);
}
//------------------------------------------------------------------------------

int SCANNER::Space(){
 if(Buffer[Index] == 0x20) return 1; // U+0020: Space
 if(Buffer[Index] == 0x09) return 1; // U+0009: Character Tabulation
 if(Buffer[Index] == 0xA0) return 1; // U+00A0: No-break Space
 if(
  Buffer[Index  ] == 0xC2 &&
  Buffer[Index+1] == 0xA0
 ) return 2; // U+00A0: No-break Space
 if(Buffer[Index] == 0xE1){
  if(
   Buffer[Index+1] == 0x9A &&
   Buffer[Index+2] == 0x80
  ) return 3; // U+1680: Ogham Space Mark
  if(
   Buffer[Index+1] == 0xA0 &&
   Buffer[Index+2] == 0x8E
  ) return 3; // U+180E: Mongolian Vowel Separator
 }
 if(Buffer[Index] == 0xE2){
  if(Buffer[Index+1] == 0x80){
   if(Buffer[Index+2] == 0x80) return 3; // U+2000: En Quad
   if(Buffer[Index+2] == 0x81) return 3; // U+2001: Em Quad
   if(Buffer[Index+2] == 0x82) return 3; // U+2002: En Space
   if(Buffer[Index+2] == 0x83) return 3; // U+2003: Em Space
   if(Buffer[Index+2] == 0x84) return 3; // U+2004: Three-per-em Space
   if(Buffer[Index+2] == 0x85) return 3; // U+2005: Four-per-em Space
   if(Buffer[Index+2] == 0x86) return 3; // U+2006: Six-per-em Space
   if(Buffer[Index+2] == 0x87) return 3; // U+2007: Figure Space
   if(Buffer[Index+2] == 0x88) return 3; // U+2008: Punctuation Space
   if(Buffer[Index+2] == 0x89) return 3; // U+2009: Thin Space
   if(Buffer[Index+2] == 0x8A) return 3; // U+200A: Hair Space
   if(Buffer[Index+2] == 0xAF) return 3; // U+202F: Narrow No-break Space
   if(Buffer[Index+2] == 0x8B) return 3; // U+200B: Zero Width Space
   if(Buffer[Index+2] == 0x8C) return 3; // U+200C: Zero Width Non-joiner
   if(Buffer[Index+2] == 0x8D) return 3; // U+200D: Zero Width Joiner
  }
  if(Buffer[Index+1] == 0x81){
   if(Buffer[Index+2] == 0x9F) return 3; // U+205F: Medium Mathematical Space
   if(Buffer[Index+2] == 0xA0) return 3; // U+2060: Word Joiner
  }
 }
 if(
  Buffer[Index  ] == 0xE3 &&
  Buffer[Index+1] == 0x80 &&
  Buffer[Index+2] == 0x80
 ) return 3; // U+3000: Ideographic Space
 if(
  Buffer[Index  ] == 0xEF &&
  Buffer[Index+1] == 0xBB &&
  Buffer[Index+2] == 0xBF
 ) return 3; // U+FEFF: Zero Width Non-breaking Space

 return 0;
}
//------------------------------------------------------------------------------

int SCANNER::Spaces(){
 int Prev = Index;

 int i = Space();
 while(i){
  Index += i;
  i = Space();
 }

 int Result = Index - Prev;
 Index = Prev;
 return Result;
}
//------------------------------------------------------------------------------

int SCANNER::Newline(){
 if(Buffer[Index] == 0x0D){
  if(Buffer[Index+1] == 0x0A) return 2; // \r\n
  else                        return 1; // \r
 }
 if(Buffer[Index] == 0x0A){
  if(Buffer[Index+1] == 0x0D) return 2; // \n\r
  else                        return 1; // \n
 }
 if(Buffer[Index] == 0x0B) return 1; // Vertical Tab
 if(Buffer[Index] == 0x0C) return 1; // Form Feed
 if(Buffer[Index] == 0x85) return 1; // Next Line (NEL)
 if(Buffer[Index] == 0xC2 && Buffer[Index+1] == 0x85) return 2; // U+85: NEL
 if(Buffer[Index] == 0xE2 && Buffer[Index+1] == 0x80){
  if(Buffer[Index+2] == 0xA8) return 3; // U+2028: Line Separator
  if(Buffer[Index+2] == 0xA9) return 3; // U+2029: Paragraph Separator
 }

 return 0;
}
//------------------------------------------------------------------------------

bool SCANNER::Open(const char* Filename){
 Line  = 1;
 Index = 0;
 error = false;

 this->Filename = Filename;

 char* TempBuffer = fs.Read(Filename);
 if(!TempBuffer){
  printf("Error reading file: %s\n", Filename);
  return false;
 }

 Buffer.UseMem(TempBuffer);

 // Read the first character
 GetChar();

 PrevIsNewline = true;
 return true;
}
//------------------------------------------------------------------------------

bool SCANNER::GetChar(){
 int e, s, n, j;

 e = s = n = 0;

 Char.Char[0] = 0;

 if(error || Index >= Buffer.Length()){
  Char.Type = tEOF;
  return false;
 }

 // Prevent checking for trailing spaces if the previous character is a
 // non-trailing space
 if(Char.Type == tSpace){
  s = Space();
  if(s){
   for(j = 0; j < s; j++) Char.Char[j] = Buffer[Index++];
   Char.Char[j] = 0;
   Char.Line    = Line;
   Char.Type    = tSpace;
   return true;
  }
 }

 // Check for trailing spaces and escaped newlines
 while(Index < Buffer.Length()){
  if(Buffer[Index] == '\\') e = 1;
  else                      e = 0;
  Index += e;

  s = Spaces (); Index += s;
  n = Newline(); Index += n;

  if(!e || !n){
   Index -= e+s+n;
   break;
  }
  Line++;
 }
 if(Index >= Buffer.Length()){
  Char.Type = tEOF;
  return false;
 }

 if(e){
  Char.Char[0] = Buffer[Index++];
  Char.Char[1] = 0;
  Char.Line    = Line;
  Char.Type    = tOther;
  return true;
 }

 if(n){
  Index += s; // Ignore trailing spaces
  for(j = 0; j < n; j++) Char.Char[j] = Buffer[Index++];
  Char.Char[j] = 0;
  Char.Line    = Line++;
  Char.Type    = tNewline;
  return true;
 }

 if(s){
  s = Space();
  for(j = 0; j < s; j++) Char.Char[j] = Buffer[Index++];
  Char.Char[j] = 0;
  Char.Line    = Line;
  Char.Type    = tSpace;
  return true;
 }

 Char.Char[0] = Buffer[Index++];
 Char.Char[1] = 0;
 Char.Line    = Line;
 Char.Type    = tOther;
 return true;
}
//------------------------------------------------------------------------------

inline bool SCANNER::Digit(){
 return (Char.Char[0] >= '0' && Char.Char[0] <= '9');
}
//------------------------------------------------------------------------------

inline bool SCANNER::HexDigit(){
 return (Char.Char[0] >= '0' && Char.Char[0] <= '9') ||
        (Char.Char[0] >= 'a' && Char.Char[0] <= 'f') ||
        (Char.Char[0] >= 'A' && Char.Char[0] <= 'F');
}
//------------------------------------------------------------------------------

inline bool SCANNER::NonDigit(){
 return (Char.Char[0] >= 'a' && Char.Char[0] <= 'z') ||
        (Char.Char[0] >= 'A' && Char.Char[0] <= 'Z') ||
        (Char.Char[0] == '_' ) ||
        (Char.Char[0] == '\\') ||
        (Char.Char[0] >= 0x80);
}
//------------------------------------------------------------------------------

bool SCANNER::Identifier(STRING& Token){
 if(!NonDigit()) return false;
 Token = (char*)Char.Char;
 GetChar();

 while(Char.Type == tOther && (Digit() || NonDigit())){
  Token << (char*)Char.Char;
  GetChar();
 }
 return true;
}
//------------------------------------------------------------------------------

// Number = (Digit | ("." Digit)) {
//  Digit | NonDigit | "." | "_" | "'"
//  (("e" | "E" | "p" | "P") ["+" | "-"])
// };
bool SCANNER::Number(STRING& Token){
 if(Token.Length()) return false;

 if(Char.Char[0] == '.'){
  Token << (char*)Char.Char;
  GetChar();
 }
 if(!Digit()) return false;
 Token << (char*)Char.Char;
 GetChar();

 bool Exponent = false;
 while(Char.Type == tOther && (
  Digit   ()               ||
  NonDigit()               ||
  Char.Char[0] == '.'      ||
  Char.Char[0] == '_'      ||
  Char.Char[0] == '\''     ||
  (Exponent && (Char.Char[0] == '+' || Char.Char[0] == '-'))
 )){
  if(
   Char.Char[0] == 'e' || Char.Char[0] == 'E' ||
   Char.Char[0] == 'p' || Char.Char[0] == 'P'
  ){
   Exponent = true;
  }else{
   Exponent = false;
  }
  Token << (char*)Char.Char;
  GetChar();
 }
 return true;
}
//------------------------------------------------------------------------------

// FixedPointCast = "`" {Digit | ("." ["-"])};
bool SCANNER::FixedPointCast(STRING& Token){
 if(Char.Char[0] != '`') return false;
 Token << (char*)Char.Char;
 GetChar();

 bool Period = false;
 while(Char.Type == tOther && (
  Digit()             ||
  Char.Char[0] == '.' ||
  (Period && (Char.Char[0] == '-'))
 )){
  if(Char.Char[0] == '.') Period = true;
  else                    Period = false;
  Token << (char*)Char.Char;
  GetChar();
 }
 return true;
}
//------------------------------------------------------------------------------

bool SCANNER::Comment(STRING& Token){
 if(Token.Length()) return false;

 if(Char.Char[0] != '/') return false;
 Token = (char*)Char.Char;
 GetChar();

 if(Char.Char[0] != '/' && Char.Char[0] != '*') return false;
 Token << (char*)Char.Char;

 bool PrevIsStar = false; // Used to terminate multiline comments
 bool Multiline  = Char.Char[0] == '*';

 while(GetChar()){
  if(Multiline){
   if(PrevIsStar && Char.Char[0] == '/'){
    Token << (char*)Char.Char;
    GetChar();
    return true;
   }
  }else{
   if(Char.Type == tNewline){
    Token << (char*)Char.Char;
    GetChar();
    PrevIsNewline = true;
    return true;
   }
  }
  Token << (char*)Char.Char;
  PrevIsStar = Char.Char[0] == '*';
 }
 Error("Incomplete comment");
 return false;
}
//------------------------------------------------------------------------------

bool SCANNER::Character(STRING& Token){
 if(Char.Char[0] != '\'') return false;

 while(GetChar()){
  if(Char.Char[0] == '\\'){
   Token << (char*)Char.Char;
   GetChar();

  }else if(Char.Char[0] == '\''){
   GetChar();
   return true;
  }
  Token << (char*)Char.Char;
 }
 Error("Incomplete character");
 return false;
}
//------------------------------------------------------------------------------

bool SCANNER::String(STRING& Token){
 if(Char.Char[0] != '"') return false;

 while(GetChar()){
  if(Char.Char[0] == '\\'){
   Token << (char*)Char.Char;
   GetChar();

  }else if(Char.Char[0] == '"'){
   GetChar();
   return true;
  }
  Token << (char*)Char.Char;
 }
 Error("Incomplete string");
 return false;
}
//------------------------------------------------------------------------------

bool SCANNER::Operator(STRING& Token){
 STRING Test;

 // At this point, Token could contain '.' or '/' from Number() or Comment()
 if(Token.Length()){
  if(!Operators.GetCode(Token.String())) return false;
  Test << Token;
 }

 // All operators start with valid operators, so use this fact to advantage
 while(Char.Type != tEOF){
  Test << (char*)Char.Char;
  if(!Operators.GetCode(Test.String())) return Token.Length();
  Token << (char*)Char.Char;
  GetChar();
 }
 return Token.Length();
}
//------------------------------------------------------------------------------

bool SCANNER::GetToken(TOKEN* Token){
 Token->Comment.Clear();
 Token->PrecedingSpace = PrevIsNewline;

 // Ignore spaces and comments (but keep comments...)
 while(!error && Char.Type != tEOF){
  Token->Token.Clear();
  Token->Line = Char.Line;

  if(PrevIsNewline && Char.Char[0] == '#'){
   GetChar();
   while(Char.Type == tSpace) GetChar();
   Identifier(Token->Token);
   Token->Type   = tDirective;
   PrevIsNewline = false;
   return true;
  }

  if(Char.Type == tNewline){
   Token->Type = tNewline;
   while(Char.Type == tNewline){ // Concatenate newlines
    GetChar();
    while(Char.Type == tSpace) GetChar(); // Remove leading spaces
   }
   PrevIsNewline         = true;
   Token->PrecedingSpace = true;
   return Char.Type != tEOF;
  }

  if(Char.Type == tSpace){
   while(Char.Type == tSpace) GetChar(); // Concatenate spaces
   Token->PrecedingSpace = true;
   continue; // Restart token scan
  }

  if(Identifier(Token->Token)){
   Token->Type   = tIdentifier;
   PrevIsNewline = false;
   return true;
  }

  if(Character(Token->Token)){
   Token->Type   = tCharacter;
   PrevIsNewline = false;
   return true;
  }
  if(String(Token->Token)){
   Token->Type   = tString;
   PrevIsNewline = false;
   return true;
  }
  if(FixedPointCast(Token->Token)){
   Token->Type   = tFixedPointCast;
   PrevIsNewline = false;
   return true;
  }

  // These start with what could potentially be a punctuator, so don't move them
  if(Number(Token->Token)){
   Token->Type   = tNumber;
   PrevIsNewline = false;
   return true;
  }
  if(Comment(Token->Token)){
   Token->Comment << Token->Token;
   Token->PrecedingSpace = true;
   continue; // Restart token scan
  }

  // Don't move this: it's placement as the last one is important
  if(Operator(Token->Token)){
   Token->Type   = tOperator;
   PrevIsNewline = false;
   return true;
  }

  if(Char.Type != tEOF){
   Token->Token = (char*)Char.Char;
   Token->Type  = tOther;
   GetChar();
   PrevIsNewline = false;
   return true;
  }
 }
 Token->Type = tEOF;
 return false;
}
//------------------------------------------------------------------------------

bool SCANNER::GetDirective(TOKEN* Token){
 Token->Comment.Clear();
 Token->PrecedingSpace = false;

 // Ignore spaces and comments (but keep comments...)
 while(!error && Char.Type != tEOF){
  Token->Token.Clear();
  Token->Line = Char.Line;

  if(PrevIsNewline && Char.Char[0] == '#'){
   GetChar();
   while(Char.Type == tSpace) GetChar();
   Identifier(Token->Token);
   Token->Type   = tDirective;
   PrevIsNewline = false;
   return true;
  }

  if(Char.Type == tNewline){
   Token->Type = tNewline;
   while(Char.Type == tNewline){ // Concatenate newlines
    GetChar();
    while(Char.Type == tSpace) GetChar(); // Remove leading spaces
   }
   PrevIsNewline = true;
   continue;
  }

  if(Char.Type == tSpace){
   while(Char.Type == tSpace) GetChar(); // Concatenate spaces
   continue; // Restart token scan
  }

  // These start with what could potentially be a punctuator, so don't move them
  if(Comment(Token->Token)){
   continue; // Restart token scan
  }

  PrevIsNewline = false;
  GetChar();
 }
 Token->Type = tEOF;
 return false;
}
//------------------------------------------------------------------------------
