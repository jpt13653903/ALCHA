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

// #include "gmp.h"
typedef double mpq_t;
typedef double mpz_t;
//------------------------------------------------------------------------------

class Number{
    private:
        mpq_t real;
        mpq_t imag;

    public:
        Number();
        Number(int           i);
        Number(unsigned      u);
        Number(double        d);
        Number(bool          b);
        Number(const Number& n);
      ~Number();

        void set(mpz_t numerator, mpz_t denominator);
        void set_e (); // First 43 terms of the series e = sum(1/n!)
        void set_pi(); // 100th fraction approximation
        void set_i ();

        // Operators
        void operator=  (int           i);
        void operator=  (unsigned      u);
        void operator=  (double        d);
        void operator=  (bool          b);
        void operator=  (const Number& n);

        bool operator== (int           i) const;
        bool operator== (unsigned      u) const;
        bool operator== (double        d) const;
        bool operator== (bool          b) const;
        bool operator== (const Number& n) const;

        bool operator!= (int           i) const;
        bool operator!= (unsigned      u) const;
        bool operator!= (double        d) const;
        bool operator!= (bool          b) const;
        bool operator!= (const Number& n) const;

        // Relational comparisons for complex numbers follow the rules presented in
        // https://www.cut-the-knot.org/do_you_know/complex_compare.shtml
        // i.e. (a + ib) < (c + id), provided either a < c or a = c and b < d.
        bool operator<  (int           i) const;
        bool operator<  (unsigned      u) const;
        bool operator<  (double        d) const;
        bool operator<  (const Number& n) const;

        bool operator<= (int           i) const;
        bool operator<= (unsigned      u) const;
        bool operator<= (double        d) const;
        bool operator<= (const Number& n) const;

        bool operator>  (int           i) const;
        bool operator>  (unsigned      u) const;
        bool operator>  (double        d) const;
        bool operator>  (const Number& n) const;

        bool operator>= (int           i) const;
        bool operator>= (unsigned      u) const;
        bool operator>= (double        d) const;
        bool operator>= (const Number& n) const;

        void add(double r, double i = 0);
        void add(const Number& n);

        void sub(double r, double i = 0);
        void sub(const Number& n);

        void mul(double r, double i = 0);
        void mul(const Number& n);

        void div(double r, double i = 0);
        void div(const Number& n);

        void binScale(int N); // *= 2^N

        void round();

        bool isInt () const; // imag == 0 && real.denominator == 1
        bool isReal() const; // imag == 0

        double getReal() const;
        double getImag() const;

        std::string& getString(int base = 10) const; // Format "123/456"
        std::string& display  () const;              // Format "123/456 (~0.269737)"
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
