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

using std::string;
//------------------------------------------------------------------------------

Number::Number()
{
    mpq_init(real);
    mpq_init(imag);
}
//------------------------------------------------------------------------------

Number::Number(int i)
{
    mpq_init(real);
    mpq_init(imag);
    operator= (i);
}
//------------------------------------------------------------------------------

Number::Number(unsigned u)
{
    mpq_init(real);
    mpq_init(imag);
    operator= (u);
}
//------------------------------------------------------------------------------

Number::Number(double d)
{
    mpq_init(real);
    mpq_init(imag);
    operator= (d);
}
//------------------------------------------------------------------------------

Number::Number(bool b)
{
    mpq_init(real);
    mpq_init(imag);
    operator= (b);
}
//------------------------------------------------------------------------------

Number::Number(const Number& n)
{
    mpq_init(real);
    mpq_init(imag);
    operator= (n);
}
//------------------------------------------------------------------------------

Number::Number(int num, int denum)
{
    mpq_init(real);
    mpq_init(imag);
    mpq_set_si(real, num, denum);
    mpq_canonicalize(real);
    mpq_set_ui(imag, 0, 1);
}
//------------------------------------------------------------------------------

Number::~Number()
{
    mpq_clear(real);
    mpq_clear(imag);
}
//------------------------------------------------------------------------------

void Number::set(mpz_t numerator, mpz_t denominator)
{
    mpz_set(mpq_numref(real), numerator  );
    mpz_set(mpq_denref(real), denominator);
    mpq_canonicalize(real);

    mpq_set_ui(imag, 0, 1);
}
//------------------------------------------------------------------------------

void Number::set_e()
{
    mpq_set_ui(real, 2, 1);
    mpq_set_ui(imag, 0, 1);

    mpq_t t, n;
    mpq_init(t);
    mpq_init(n);
    mpq_set_ui(t, 1, 1);

    // 43 terms provide about 52 decimal digits of accuracy.  It was chosen so
    // that the "Display" functions for e and pi result in the same string length
    for(int nn = 2; nn < 43; nn++){
      mpq_set_ui(n, nn, 1);
      mpq_div(t, t, n);
      mpq_add(real, real, t);
    }

    mpq_clear(n);
    mpq_clear(t);
}
//------------------------------------------------------------------------------

// Numerator:   https://oeis.org/A002485/b002485.txt
// Denominator: https://oeis.org/A002486/b002486.txt
void Number::set_pi()
{
    mpq_set_str(real,
                "394372834342725903069943709807632345074473102456264/"
                "125532772013612015195543173729505082616186012726141", 10);
    mpq_canonicalize(real);
    mpq_set_ui(imag, 0, 1);
}
//------------------------------------------------------------------------------

void Number::set_i()
{
    mpq_set_ui(real, 0, 1);
    mpq_set_ui(imag, 1, 1);
}
//------------------------------------------------------------------------------

void Number::operator= (int i)
{
    mpq_set_si(real, i, 1);
    mpq_set_ui(imag, 0, 1);
}
//------------------------------------------------------------------------------

void Number::operator= (unsigned u)
{
    mpq_set_ui(real, u, 1);
    mpq_set_ui(imag, 0, 1);
}
//------------------------------------------------------------------------------

void Number::operator= (double d)
{
    mpq_set_d (real, d);
    mpq_set_ui(imag, 0, 1);
}
//------------------------------------------------------------------------------

void Number::operator= (bool b)
{
    operator= (b ? 1 : 0);
}
//------------------------------------------------------------------------------

void Number::operator= (const Number& n)
{
    mpq_set(real, n.real);
    mpq_set(imag, n.imag);
}
//------------------------------------------------------------------------------

bool Number::operator== (int i) const
{
    if(!isReal()) return false;

    Number n(i);
    return operator== (n);
}
//------------------------------------------------------------------------------

bool Number::operator== (unsigned u) const
{
    if(!isReal()) return false;

    Number n(u);
    return operator== (n);
}
//------------------------------------------------------------------------------

bool Number::operator== (double d) const
{
    if(!isReal()) return false;

    Number n(d);
    return operator== (n);
}
//------------------------------------------------------------------------------

bool Number::operator== (bool b) const
{
    return (operator== (0)) != b;
}
//------------------------------------------------------------------------------

bool Number::operator== (const Number& n) const
{
    if(!mpq_equal(real, n.real)) return false;
    if(!mpq_equal(imag, n.imag)) return false;
    return true;
}
//------------------------------------------------------------------------------

bool Number::operator!= (int i) const
{
    return !operator==(i);
}
//------------------------------------------------------------------------------

bool Number::operator!= (unsigned u) const
{
    return !operator==(u);
}
//------------------------------------------------------------------------------

bool Number::operator!= (double d) const
{
    return !operator==(d);
}
//------------------------------------------------------------------------------

bool Number::operator!= (bool b) const
{
    return !operator==(b);
}
//------------------------------------------------------------------------------

bool Number::operator!= (const Number& n) const
{
    return !operator==(n);
}
//------------------------------------------------------------------------------

