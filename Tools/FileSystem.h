#ifndef FileSystem_h
#define FileSystem_h
//------------------------------------------------------------------------------

#include "OS_Header.h"
#include "MyString.h"
//------------------------------------------------------------------------------

class FILE_SYSTEM{
 private:
  void GetLongPath(const char* Path, STRING& LongPath);

 public:
  // UTF-8 name; returns null on error; null-terminates the buffer
  char* Read (const char* Filename, unsigned* Filesize = 0);

  // UTF-8 name
  bool Write(const char* Filename, const char* Data, unsigned Size);

  // Returns false on error, but true when it already exists
  // Creates from the root up, so can create multiple levels in one go
  bool CreatePath(const char* Path);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
