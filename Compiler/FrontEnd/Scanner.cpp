#include "Scanner.h"
//------------------------------------------------------------------------------

SCANNER::SCANNER(){
 Line  = 1;
 Index = 0;
}
//------------------------------------------------------------------------------

SCANNER::~SCANNER(){
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

 char* TempBuffer = fs.Read(Filename);
 if(!TempBuffer) return false;

 Buffer.UseMem(TempBuffer);

 return true;
}
//------------------------------------------------------------------------------

bool SCANNER::Get(CHAR& Char){
 int e, s, n, j;

 if(Index >= Buffer.Length()) return false;

 // Check for trailing spaces and escaped newlines
 while(Index < Buffer.Length()){
  if(Buffer[Index] == '\\') e = 1;
  else                      e = 0;

  Index += e;
  s = Spaces();
  Index += s;
  n = Newline();
  Index -= s;
  Index -= e;

  if(!e || !n) break;

  Line  ++;
  Index += e+s+n;
 }
 if(Index >= Buffer.Length()) return false;

 if(e){
  Char.Char[0] = Buffer[Index++];
  Char.Char[1] = 0;
  Char.Line    = Line;
  Char.Type    = tByte;
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
 Char.Type    = tByte;
 return true;
}
//------------------------------------------------------------------------------
