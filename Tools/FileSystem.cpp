#include "FileSystem.h"
//------------------------------------------------------------------------------

void FILE_SYSTEM::GetLongPath(const char* Path, STRING& LongPath){
 int j;

 // Extend the Path to use long names
 if(Path[1] == ':' && Path[2] == '\\') LongPath << "\\\\?\\";

 for(j = 0; Path[j]; j++){
  if(Path[j] == '/') LongPath << '\\';
  else               LongPath << Path[j];
 }
}
//------------------------------------------------------------------------------

char* FILE_SYSTEM::Read(const char* Filename, unsigned* Filesize){
 if(!Filename   ) return 0;
 if(!Filename[0]) return 0;

 STRING LongPath;

 GetLongPath(Filename, LongPath);

 HANDLE Handle = CreateFile(
  LongPath.WideString(),
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

 STRING LongPath;

 GetLongPath(Filename, LongPath);

 HANDLE Handle = CreateFile(
  LongPath.WideString(),
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

bool FILE_SYSTEM::CreatePath(const char* Path){
 if(!Path   ) return false;
 if(!Path[0]) return false;

 int    j;
 STRING LongPath;

 GetLongPath(Path, LongPath);

 // Create paths
 wchar_t* Paths = (wchar_t*)LongPath.WideString();
 if(!Paths || !Paths[0] || !Paths[1] || !Paths[2] || !Paths[3]) return false;

 j = 0;
 if(Paths[  0] == '\\' && Paths[  1] == '\\' && Paths[3] == '\\') j = 4;
 if(Paths[j+1] == ':'  && Paths[j+2] == '\\') j += 3;
 if(Paths[j  ] == '\\') j++;

 while(Paths[j]){
  if(Paths[j] == '\\'){
   Paths[j] = 0;
    if(!CreateDirectory(Paths, 0)){
     if(GetLastError() != ERROR_ALREADY_EXISTS) return false;
    }
   Paths[j] = '\\';
  }
  j++;
 }
 if(!CreateDirectory(Paths, 0)){
  if(GetLastError() != ERROR_ALREADY_EXISTS) return false;
 }
 return true;
}
//------------------------------------------------------------------------------


