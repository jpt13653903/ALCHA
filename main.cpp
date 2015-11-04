#include "main.h"
//------------------------------------------------------------------------------

int main(int argc, char** argv){
 #ifdef __WIN32
  SetConsoleOutputCP(CP_UTF8); // UTF-8 console
 #endif

 FILE_SYSTEM fs;

 char*    Buffer;
 unsigned Size;

 if(!(Buffer = fs.Read("../main.cpp", &Size))) return -1;
 if(!fs.Write("../main -- Copy ιν γρεεκ.cpp", Buffer, Size)) return -2;

 return 0;
}
//------------------------------------------------------------------------------
