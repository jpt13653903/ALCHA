#include "MyString.h"
//------------------------------------------------------------------------------

STRING::STRING(){
 Allocated    = 4;
 TheLength    = 0;
 TheString    = new unsigned char[Allocated];
 TheString[0] = 0;

 Changed          = false;
 TheWideString    = new uwchar_t[1];
 TheWideString[0] = 0;
}
//------------------------------------------------------------------------------

STRING::~STRING(){
 delete[] TheString;
 delete[] TheWideString;
}
//------------------------------------------------------------------------------

unsigned STRING::Length(){
 return TheLength;
}
//------------------------------------------------------------------------------

STRING& STRING::operator<< (char c){
 Changed = true;

 TheString[TheLength++] = c;

 if(TheLength == Allocated){
  Allocated <<= 1;
  unsigned char* Temp = new unsigned char[Allocated];
  for(unsigned j = 0; j < TheLength; j++) Temp[j] = TheString[j];
  delete[] TheString;
  TheString = Temp;
 }

 TheString[TheLength] = 0;

 return *this;
}
//------------------------------------------------------------------------------

STRING& STRING::operator<< (wchar_t c){
 Append_UTF_32((uwchar_t)c);

 return *this;
}
//------------------------------------------------------------------------------

void STRING::Append_UTF_32(unsigned c){
 int           j    = 0 ;
 unsigned char Head = 0x3F; // Active bits in the leading byte
 unsigned char Lead = 0x80; // Leading byte
 unsigned char Cont[6];     // Continuation bytes

 if(c < 0x80){
  operator<<((char)c);

 }else{
  while(c > Head){ // Doesn't fit in the leading byte
   Cont[j] = 0x80 | (c & 0x3F);
   Lead    = 0x80 | (Lead >> 1);
   Head  >>= 1;
   c     >>= 6;
   j++;
  }
  operator<<((char)(Lead | c));
  for(j--; j >= 0; j--) operator<<((char)(Cont[j]));
 }
}
//------------------------------------------------------------------------------

STRING& STRING::operator<< (const char* s){
 for(int j = 0; s[j]; j++) operator<<(s[j]);

 return *this;
}
//------------------------------------------------------------------------------

STRING& STRING::operator<< (const wchar_t* s){
 uwchar_t* u = (uwchar_t*)s;

 unsigned UTF_32;

 for(int j = 0; u[j]; j++){
  #if WCHAR_MAX < 0x10000 // UTF-16
   if(
    ((u[j  ] & 0xFC00) == 0xD800) &&
    ((u[j+1] & 0xFC00) == 0xDC00)
   ){
    UTF_32 =                   u[j++] & 0x3FF ;
    UTF_32 = (UTF_32 << 10) | (u[j  ] & 0x3FF);
    Append_UTF_32(UTF_32);

   }else{
    Append_UTF_32(u[j]);
   }
  #else // UTF-32
   Append_UTF_32(u[j]);
  #endif
 }

 return *this;
}
//------------------------------------------------------------------------------

STRING& STRING::operator<< (STRING& s){
 return operator<<(s.String());
}
//------------------------------------------------------------------------------

STRING& STRING::operator<< (int i){
 char s[0x100];
 sprintf(s, "%d", i);
 return operator<< (s);
}
//------------------------------------------------------------------------------

STRING& STRING::operator<< (float f){
 char s[0x100];
 sprintf(s, "%g", f);
 return operator<< (s);
}
//------------------------------------------------------------------------------

STRING& STRING::operator<< (double d){
 char s[0x100];
 sprintf(s, "%lg", d);
 return operator<< (s);
}
//------------------------------------------------------------------------------

const char* STRING::String(){
 return (char*)TheString;
}
//------------------------------------------------------------------------------

