#include "main.h"
//------------------------------------------------------------------------------

int main(int argc, char** argv){
 #ifdef __WIN32
  SetConsoleOutputCP(CP_UTF8); // UTF-8 console
 #endif

 SCANNER Scanner;

 if(!Scanner.Open("../../Test Cases\\FrontEnd\\Scanner.alc")) return -1;

 STRING s;
 SCANNER::CHAR Char;
 while(Scanner.Get(Char)){
  s << (char*)Char.Char;
  printf("%05d: %s\n", Char.Line, Char.Char);
 }
 printf("\n\n%s\n", s.String());

 return 0;
}
//------------------------------------------------------------------------------
