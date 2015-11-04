#include "main.h"
//------------------------------------------------------------------------------

int main(int argc, char** argv){
 SetConsoleOutputCP(CP_UTF8); // UTF-8 console

 STRING s, q;
 s << "Hello there\n";
 q << s << L"ανδ σομε γρεεκ\n";
 q << 123 << '\n' << 123.456f << '\n' << 456.789 << '\n' << 123.456789;

 //MessageBox(0, q.WideString(), L"Info", MB_ICONINFORMATION);
 printf("%s\n", q.String());

 return 0;
}
//------------------------------------------------------------------------------
