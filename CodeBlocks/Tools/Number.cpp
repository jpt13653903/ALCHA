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
 mpq_init(x);
}
//------------------------------------------------------------------------------

NUMBER::~NUMBER(){
 mpq_clear(x);
}
//------------------------------------------------------------------------------

void NUMBER::Set(mpz_t Numerator, mpz_t Denominator){
 mpz_set(mpq_numref(x), Numerator  );
 mpz_set(mpq_denref(x), Denominator);
 mpq_canonicalize(x);
}
//------------------------------------------------------------------------------

void NUMBER::operator= (int i){
 mpq_set_si(x, i, 1);
}
//------------------------------------------------------------------------------

void NUMBER::operator= (unsigned u){
 mpq_set_ui(x, u, 1);
}
//------------------------------------------------------------------------------

void NUMBER::operator= (double d){
 mpq_set_d(x, d);
}
//------------------------------------------------------------------------------

double NUMBER::FloatingPoint(){
 return mpq_get_d(x);
}
//------------------------------------------------------------------------------

void NUMBER::Display(){
 char* s = mpq_get_str(0, 10, x);

 printf("%s (~%.18lg)", s, mpq_get_d(x));
 free(s);
}
//------------------------------------------------------------------------------
