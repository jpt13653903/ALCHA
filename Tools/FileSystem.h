#ifndef FileSystem_h
#define FileSystem_h
//------------------------------------------------------------------------------

#include "OS_Header.h"
#include "MyString.h"
//------------------------------------------------------------------------------

class FILE_SYSTEM{
 private:
  void GetLongName(const char   * Path, STRING& LongPath);
  bool CreatePath (      wchar_t* Path);

 public:
  // UTF-8 name; returns null on error; null-terminates the buffer
  char* Read (const char* Filename, unsigned* Filesize = 0);

  // UTF-8 name; also creates the path if it does not exist
  bool Write(const char* Filename, const char* Data, unsigned Size);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
