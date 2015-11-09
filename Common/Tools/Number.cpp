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
 /// @todo This needs to be dynamic
 NumLength = 1;
 DenLength = 1;

 Negative    = false;
 Numerator   = new unsigned[1];
 Denominator = new unsigned[1];

 Numerator  [0] = 0;
 Denominator[0] = 1;
}
//------------------------------------------------------------------------------

NUMBER::~NUMBER(){
 delete[] Numerator;
 delete[] Denominator;
}
//------------------------------------------------------------------------------

void NUMBER::operator= (int i){
 if(i < 0){
  Negative     = true;
  Numerator[0] = -i;
 }else{
  Negative     = false;
  Numerator[0] = i;
 }
 Denominator[0] = 1;
}
//------------------------------------------------------------------------------

void NUMBER::operator= (unsigned u){
 Negative       = false;
 Numerator  [0] = u;
 Denominator[0] = 1;
}
//------------------------------------------------------------------------------

void NUMBER::operator*= (unsigned u){
 /// @todo Make this arbitrary length
 Numerator[0] *= u;
}
//------------------------------------------------------------------------------

void NUMBER::operator/= (unsigned u){
 /// @todo Make this arbitrary length
 Denominator[0] *= u;
}
//------------------------------------------------------------------------------

void NUMBER::AddToNumerator(unsigned u){
 /// @todo Make this arbitrary length
 Numerator[0] += u;
}
//------------------------------------------------------------------------------

long double NUMBER::FloatingPoint(){
 int         j;
 long double Num = 0;
 long double Den = 0;

 for(j = NumLength-1; j >= 0; j--) Num = Num*0x1p32 + Numerator  [j];
 for(j = DenLength-1; j >= 0; j--) Den = Den*0x1p32 + Denominator[j];

 if(Negative) return -Num / Den;
 else         return  Num / Den;
}
//------------------------------------------------------------------------------