bool Number::operator< (int i) const
{
    Number n(i);
    return operator<(n);
}
//------------------------------------------------------------------------------

bool Number::operator< (unsigned u) const
{
    Number n(u);
    return operator<(n);
}
//------------------------------------------------------------------------------

bool Number::operator< (double d) const
{
    Number n(d);
    return operator<(n);
}
//------------------------------------------------------------------------------

#include "General.h"
bool Number::operator< (const Number& n) const
{
    int result = mpq_cmp(real, n.real);
    if(result < 0) return true;
    if(result > 0) return false;
    return mpq_cmp(imag, n.imag) < 0;
}
//------------------------------------------------------------------------------

bool Number::operator<= (int i) const
{
    return operator<(i) || operator==(i);
}
//------------------------------------------------------------------------------

bool Number::operator<= (unsigned u) const
{
    return operator<(u) || operator==(u);
}
//------------------------------------------------------------------------------

bool Number::operator<= (double d) const
{
    return operator<(d) || operator==(d);
}
//------------------------------------------------------------------------------

bool Number::operator<= (const Number& n) const
{
    return operator<(n) || operator==(n);
}
//------------------------------------------------------------------------------

bool Number::operator> (int i) const
{
    return !operator<(i) && !operator==(i);
}
//------------------------------------------------------------------------------

bool Number::operator> (unsigned u) const
{
    return !operator<(u) && !operator==(u);
}
//------------------------------------------------------------------------------

bool Number::operator> (double d) const
{
    return !operator<(d) && !operator==(d);
}
//------------------------------------------------------------------------------

bool Number::operator> (const Number& n) const
{
    return !operator<(n) && !operator==(n);
}
//------------------------------------------------------------------------------

bool Number::operator>= (int i) const
{
    return !operator<(i);
}
//------------------------------------------------------------------------------

bool Number::operator>= (unsigned u) const
{
    return !operator<(u);
}
//------------------------------------------------------------------------------

bool Number::operator>= (double d) const
{
    return !operator<(d);
}
//------------------------------------------------------------------------------

bool Number::operator>= (const Number& n) const
{
    return !operator<(n);
}
//------------------------------------------------------------------------------

void Number::add(double r, double i)
{
    mpq_t a, b;

    mpq_init(a); mpq_set_d(a, r);
    mpq_init(b); mpq_set_d(b, i);

    mpq_add(real, real, a);
    mpq_add(imag, imag, b);

    mpq_clear(a);
    mpq_clear(b);
}
//------------------------------------------------------------------------------

void Number::add(const Number& n)
{
    mpq_add(real, real, n.real);
    mpq_add(imag, imag, n.imag);
}
//------------------------------------------------------------------------------

void Number::sub(double r, double i)
{
    mpq_t a, b;

    mpq_init(a); mpq_set_d(a, r);
    mpq_init(b); mpq_set_d(b, i);

    mpq_sub(real, real, a);
    mpq_sub(imag, imag, b);

    mpq_clear(a);
    mpq_clear(b);
}
//------------------------------------------------------------------------------

void Number::sub(const Number& n)
{
    mpq_sub(real, real, n.real);
    mpq_sub(imag, imag, n.imag);
}
//------------------------------------------------------------------------------

void Number::mul(double r, double i)
{
    mpq_t a, b, c, d;

    mpq_init(a);
    mpq_init(b);
    mpq_init(c);
    mpq_init(d);

    mpq_set  (a, real);
    mpq_set  (b, imag);
    mpq_set_d(c, r   );
    mpq_set_d(d, i   );

    mpq_mul(real, a, c);
    mpq_mul(imag, b, d);
    mpq_sub(real, real, imag);

    mpq_mul(imag, a, d);
    mpq_mul(a   , b, c);
    mpq_add(imag, imag, a);

    mpq_clear(a);
    mpq_clear(b);
    mpq_clear(c);
    mpq_clear(d);
}
//------------------------------------------------------------------------------

void Number::mul(const Number& n)
{
    mpq_t a, b;

    mpq_init(a);
    mpq_init(b);

    mpq_set(a, real);
    mpq_set(b, imag);

    mpq_mul(real, a, n.real);
    mpq_mul(imag, b, n.imag);
    mpq_sub(real, real, imag);

    mpq_mul(imag, a, n.imag);
    mpq_mul(a   , b, n.real);
    mpq_add(imag, imag, a);

    mpq_clear(a);
    mpq_clear(b);
}
//------------------------------------------------------------------------------

void Number::div(double r, double i)
{
    mpq_t a, b, c, d;

    mpq_init(a);
    mpq_init(b);
    mpq_init(c);
    mpq_init(d);

    mpq_set  (a, real);
    mpq_set  (b, imag);
    mpq_set_d(c, r   );
    mpq_set_d(d, i   );

    mpq_mul(real, a, c);
    mpq_mul(imag, b, d);
    mpq_add(real, real, imag);

    mpq_mul(imag, b, c);
    mpq_mul(a   , a, d);
    mpq_sub(imag, imag, a);

    mpq_mul(c, c, c);
    mpq_mul(d, d, d);
    mpq_add(c, c, d);
    mpq_div(real, real, c);
    mpq_div(imag, imag, c);

    mpq_clear(a);
    mpq_clear(b);
    mpq_clear(c);
    mpq_clear(d);
}
//------------------------------------------------------------------------------