const wchar_t* STRING::WideString(){
 if(Changed){
  delete[] TheWideString;
  TheWideString = new uwchar_t[TheLength+1];

  unsigned c, w, t;
  unsigned UTF_32;

  c = 0; w = 0;
  while(c < TheLength){
   UTF_32 = GetUTF_32(c, &t); c += t;
   #if WCHAR_MAX < 0x10000 // UTF-16
    if(UTF_32 > 0xFFFF){
     TheWideString[w++] = 0xD800 | (UTF_32 >>    10);
     TheWideString[w++] = 0xDC00 | (UTF_32 &  0x3FF);
    }else{
     TheWideString[w++] = UTF_32;
    }
   #else // UTF-32
    TheWideString[w++] = UTF_32;
   #endif
  }
  TheWideString[w++] = 0;
 }
 Changed = false;

 return (wchar_t*)TheWideString;
}
//------------------------------------------------------------------------------

unsigned STRING::GetUTF_32(unsigned Index, unsigned* CodeLength){
       unsigned       j;
       unsigned       Bits;   // Valid bits
       unsigned       UTF_32; // The result
       unsigned char  Lead;   // The leading byte
 const unsigned char* s = TheString + Index;

 if((s[0] & 0xC0) == 0xC0){
  j      = 1;
  Lead   = s[0] << 1;
  Bits   = 0x3F; // 6 bits
  UTF_32 = s[0];

  while(Lead & 0x80){
   if((s[j] & 0xC0) != 0x80){ // Invalid code-word
    if(CodeLength) *CodeLength = 1;
    return s[0];
   }
   Bits   = (Bits   << 5) |         0x1F ;
   UTF_32 = (UTF_32 << 6) | (s[j] & 0x3F);
   Lead <<= 1;
   j++;
  }
  if(CodeLength) *CodeLength = j;
  return UTF_32 & Bits;

 }else{
  if(CodeLength) *CodeLength = 1;
  return s[0];
 }
}
//------------------------------------------------------------------------------

bool STRING::IsSpace(unsigned Index, unsigned* CodeLength){
 unsigned UTF_32 = GetUTF_32(Index, CodeLength);

 switch(UTF_32){
  case 0x0009: // Character Tabulation
  case 0x0020: // Space
  case 0x00A0: // No-break Space
  case 0x1680: // Ogham Space Mark
  case 0x2000: // En Quad
  case 0x2001: // Em Quad
  case 0x2002: // En Space
  case 0x2003: // Em Space
  case 0x2004: // Three-per-em Space
  case 0x2005: // Four-per-em Space
  case 0x2006: // Six-per-em Space
  case 0x2007: // Figure Space
  case 0x2008: // Punctuation Space
  case 0x2009: // Thin Space
  case 0x200A: // Hair Space
  case 0x202F: // Narrow No-break Space
  case 0x205F: // Medium Mathematical Space
  case 0x3000: // Ideographic Space

  case 0x180E: // Mongolian Vowel Separator
  case 0x200B: // Zero Width Space
  case 0x200C: // Zero Width Non-joiner
  case 0x200D: // Zero Width Joiner
  case 0x2060: // Word Joiner
  case 0xFEFF: // Zero Width Non-breaking Space
   return true;

  default:
   return false;
 }
 return false;
}
//------------------------------------------------------------------------------

bool STRING::IsNewLine(unsigned Index, unsigned* CodeLength){
 if(TheString[Index] == '\r' && TheString[Index+1] == '\n'){
  if(CodeLength) *CodeLength = 2;
  return true;
 }
 if(TheString[Index] == '\n' && TheString[Index+1] == '\r'){
  if(CodeLength) *CodeLength = 2;
  return true;
 }
 unsigned UTF_32 = GetUTF_32(Index, CodeLength);

 switch(UTF_32){
  case 0x000A: // Line Feed
  case 0x000B: // Vertical Tab
  case 0x000C: // Form Feed
  case 0x000D: // Carriage Return
  case 0x0085: // Next Line
  case 0x2028: // Line Separator
  case 0x2029: // Paragraph Separator
   return true;

  default:
   return false;
 }
 return false;
}
//------------------------------------------------------------------------------
