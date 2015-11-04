#ifndef MyString_h
#define MyString_h
//------------------------------------------------------------------------------

/** The use of UTF-8 was chosen because:
- Linux file names are in UFT-8, and in rare cases a conversion from UTF-8 to
  UTF-32 and back changes the encoding (when the UTF-8 uses high characters,
  such as 0xFF, without the proper encoding).
- It is unlikely that Unicode will be encountered in a programming language,
  so the extra space required for UTF-32 representation is a waste of space.
- Except in rare cases, the UTF-8 bytes can be treated as separate characters
  without adverse effects
*/
//------------------------------------------------------------------------------

#include <stdio.h>
#include <wchar.h>
//------------------------------------------------------------------------------

class STRING{
 private:
  unsigned       Allocated; // In bytes
  unsigned       TheLength; // In bytes
  unsigned char* TheString; // UTF-8

  bool     Changed;       // Signals that TheWideString must be rebuilt
  wchar_t* TheWideString; // Local buffer for WideString()

 public:
  STRING();
 ~STRING();

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

  const char   *     String();
  const wchar_t* WideString();

  // Index is in bytes
  unsigned GetUTF_32(unsigned Index, unsigned* CodeLength = 0);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
