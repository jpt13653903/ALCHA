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
 if(!fs.Write("../Testing/Path/Creation/to/many1/many2/many3/many4/many5/levels/main -- Copy ιν γρεεκ.cpp", Buffer, Size)) return -2;
 if(!fs.Write("C:\\Buffer/Testing/Path/Creation/to/many1/many2/many3/many4/many5/levels/main -- Copy ιν γρεεκ.cpp", Buffer, Size)) return -3;
 if(!fs.Write("/Testing/Path/Creation/to/many1/many2/many3/many4/many5/levels/main -- Copy ιν γρεεκ.cpp", Buffer, Size)) return -4;

 return 0;
}
//------------------------------------------------------------------------------
