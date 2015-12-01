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

 // Testing GMP ----------------------------------------------------------------
 mpq_t MyFraction;
 mpq_init(MyFraction);
 mpq_set_ui(MyFraction, 57, 912);
 mpq_canonicalize(MyFraction);
 char* String = mpq_get_str(0, 10, MyFraction);
 printf("MyFraction = %s\n", String);
 free(String);
 mpq_clear(MyFraction);

 // Testing FFTW ---------------------------------------------------------------
 int N = 256;
 fftw_complex *in, *out;
 fftw_plan p;

 in  = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
 out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
 p   = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

 fftw_execute(p); /* repeat as needed */

 fftw_destroy_plan(p);
 fftw_free(in);
 fftw_free(out);

 // Testing MPFR ---------------------------------------------------------------
 mpfr_t s, c;
 mpfr_init2(s, 256);
 mpfr_init2(c, 256);

 mpfr_set_d(s, 355./113./3., MPFR_RNDN); // 60 deg
 mpfr_sin_cos(s, c, s, MPFR_RNDN);

 mpfr_exp_t e;

 String = mpfr_get_str(0, &e, 10, 0, s, MPFR_RNDN);
 printf("sin(60 deg) = 0.%s x10^%d\n", String, (int)e);
 mpfr_free_str(String);

 String = mpfr_get_str(0, &e, 10, 0, c, MPFR_RNDN);
 printf("cos(60 deg) = 0.%s x10^%d\n", String, (int)e);
 mpfr_free_str(String);

 mpfr_clear(s);
 mpfr_clear(c);

 return 0;
 // Done Testing ---------------------------------------------------------------

 PARSER Parser;
// if(!Parser.Run("../../Test Cases\\FrontEnd\\Scanner.alc")) return -1;
// if(!Parser.Run("../../Test Cases\\Target\\Cyclone V GX Starter Kit.ah")){
//  return -1;
// }
// if(!Parser.Run("../../Test Cases\\Target\\BeMicro Max 10.ah")){
//  return -1;
// }
 if(!Parser.Run("../../Test Cases\\Target\\Minimal BeMicro Max 10.ah")){
  return -1;
 }
// if(!Parser.Run("../../Test Cases/Buttons to LEDs/main.alc")){
//  return -1;
// }

// ALTERA Altera;
// if(!Altera.WriteProject("../../Test Cases/Output", "Testing")) return -2;

 return 0;
}
//------------------------------------------------------------------------------
