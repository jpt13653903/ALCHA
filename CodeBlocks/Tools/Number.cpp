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

#include "Number.h"
//------------------------------------------------------------------------------

NUMBER::NUMBER(){
 mpq_init(Real);
 mpq_init(Imag);
}
//------------------------------------------------------------------------------

NUMBER::~NUMBER(){
 mpq_clear(Real);
 mpq_clear(Imag);
}
//------------------------------------------------------------------------------

void NUMBER::Set(mpz_t Numerator, mpz_t Denominator){
 mpz_set(mpq_numref(Real), Numerator  );
 mpz_set(mpq_denref(Real), Denominator);
 mpq_canonicalize(Real);

 mpq_set_ui(Imag, 0, 1);
}
//------------------------------------------------------------------------------

void NUMBER::operator= (int i){
 mpq_set_si(Real, i, 1);
 mpq_set_ui(Imag, 0, 1);
}
//------------------------------------------------------------------------------

void NUMBER::operator= (unsigned u){
 mpq_set_ui(Real, u, 1);
 mpq_set_ui(Imag, 0, 1);
}
//------------------------------------------------------------------------------

void NUMBER::operator= (double d){
 mpq_set_d (Real, d);
 mpq_set_ui(Imag, 0, 1);
}
//------------------------------------------------------------------------------

void NUMBER::Mul(double r, double i){
 mpq_t a, b, c, d;

 mpq_init(a);
 mpq_init(b);
 mpq_init(c);
 mpq_init(d);

 mpq_set  (a, Real);
 mpq_set  (b, Imag);
 mpq_set_d(c, r   );
 mpq_set_d(d, i   );

 mpq_mul(Real, a, c);
 mpq_mul(Imag, b, d);
 mpq_sub(Real, Real, Imag);

 mpq_mul(Imag, a, d);
 mpq_mul(a   , b, c);
 mpq_add(Imag, Imag, a);

 mpq_clear(a);
 mpq_clear(b);
 mpq_clear(c);
 mpq_clear(d);
}
//------------------------------------------------------------------------------

double NUMBER::GetReal(){
 return mpq_get_d(Real);
}
//------------------------------------------------------------------------------

double NUMBER::GetImag(){
 return mpq_get_d(Imag);
}
//------------------------------------------------------------------------------

void NUMBER::Display(){
 char* s = mpq_get_str(0, 10, Real);

 printf("%s (~%.18lg)", s, mpq_get_d(Real));
 free(s);

 if(mpq_cmp_ui(Imag, 0, 1)){
  s = mpq_get_str(0, 10, Imag);
  printf(" + %sj (~%.18lgj)", s, mpq_get_d(Imag));
  free(s);
 }
}
//------------------------------------------------------------------------------