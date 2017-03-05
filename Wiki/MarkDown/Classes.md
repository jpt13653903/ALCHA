[[include repo=code path=Wiki/MarkDown/Header.md]]

[TOC]

# Classes
## Definition and Inheritance

ALCHA supports a simple class structure that has a single constructor form.  The body of the class is the constructor.  Classes can also inherit from other classes, as presented in the example below:

    :::C++
    class Multiplier(A, B){
     auto Y = A * B; // The type of Y is inferred from the types of A and B
    }
    pin'8 A, B;
    Multiplier MyMultiplier = Multiplier(A, B);
    pin'16 Y = MyMultiplier.Y;

    class MultiplyAdd(A, B, C): Multiplier(A, B){
     Y += C; // This is evaluated after the constructor of Multiplier, which declares the type
    }
    pin'8  x, y, z;
    MultiplyAdd MyMultiplyAdd = MultiplyAdd(x, y, z);
    pin'16 w = MyMultiplyAdd.Y;

The constructor parameters is part of the class name, so class inheritance can be used to implement multiple constructors, as follows:

    :::C++
    class Base(N){
     net'N x;
    };
    class Base: Base(8){} // Define a default constructor for class Base

## Instantiation

A variable defined with a class type is actually a reference to that class.  Calling the class, as if it is a function, executes the constructor and returns a reference to a new instance of that class.  The class instance is automatically destroyed when no more references to it exists.  This mechanism enables expressions like:

    :::C++
    class vec4(x, y, z, w){
     num dot(vec4 V){
      dot = V.x*x + V.y*y + V.z*z + V.w*w;
     }
    };
    vec4 A = vec4(1, 2, 3, 4);
    num  B = A.dot(vec4(9, 8, 7, 6));

## Abstract Signal Types

When calling a function, the function name becomes a reference to the target variable, and the parameters are references to the input parameters.  This can be used to define abstractions of streaming processors, for example.  Below is an example of how a digital signal processing chain might be implemented:

    :::C++
    class STREAM(){
     net Data;
     net Valid;
     net Ready;
    }

    STREAM Filter(STREAM Input){
     Filter = STREAM(); // Creates a new stream object

     Input.Ready = Filter.Ready; // Implements back pressure

     rtl(Clk){ // Uses global clock
      if(Filter.Ready && Input.Valid){
       // Implement the body of the filter here, controlling the
       // output data and valid fields as required.
      }
     }
    }

    STREAM Decimate(STREAM Input, num N = 128){
     // Similarly defined as the filter above
    }
    
    STREAM FFT(STREAM Input){
     // Implements a streaming FFT
    }

    // Hook up the chain:
    STREAM Output = FFT(Decimate(Filter(Input)));
     
## Attributes

Whenever a class is instantiated, the instance can be assigned various attributes (in similar fashion to pins and nets).  A typical attribute that might be assigned is the `location`, which indicates where on the FPGA the class instance should be placed.  The exact details of this has not been finalised yet, but it is envisioned that the developer can define a rectangle in normalised coordinates, similar to the example below:

    :::C++
    class SomeModule{
     // Some members
    }
    SomeModule<location = "(0.1, 0.3)->(0.8, 0.5)"> TheInstance;

[[include repo=code path=Wiki/MarkDown/Footer.md]]