void Number::div(const Number& n)
{
    mpq_t a, b, c, d;

    mpq_init(a);
    mpq_init(b);
    mpq_init(c);
    mpq_init(d);

    mpq_set(a,   real);
    mpq_set(b,   imag);
    mpq_set(c, n.real);
    mpq_set(d, n.imag);

    mpq_mul(real, a, c);
    mpq_mul(imag, b, d);
    mpq_add(real, real, imag);

    mpq_mul(imag, b, c);
    mpq_mul(a   , a, d);
    mpq_sub(imag, imag, a);

    mpq_mul(c, c, c);
    mpq_mul(d, d, d);
    mpq_add(c, c, d);
    mpq_div(real, real, c);
    mpq_div(imag, imag, c);

    mpq_clear(a);
    mpq_clear(b);
    mpq_clear(c);
    mpq_clear(d);
}
//------------------------------------------------------------------------------

void Number::binScale(int n)
{
    if(n > 0){
        mpq_mul_2exp(real, real, n);
        mpq_mul_2exp(imag, imag, n);
    }else{
        mpq_div_2exp(real, real, -n);
        mpq_div_2exp(imag, imag, -n);
    }
}
//------------------------------------------------------------------------------

void Number::round()
{
    mpz_t n, d;

    mpz_init(n);
    mpz_init(d);

    mpq_get_num(n, real);
    mpq_get_den(d, real);

    if(mpz_cmp_ui(d, 1)){
      mpz_mul_ui(n, n, 2);
      mpz_add   (n, n, d);
      mpz_mul_ui(d, d, 2);
      mpz_fdiv_q(n, n, d);
      mpz_set_ui(d, 1);

      mpq_set_num(real, n);
      mpq_set_den(real, d);
    }

    if(mpq_cmp_ui(imag, 0, 1)){
      mpq_get_num(n, imag);
      mpq_get_den(d, imag);

      if(mpz_cmp_ui(d, 1)){
        mpz_mul_ui(n, n, 2);
        mpz_add   (n, n, d);
        mpz_mul_ui(d, d, 2);
        mpz_fdiv_q(n, n, d);
        mpz_set_ui(d, 1);

        mpq_set_num(imag, n);
        mpq_set_den(imag, d);
      }
    }

    mpz_clear(n);
    mpz_clear(d);
}
//------------------------------------------------------------------------------

bool Number::isInt() const
{
    return !mpq_cmp_ui(imag, 0, 1) && !mpz_cmp_ui(mpq_denref(real), 1);
}
//------------------------------------------------------------------------------

bool Number::isReal() const
{
    return !mpq_cmp_ui(imag, 0, 1);
}
//------------------------------------------------------------------------------

double Number::getReal() const
{
    return mpq_get_d(real);
}
//------------------------------------------------------------------------------

double Number::getImag() const
{
    return mpq_get_d(imag);
}
//------------------------------------------------------------------------------

string& Number::getString(int base) const
{
    static string result;
    result.clear();

    bool   r = mpq_cmp_ui(real, 0, 1);
    bool   i = mpq_cmp_ui(imag, 0, 1);

    char* sr = mpq_get_str(0, base, real);
    char* si = mpq_get_str(0, base, imag);

    char* s = new char[strlen(sr) + strlen(si) + 0x100];
    if(r && i) sprintf(s, "(%s+%sj)", sr, si);
    else if(i) sprintf(s, "%sj"     , si);
    else       sprintf(s, "%s"      , sr);

    if(base > 10){
      for(int n = 0; s[n]; n++){
        if(s[n] >= 'a' && s[n] <= 'z') s[n] += 'A' - 'a';
      }
    }
    result += s;

    free(sr);
    free(si);

    delete[] s;

    return result;
}
//------------------------------------------------------------------------------

string& Number::display() const
{
    static string result;
    result.clear();

    bool   r = mpq_cmp_ui(real, 0, 1);
    bool   i = mpq_cmp_ui(imag, 0, 1);

    char* sr = mpq_get_str(0, 10, real);
    char* si = mpq_get_str(0, 10, imag);

    double dr = mpq_get_d(real);
    double di = mpq_get_d(imag);

    char* s = new char[strlen(sr) + strlen(si) + 0x100];
    if(r && i) sprintf(s, "(%s+%sj) (~(%.18g+%.18gj))", sr, si, dr, di);
    else if(i) sprintf(s, "%sj (~%.18gj)", si, di);
    else       sprintf(s, "%s (~%.18g)"  , sr, dr);
    result += s;

    free(sr);
    free(si);

    delete[] s;

    return result;
}
//------------------------------------------------------------------------------
