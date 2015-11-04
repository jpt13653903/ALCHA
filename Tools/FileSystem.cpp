#include "FileSystem.h"
//------------------------------------------------------------------------------

void FILE_SYSTEM::GetLongName(const char* Path, STRING& LongName){
 int j;

 // Extend the Path to use long names
 if(Path[1] == ':' && Path[2] == '\\') LongName << "\\\\?\\";

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
   if(GetLastError() == ERROR_ALREADY_EXISTS) return true;
   CreatePath(Path);
   if(!CreateDirectory(Path, 0)) return false;
  }
 Path[b] = '\\';

 return true;
}
//------------------------------------------------------------------------------

char* FILE_SYSTEM::Read(const char* Filename, unsigned* Filesize){
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
 char* Buffer = new char[Size+1];
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

bool FILE_SYSTEM::Write(const char* Filename, const char* Data, unsigned Size){
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
