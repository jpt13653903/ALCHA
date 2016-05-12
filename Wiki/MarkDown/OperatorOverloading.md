[[include repo=code path=Wiki/Markdown/Header.md]]

[TOC]

# Operator Overloading

Operator overloading is handled in similar fashion to C.  It can be used to define abstract signal types, such as complex numbers and floating point implementations.  Operator overloading can be clocked functions, but then they cannot be used in combinational statements.  A simple fixed-point complex number class could be partially implemented as follows:

    :::C++
    class Complex{
     sig'(16, 2^8) x = 0;
     sig'(16, 2^8) y = 0;

     Complex operator+(complex C){
      Complex r;
      r.x = x + real(C);
      r.y = y + imag(C);
      return r;
     }
     Complex operator+(sig X){
      Complex r;
      r.x = x + X;
      r.y = y;
      return r;
     }
     Complex operator+(Complex C){
      Complex r;
      r.x = x + C.x;
      r.y = y + C.y;
      return r;
     }
    }

The pattern continues for other relevant operators.  The `complex` type is a built-in scripting type and will be presented in more detail in the scripting section.

[[include repo=code path=Wiki/Markdown/Footer.md]]

