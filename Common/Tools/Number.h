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

/**
 Abstract data type for arbitrary-precision numbers
*/
//------------------------------------------------------------------------------

#ifndef Number_h
#define Number_h
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdint.h>
//------------------------------------------------------------------------------

class NUMBER{
 private:
  int NumLength; // Number of 32-bit words
  int DenLength; // Number of 32-bit words

  bool      Negative;
  unsigned* Numerator;
  unsigned* Denominator;

 public:
  NUMBER();
 ~NUMBER();

  void operator= (int      i);
  void operator= (unsigned u);

  void MultiplyNumerator  (unsigned u);
  void MultiplyDenominator(unsigned u);
  void AddToNumerator     (unsigned u); // Ignores what the denominator is

  void        Display();
  long double FloatingPoint();
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
