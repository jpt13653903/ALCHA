[[include repo=code path=Wiki/Markdown/Header.md]]

[TOC]

# Classes
## Definition and Inheritance
ALCHA supports a simple class structure that has a single constructor form.  The body of the class is the constructor.  Classes can also inherit from other classes, as presented in the example below:

    :::C++
    class Multiplier(A, B){
     Y = A * B; // The type of Y is inferred from the types of A and B
    }
    pin'8  A, B;
    pin'16 Y;
    Multiplier(A, B) MyMultiplier;
    Y = MyMultiplier.Y;

    class MultiplyAdd(A, B, C): Multiplier(A, B){
     Y += C; // This is evaluated after the constructor of Multiplier
    }
    pin'8  x, y, z;
    pin'16 w;
    MultiplyAdd(x, y, z) MyMultiplyAdd;
    w = MyMultiplyAdd.Y;

The constructor parameters is part of the class name, so class inheritance can be used to implement multiple constructors, as follows:

    :::C++
    class Base(N){
     sig'N x;
    };
    class Base: Base(8){} // Define a default constructor for class Base

## Attributes
Whenever a class is instantiated, the instance can be assigned various attributes (in similar fashion to signals and pins).  A typical attribute that might be assigned is the `location`, which indicates where on the FPGA the class instance should be placed.  The exact details of this has not been finalised yet, but it is envisioned that the developer can define a rectangle in normalised coordinates, similar to the example below:

    :::C++
    class SomeModule{
     // Some members
    }
    SomeModule<location = "(0.1, 0.3)->(0.8, 0.5)"> TheInstance;

[[include repo=code path=Wiki/Markdown/Footer.md]]

