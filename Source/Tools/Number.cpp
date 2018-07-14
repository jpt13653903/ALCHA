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

NUMBER::NUMBER(int i){
 mpq_init(Real);
 mpq_init(Imag);
 operator= (i);
}
//------------------------------------------------------------------------------

NUMBER::NUMBER(unsigned u){
 mpq_init(Real);
 mpq_init(Imag);
 operator= (u);
}
//------------------------------------------------------------------------------

NUMBER::NUMBER(double d){
 mpq_init(Real);
 mpq_init(Imag);
 operator= (d);
}
//------------------------------------------------------------------------------

NUMBER::NUMBER(NUMBER& n){
 mpq_init(Real);
 mpq_init(Imag);
 operator= (n);
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

void NUMBER::operator= (NUMBER& n){
 mpq_set(Real, n.Real);
 mpq_set(Imag, n.Imag);
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

bool NUMBER::IsInt(){
 // Imag == 0 && Real.Denominator == 1
 return !mpq_cmp_ui(Imag, 0, 1) && !mpz_cmp_ui(mpq_denref(Real), 1);
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
 bool   r = mpq_cmp_ui(Real, 0, 1);
 bool   i = mpq_cmp_ui(Imag, 0, 1);

 char* sr = mpq_get_str(0, 10, Real);
 char* si = mpq_get_str(0, 10, Imag);

 double dr = mpq_get_d(Real);
 double di = mpq_get_d(Imag);

 if(r && i) printf("(%s+%sj) (~(%.18lg+%.18lgj))", sr, si, dr, di);
 else if(i) printf("%sj (~%.18lgj)", si, di);
 else       printf("%s (~%.18lg)"  , sr, dr);
}
//------------------------------------------------------------------------------
