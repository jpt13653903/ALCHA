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

#ifndef FileSystem_h
#define FileSystem_h
//------------------------------------------------------------------------------

#include "OS_Header.h"
#include "MyString.h"
//------------------------------------------------------------------------------

class FILE_SYSTEM{
 private:
  void GetLongName(const byte   * Path, STRING& LongPath);
  bool CreatePath (      wchar_t* Path);

 public:
  // UTF-8 name; returns null on error; null-terminates the buffer
  byte* Read(const byte* Filename, unsigned* Filesize = 0);

  // UTF-8 name; also creates the path if it does not exist
  // If Size is 0, Data is assumed to be null-terminated
  bool Write(const byte* Filename, const byte* Data, unsigned Size = 0);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
