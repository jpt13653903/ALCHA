/**
- Removes trailing spaces
- Escape new-lines
- Populates line numbers
*/
//------------------------------------------------------------------------------

#ifndef Scanner_h
#define Scanner_h
//------------------------------------------------------------------------------

#include "FileSystem.h"
//------------------------------------------------------------------------------

class SCANNER{
 public:
  enum TYPE{tByte, tSpace, tNewline};

  struct CHAR{
   unsigned char Char[4]; // UTF-8 string
   int           Line;
   TYPE          Type;
  };

 private:
  FILE_SYSTEM fs;

  int      Line;
  unsigned Index;
  STRING   Buffer;

  // These don't modify Index
  int Space  (); // Single space, returns code length of the space
  int Spaces (); // Scans for multiple spaces, returns the number of bytes
  int Newline(); // Single newline, returns code length of the newline

 public:
  SCANNER();
 ~SCANNER();

  bool Open(const char* Filename);

  bool Get(CHAR& Char);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
