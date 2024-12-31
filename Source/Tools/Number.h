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

class Number{
    private:
        bool  moved = false;
        mpq_t real;
        mpq_t imag;

        void round(mpq_t n) const;
        std::string print(const mpq_t n, bool imag = false) const;

    public:
        Number();
        Number(int      i);
        Number(unsigned u);
        Number(bool     b);
        Number(double   r, double i = 0);

        Number(const Number& n);
        Number(Number&& n);
       ~Number();

        void setNumDenom(mpz_t numerator, mpz_t denominator);

        void set_e (); // First 43 terms of the series e = sum(1/n!)
        void set_pi(); // 100th fraction approximation
        void set_i ();

        // Operators
        void set(double r, double i = 0);
        void operator=  (int           i);
        void operator=  (unsigned      u);
        void operator=  (double        d);
        void operator=  (bool          b);
        void operator=  (const Number& n);
        void operator=  (Number&&      n);

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

        void   add(double r, double i = 0);
        void   operator+= (const Number& n);
        Number operator+  (int           i) const;
        Number operator+  (unsigned      u) const;
        Number operator+  (double        d) const;
        Number operator+  (const Number& n) const;

        void   sub(double r, double i = 0);
        void   operator-= (const Number& n);
        Number operator-  (int           i) const;
        Number operator-  (unsigned      u) const;
        Number operator-  (double        d) const;
        Number operator-  (const Number& n) const;

        void   mul(double r, double i = 0);
        void   operator*= (const Number& n);
        Number operator*  (int           i) const;
        Number operator*  (unsigned      u) const;
        Number operator*  (double        d) const;
        Number operator*  (const Number& n) const;

        void   div(double r, double i = 0);
        void   operator/= (const Number& n);
        Number operator/  (int           i) const;
        Number operator/  (unsigned      u) const;
        Number operator/  (double        d) const;
        Number operator/  (const Number& n) const;

        void binScale(int n); // *= 2^n

        void round();

        bool isInt () const; // imag == 0 && real.denominator == 1
        bool isReal() const; // imag == 0

        std::string print() const;
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
