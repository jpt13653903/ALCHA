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
  Class wrapper for GNU MP rational numbers
*/
//------------------------------------------------------------------------------

#ifndef Number_h
#define Number_h
//------------------------------------------------------------------------------

#include <stdlib.h>
//------------------------------------------------------------------------------

#include "gmp.h"
//------------------------------------------------------------------------------

class NUMBER{
  private:
    mpq_t Real;
    mpq_t Imag;

  public:
    NUMBER();
    NUMBER(int      i);
    NUMBER(unsigned u);
    NUMBER(double   d);
    NUMBER(NUMBER&  n);
   ~NUMBER();

    void Set(mpz_t Numerator, mpz_t Denominator);

    // Operators
    void operator= (int      i);
    void operator= (unsigned u);
    void operator= (double   d);
    void operator= (NUMBER&  n);

    bool operator== (int      i);
    bool operator== (unsigned u);
    bool operator== (double   d);
    bool operator== (NUMBER&  n);

    void Mul(double r, double i = 0);
    void Mul(NUMBER& n);

    bool IsInt ();
    bool IsReal();

    double GetReal();
    double GetImag();

    void Display();
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
