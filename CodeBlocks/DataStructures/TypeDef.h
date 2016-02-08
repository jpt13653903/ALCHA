#ifndef TypeDef_h
#define TypeDef_h
//------------------------------------------------------------------------------

#include "MyString.h"
//------------------------------------------------------------------------------

struct TYPE_DEF;
//------------------------------------------------------------------------------

struct SYMBOL{
 STRING    Name;
 TYPE_DEF* Type;

 SYMBOL* Next; // Linked list used for parameters

 SYMBOL(const byte* Name, TYPE_DEF* Type);
~SYMBOL();
};
//------------------------------------------------------------------------------

struct TYPE_DEF{
 bool Synthesiseable; // compile-time otherwise

 enum TYPE{
  // Compile-time
  Integer,
  Rational,
  Float,
  Complex,

  // Synthesiseable
  Signed,
  Unsigned,

  // Structured Types
  Array,
  Class,

  Function
 } Type;

 union{
  struct{ // Float and Complex
   int FloatPrecision;
  };

  struct{ // Signed and Unsigned
   int IntegerBits;
   int FractionBits;
  };

  struct{ // Array
   TYPE_DEF* BaseType;
   int       Size;
  };

  struct{ // Function
   TYPE_DEF* ReturnType; // Funtion
   SYMBOL*   Parameters; // Linked List
  };
 };
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------


