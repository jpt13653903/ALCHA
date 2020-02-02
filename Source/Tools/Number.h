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
#include <string.h>
#include <string>
//------------------------------------------------------------------------------

#include "gmp.h"
//------------------------------------------------------------------------------

class NUMBER{
  private:
    mpq_t Real;
    mpq_t Imag;

  public:
    NUMBER();
    NUMBER(int           i);
    NUMBER(unsigned      u);
    NUMBER(double        d);
    NUMBER(bool          b);
    NUMBER(const NUMBER& n);
   ~NUMBER();

    void Set(mpz_t Numerator, mpz_t Denominator);
    void Set_e (); // First 43 terms of the series e = sum(1/n!)
    void Set_pi(); // 100th fraction approximation
    void Set_i ();

    // Operators
    void operator=  (int           i);
    void operator=  (unsigned      u);
    void operator=  (double        d);
    void operator=  (bool          b);
    void operator=  (const NUMBER& n);

    bool operator== (int           i);
    bool operator== (unsigned      u);
    bool operator== (double        d);
    bool operator== (bool          b);
    bool operator== (const NUMBER& n);

    bool operator!= (int           i);
    bool operator!= (unsigned      u);
    bool operator!= (double        d);
    bool operator!= (bool          b);
    bool operator!= (const NUMBER& n);

    // Relational comparisons for complex numbers follow the rules presented in
    // https://www.cut-the-knot.org/do_you_know/complex_compare.shtml
    // i.e. (a + ib) < (c + id), provided either a < c or a = c and b < d. 
    bool operator<  (int           i);
    bool operator<  (unsigned      u);
    bool operator<  (double        d);
    bool operator<  (const NUMBER& n);

    bool operator<= (int           i);
    bool operator<= (unsigned      u);
    bool operator<= (double        d);
    bool operator<= (const NUMBER& n);

    bool operator>  (int           i);
    bool operator>  (unsigned      u);
    bool operator>  (double        d);
    bool operator>  (const NUMBER& n);

    bool operator>= (int           i);
    bool operator>= (unsigned      u);
    bool operator>= (double        d);
    bool operator>= (const NUMBER& n);

    void Add(double r, double i = 0);
    void Add(const NUMBER& n);

    void Sub(double r, double i = 0);
    void Sub(const NUMBER& n);

    void Mul(double r, double i = 0);
    void Mul(const NUMBER& n);

    void Div(double r, double i = 0);
    void Div(const NUMBER& n);

    void BinScale(int N); // *= 2^N

    void Round();

    bool IsInt     (); // Imag == 0 && Real.Denominator == 1
    bool IsReal    (); // Imag == 0
    bool IsPositive(); // Imag == 0 && Real >= 0

    double GetReal();
    double GetImag();

    std::string& GetString(int Base = 10); // Format "123/456"
    std::string& Display  ();              // Format "123/456 (~0.269737)"
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
