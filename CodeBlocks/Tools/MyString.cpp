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

#include "MyString.h"
//------------------------------------------------------------------------------

STRING::STRING(){
 Allocated    = 4;
 TheLength    = 0;
 TheString    = new byte[Allocated];
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

void STRING::Clear(){
 Changed      = true;
 TheLength    = 0;
 TheString[0] = 0;
}
//------------------------------------------------------------------------------

unsigned STRING::Length(){
 return TheLength;
}
//------------------------------------------------------------------------------

STRING& STRING::operator= (const char* s){
 Clear();
 operator<< ((byte*)s);
 return *this;
}
//------------------------------------------------------------------------------

STRING& STRING::operator= (const byte* s){
 Clear();
 operator<< (s);
 return *this;
}
//------------------------------------------------------------------------------

STRING& STRING::operator= (const wchar_t* s){
 Clear();
 operator<< (s);
 return *this;
}
//------------------------------------------------------------------------------

STRING& STRING::operator= (STRING& s){
 Clear();
 operator<< (s);
 return *this;
}
//------------------------------------------------------------------------------

STRING& STRING::operator<< (char c){
 operator<< ((byte)c);
 return *this;
}
//------------------------------------------------------------------------------

STRING& STRING::operator<< (byte c){
 Changed = true;

 TheString[TheLength++] = c;

 if(TheLength == Allocated){
  Allocated <<= 1;
  byte* Temp = new byte[Allocated];
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
 byte Head = 0x3F; // Active bits in the leading byte
 byte Lead = 0x80; // Leading byte
 byte Cont[6];     // Continuation bytes

 if(c < 0x80){
  operator<<((byte)c);

 }else{
  while(c > Head){ // Doesn't fit in the leading byte
   Cont[j] = 0x80 | (c & 0x3F);
   Lead    = 0x80 | (Lead >> 1);
   Head  >>= 1;
   c     >>= 6;
   j++;
  }
  operator<<((byte)(Lead | c));
  for(j--; j >= 0; j--) operator<<((byte)(Cont[j]));
 }
}
//------------------------------------------------------------------------------

STRING& STRING::operator<< (const char* s){
 for(int j = 0; s[j]; j++) operator<<((byte)s[j]);

 return *this;
}
//------------------------------------------------------------------------------

STRING& STRING::operator<< (const byte* s){
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
 return operator<< ((byte*)s);
}
//------------------------------------------------------------------------------

STRING& STRING::operator<< (float f){
 char s[0x100];
 sprintf(s, "%g", f);
 return operator<< ((byte*)s);
}
//------------------------------------------------------------------------------

STRING& STRING::operator<< (double d){
 char s[0x100];
 sprintf(s, "%lg", d);
 return operator<< ((byte*)s);
}
//------------------------------------------------------------------------------

STRING& STRING::Hex(unsigned i){
 char s[0x100];
 sprintf(s, "%X", i);
 return operator<< ((byte*)s);
}
//------------------------------------------------------------------------------

byte STRING::operator[] (unsigned Index){
 return TheString[Index];
}
//------------------------------------------------------------------------------

const byte* STRING::String(){
 return (byte*)TheString;
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
       byte  Lead;   // The leading byte
 const byte* s = TheString + Index;

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

void STRING::UseMem(byte* s){
 delete TheString;

 for(TheLength = 0; s[TheLength]; TheLength++);

 Changed   = true;
 TheString = (byte*)s;
 Allocated = TheLength+1;
}
//------------------------------------------------------------------------------

int STRING::Compare(STRING& Right){
 return Compare(Right.String());
}
//------------------------------------------------------------------------------

int STRING::Compare(const char* Right){
 return Compare((byte*)Right);
}
//------------------------------------------------------------------------------

int STRING::Compare(const byte* Right){
 unsigned j;

 for(j = 0; TheString[j] && Right[j]; j++){
  if(TheString[j] < (byte)Right[j]) return -2;
  if(TheString[j] > (byte)Right[j]) return  2;
 }
 if(TheString[j] < (byte)Right[j]) return -1;
 if(TheString[j] > (byte)Right[j]) return  1;
 return 0;
}
//------------------------------------------------------------------------------
