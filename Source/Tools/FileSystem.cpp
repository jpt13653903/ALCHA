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

#include "FileSystem.h"
//------------------------------------------------------------------------------

void FILE_SYSTEM::GetLongName(const byte* Path, STRING& LongName){
  int j;

  // Extend the Path to use long names
  if(Path[1] == ':' && Path[2] == '\\') LongName = "\\\\?\\";

  for(j = 0; Path[j]; j++){
    if(Path[j] == '/') LongName << '\\';
    else               LongName << Path[j];
  }
}
//------------------------------------------------------------------------------

bool FILE_SYSTEM::CreatePath(wchar_t* Path){
  int j, b = 0;
  for(j = 0; Path[j]; j++){
    if(Path[j] == '\\') b = j;
  }
  if(!b) return true;

  Path[b] = 0;
    if(!CreateDirectory(Path, 0)){
      if(GetLastError() == ERROR_ALREADY_EXISTS){
        Path[b] = '\\';
        return true;
      }
      CreatePath(Path);
      if(!CreateDirectory(Path, 0)){
        Path[b] = '\\';
        return false;
      }
    }
  Path[b] = '\\';

  return true;
}
//------------------------------------------------------------------------------

byte* FILE_SYSTEM::Read(const byte* Filename, unsigned* Filesize){
  if(!Filename   ) return 0;
  if(!Filename[0]) return 0;

  STRING LongName;

  GetLongName(Filename, LongName);

  HANDLE Handle = CreateFile(
    LongName.WideString(),
    GENERIC_READ,
    FILE_SHARE_READ,
    0,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    0
  );
  if(Handle == INVALID_HANDLE_VALUE) return 0;

  DWORD Size = GetFileSize(Handle, 0);
  byte* Buffer = new byte[Size+1];
  if(!Buffer){
    CloseHandle(Handle);
    return 0;
  }

  DWORD Actual;
  if(!ReadFile(Handle, Buffer, Size, &Actual, 0)){
    CloseHandle(Handle);
    delete[] Buffer;
    return 0;
  }
  CloseHandle(Handle);

  if(Size != Actual){
    delete[] Buffer;
    return 0;
  }

  Buffer[Size] = 0;

  if(Filesize) *Filesize = Size;
  return Buffer;
}
//------------------------------------------------------------------------------

bool FILE_SYSTEM::Write(const byte* Filename, const byte* Data, unsigned Size){
  if(!Filename   ) return false;
  if(!Filename[0]) return false;

  STRING LongName;

  GetLongName(Filename, LongName);

  // Create path
  wchar_t* Path = (wchar_t*)LongName.WideString();
  if(!CreatePath(Path)) return false;

  // Write the file
  HANDLE Handle = CreateFile(
    LongName.WideString(),
    GENERIC_WRITE,
    0,
    0,
    CREATE_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    0
  );
  if(Handle == INVALID_HANDLE_VALUE) return false;

  // Get size for null-terminated data
  if(!Size){
    for(Size = 0; Data[Size]; Size++);
  }

  DWORD Actual;
  if(!WriteFile(Handle, Data, Size, &Actual, 0)){
    CloseHandle(Handle);
    return false;
  }
  CloseHandle(Handle);

  if(Size != Actual) return false;

  return true;
}
//------------------------------------------------------------------------------
