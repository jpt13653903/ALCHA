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

#include "General.h"
//------------------------------------------------------------------------------

class STRING{
  private:
    unsigned Allocated; // In bytes
    unsigned TheLength; // In bytes
    byte*    TheString; // UTF-8

    bool      Changed;       // Signals that TheWideString must be rebuilt
    uwchar_t* TheWideString; // Local buffer for WideString()

  public:
    STRING();

    STRING(const char   * s);
    STRING(const byte   * s);
    STRING(const wchar_t* s);
    STRING(      STRING & s);

   ~STRING();

    void Clear();

    unsigned Length(); // In bytes

    STRING& operator= (const char   * s);
    STRING& operator= (const byte   * s);
    STRING& operator= (const wchar_t* s);
    STRING& operator= (      STRING & s);

    STRING& operator<<   (char     c);
    STRING& operator<<   (byte     c);
    STRING& operator<<   (wchar_t  c);
    void    Append_UTF_32(unsigned c);

    STRING& operator<< (const char   * s);
    STRING& operator<< (const byte   * s);
    STRING& operator<< (const wchar_t* s);
    STRING& operator<< (      STRING & s);

    STRING& operator<< (int    i);
    STRING& operator<< (float  f);
    STRING& operator<< (double d);

    STRING& Hex(unsigned i);

    byte operator[] (unsigned Index);

    const byte   *     String(); // Internal memory management: do not delete
    const wchar_t* WideString(); // Internal memory management: do not delete

    // Index is in bytes, not characters
    unsigned GetUTF_32(unsigned Index, unsigned* CodeLength = 0);

    // Deletes the current buffer and uses the given buffer instead.
    // Simply uses the pointer -- it does not copy the data.
    void UseMem(byte* s);

    // Returns:
    //  0 when equal
    // -1 when left is a substring of right
    //  1 when right is a substring of left
    // -2 when left is smaller than right
    //  2 when left is greater than right
    int Compare(STRING&     Right);
    int Compare(const char* Right);
    int Compare(const byte* Right);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
