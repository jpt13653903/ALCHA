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

#ifndef MyString_h
#define MyString_h
//------------------------------------------------------------------------------

/** The use of UTF-8 was chosen because:
- Linux file names are in UFT-8, and in rare cases a conversion from UTF-8 to
  UTF-32 and back changes the encoding (when the UTF-8 uses characters in the
  range 0x80 to 0xFF without the proper encoding).
- It is unlikely that Unicode will be encountered in a programming language,
  so the extra space required for UTF-32 representation is a waste of space.
- Except in rare cases, the UTF-8 bytes can be treated as separate characters
  without adverse effects
*/
//------------------------------------------------------------------------------

#include <stdio.h>
#include <wchar.h>
//------------------------------------------------------------------------------

#if WCHAR_MIN < 0
 #define uwchar_t unsigned wchar_t
#else
 #define uwchar_t wchar_t
#endif
//------------------------------------------------------------------------------

class STRING{
 private:
  unsigned       Allocated; // In bytes
  unsigned       TheLength; // In bytes
  unsigned char* TheString; // UTF-8

  bool      Changed;       // Signals that TheWideString must be rebuilt
  uwchar_t* TheWideString; // Local buffer for WideString()

 public:
  STRING();
 ~STRING();

  void Clear();

  unsigned Length(); // In bytes

  STRING& operator<<   (char     c);
  STRING& operator<<   (wchar_t  c);
  void    Append_UTF_32(unsigned c);

  STRING& operator<< (const char   * s);
  STRING& operator<< (const wchar_t* s);
  STRING& operator<< (      STRING & s);

  STRING& operator<< (int    i);
  STRING& operator<< (float  f);
  STRING& operator<< (double d);

  unsigned char operator[] (unsigned Index);

  const char   *     String();
  const wchar_t* WideString();

  // Index is in bytes
  unsigned GetUTF_32(unsigned Index, unsigned* CodeLength = 0);

  // Deletes the current buffer and uses the given buffer instead.
  // Simply uses the pointer -- it does not copy the data.
  void UseMem(char* s);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
