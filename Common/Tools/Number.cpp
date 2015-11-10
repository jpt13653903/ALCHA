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
 int j;
 for(j = 1; j < NumLength; j++) Numerator  [j] = 0;
 for(j = 1; j < DenLength; j++) Denominator[j] = 0;

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
 int j;
 for(j = 1; j < NumLength; j++) Numerator  [j] = 0;
 for(j = 1; j < DenLength; j++) Denominator[j] = 0;

 Negative       = false;
 Numerator  [0] = u;
 Denominator[0] = 1;
}
//------------------------------------------------------------------------------

void NUMBER::MultiplyNumerator(unsigned u){
 if(u == 0){
  operator= (0);
  return;
 }
 if(u == 1){
  return;
 }

 int j;
 int NumBits = 0;
 int uBits   = 0;
 int Test;

 Test  = 0x80000000;
 for(uBits = 32; uBits > 0; uBits--){
  if(u & Test) break;
  Test >>= 1;
 }

 for(j = NumLength-1; j > 0; j--){
  if(Numerator[j] != 0) break;
 }
 Test  = 0x80000000;
 for(NumBits = 32; NumBits > 0; NumBits--){
  if(Numerator[j] & Test) break;
  Test >>= 1;
 }
 NumBits += j*32;

 if(NumBits + uBits > NumLength*32){
  unsigned* Temp = new unsigned[NumLength+1];
  for(j = 0; j < NumLength; j++) Temp[j] = Numerator[j];
  Temp[j] = 0;
  NumLength++;
  delete[] Numerator;
  Numerator = Temp;
 }

 uint64_t i64 = 0;
 unsigned Carry;

 for(j = 0; j < NumLength; j++){
  Carry = i64 >> 32;
  i64   = (uint64_t)Numerator[j] * u + Carry;
  Numerator[j] = i64 & 0xFFFFFFFF;
 }
}
//------------------------------------------------------------------------------

void NUMBER::MultiplyDenominator(unsigned u){
 int j;

 if(u == 0){
  for(j = 1; j < NumLength; j++) Numerator  [j] = 0;
  for(j = 0; j < DenLength; j++) Denominator[j] = 0;

  Negative     = false;
  Numerator[0] = 1;
  return;
 }
 if(u == 1){
  return;
 }

 int DenBits = 0;
 int uBits   = 0;
 int Test;

 Test  = 0x80000000;
 for(uBits = 32; uBits > 0; uBits--){
  if(u & Test) break;
  Test >>= 1;
 }

 for(j = DenLength-1; j > 0; j--){
  if(Denominator[j] != 0) break;
 }
 Test  = 0x80000000;
 for(DenBits = 32; DenBits > 0; DenBits--){
  if(Denominator[j] & Test) break;
  Test >>= 1;
 }
 DenBits += j*32;

 if(DenBits + uBits > DenLength*32){
  unsigned* Temp = new unsigned[DenLength+1];
  for(j = 0; j < DenLength; j++) Temp[j] = Denominator[j];
  Temp[j] = 0;
  DenLength++;
  delete[] Denominator;
  Denominator = Temp;
 }

 uint64_t i64 = 0;
 unsigned Carry;

 for(j = 0; j < DenLength; j++){
  Carry = i64 >> 32;
  i64   = (uint64_t)Denominator[j] * u + Carry;
  Denominator[j] = i64 & 0xFFFFFFFF;
 }
}
//------------------------------------------------------------------------------

void NUMBER::AddToNumerator(unsigned u){
 int j;

 if(
  ((NumLength == 1) &&  (u & 0x80000000)) ||
  ( Numerator[NumLength-1] & 0x80000000 )
 ){
  unsigned* Temp = new unsigned[NumLength+1];
  for(j = 0; j < NumLength; j++) Temp[j] = Numerator[j];
  Temp[j] = 0;
  NumLength++;
  delete[] Numerator;
  Numerator = Temp;
 }

 uint64_t i64 = (uint64_t)Numerator[0] + (uint64_t)u;
 Numerator[0] = i64 & 0xFFFFFFFF;

 for(j = 1; j < NumLength; j++){
  i64 = Numerator[j] + (i64 >> 32);
  Numerator[j] = i64 & 0xFFFFFFFF;
 }
}
//------------------------------------------------------------------------------

void NUMBER::Display(){
 int j;

 if(Negative) printf("-");
 printf("0x");
 for(j = NumLength-1; j >= 0; j--) printf("%08X", Numerator  [j]);
 printf("/0x");
 for(j = DenLength-1; j >= 0; j--) printf("%08X", Denominator[j]);

 printf(" ~ %.18lg", (double)FloatingPoint());
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
