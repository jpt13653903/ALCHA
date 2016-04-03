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

#include "main.h"
//------------------------------------------------------------------------------

// Used to convert the HTML character names from a JSON file to a C Header
void ConvertJSON(){
 STRING      Out;
 FILE_SYSTEM fs;

 Out << "#ifndef CharacterNames_h\n"
        "#define CharacterNames_h\n"
        "//--------------------------------------"
        "----------------------------------------\n"
        "\n"
        "/**\n"
        "This list is used in the scanner to evaluate "
        "escape sequences of the form\n"
        "\"\\&nnnn;\", where nnnn is any of the 2 125 "
        "HTML-5 character names.             */\n"
        "//--------------------------------------"
        "----------------------------------------\n"
        "\n"
        "static const char* CharacterNames[] = {\n";

 byte* Buffer = fs.Read(
  (byte*)"C:\\JPT\\Projects\\15\\1509 - ALCHA\\CodeBlocks\\FrontEnd\\"
         "CharacterNames.json"
 );

 if(!Buffer){
  printf("Error opening JSON file\n");
  return;
 }

 int    j;
 char   t[8];
 STRING s;

 while(*Buffer){
  if(Buffer[0] == '"' && Buffer[1] == '&'){
   Buffer += 2;
   for(j = 0; Buffer[j] && Buffer[j] != '"'; j++);
   if(!Buffer[j]) break;
   if(Buffer[j-1] == ';'){ // Test for cases not ending in ";"
    Buffer[j-1] = 0;
    Out << " \"" << Buffer << "\", ";
    Buffer += j;
   }else{
    while(Buffer[0] && Buffer[0] != '}') Buffer++;
   }
  }
  if(Buffer[0] == '['){
   Out << '"';
   Buffer++;
   j = 0;
   while(*Buffer){
    switch(Buffer[0]){
     case '0':
     case '1':
     case '2':
     case '3':
     case '4':
     case '5':
     case '6':
     case '7':
     case '8':
     case '9':
      j = 10*j + (Buffer[0] - '0');
      break;

     case ',':
      s.Clear();
      s.Append_UTF_32(j);
      for(j = 0; s[j]; j++){
       sprintf(t, "\\x%02X", (unsigned)s[j]);
       Out << t;
      }
      j = 0;
      break;

     case ']':
      s.Clear();
      s.Append_UTF_32(j);
      for(j = 0; s[j]; j++){
       sprintf(t, "\\x%02X", (unsigned)s[j]);
       Out << t;
      }
      j = 0;
      Out << "\",\n";
      goto Break2;

     default:
      break;
    }
    Buffer++;
   }
   printf("Unexpected end of file");
   return;
  }
  Break2:
  Buffer++;
 }

 Out << " 0, 0\n"
        "};\n"
        "//--------------------------------------"
        "----------------------------------------\n"
        "\n"
        "#endif\n"
        "//--------------------------------------"
        "----------------------------------------\n";

 fs.Write(
  (byte*)"C:\\JPT\\Projects\\15\\1509 - ALCHA\\CodeBlocks\\FrontEnd\\"
         "CharacterNames.h",
  Out.String(),
  Out.Length()
 );
 printf("JSON file converted successfully\n");
}
//------------------------------------------------------------------------------

int main(int argc, char** argv){
 #ifdef __WIN32
  // UTF-8 Console encoding
  SetConsoleOutputCP(CP_UTF8); // UTF-8 console
  // Read the current console size
  CONSOLE_SCREEN_BUFFER_INFO Info;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
  // Make the console buffer as many lines as the system will allow
  Info.dwSize.Y = 0x7FFF;
  while(
   !SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), Info.dwSize)
  ) Info.dwSize.Y--;
 #endif

// ConvertJSON();
 SetCurrentDirectory(
  L"C:\\JPT\\Projects\\15\\1509 - ALCHA\\Test Cases\\Counter"
 );

 PARSER Parser;
 AST = Parser.Run((byte*)"Counter.alc");
 if(!AST) return -1;

// ALTERA Altera;
// if(!Altera.WriteProject()) return -2;

 if(AST) delete AST;
 return 0;
}
//------------------------------------------------------------------------------
