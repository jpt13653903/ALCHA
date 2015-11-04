#include "MyString.h"
//------------------------------------------------------------------------------

STRING::STRING(){
 Allocated    = 4;
 TheLength    = 0;
 TheString    = new unsigned char[Allocated];
 TheString[0] = 0;

 Changed          = false;
 TheWideString    = new wchar_t[1];
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
 Append_UTF_32(c);

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
 unsigned UTF_32;

 for(int j = 0; s[j]; j++){
  #if WCHAR_MAX < 0x10000 // UTF-16
   if(
    ((s[j  ] & 0xFC00) == 0xD800) &&
    ((s[j+1] & 0xFC00) == 0xDC00)
   ){
    UTF_32 =                   s[j++] & 0x3FF ;
    UTF_32 = (UTF_32 << 10) | (s[j  ] & 0x3FF);
    Append_UTF_32(UTF_32);

   }else{
    Append_UTF_32(s[j]);
   }
  #else // UTF-32
   Append_UTF_32(s[j]);
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
  TheWideString = new wchar_t[TheLength+1];

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

 return TheWideString;
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
