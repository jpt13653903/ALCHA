[[include repo=code path=Wiki/MarkDown/Header.md]]

[TOC]

# Classes
## Definition and Inheritance

ALCHA supports a simple class structure that has a single constructor form.

## Constructor

The body of the class is the constructor.  The constructor parameters is part of the class name, so class inheritance can be used to implement multiple constructors, as follows:

    :::C++
    class Base(N){
     net'N x;
    };
    class Base: Base(8){} // Define a default constructor for class Base

## Member Functions

ALCHA classes can take member functions.  These behave the same as any other ALCHA function, except that they have direct access to the class members (instead of accessing members through a class instance).

## Instantiation

A variable defined with a class type is actually a reference to that class.  Calling the class, as if it is a function, executes the constructor and returns a reference to a new instance of that class.  The class instance is automatically destroyed when no more references to it exists (similar to Python).  This mechanism enables expressions like:

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
    class STREAM(num Width = 8){
     net'Width Data;
     net       Valid;
     net       Ready;
    }

    STREAM Filter(STREAM Input){
     // Creates a new stream object which has double the width of the input.
     Filter = STREAM(Input.Data.Width * 2);

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
    ADC Input = ADC();
    STREAM Output = FFT(Decimate(Filter(Input)));

The ADC class used in the example above might, for instance, be defined as below.

    :::C++
    group<standard = "CMOS", voltage = "2.5 V"> ADC{
     group Data{
      pin   <location = "B7"> Clock; // From FPGA to ADC
      pin'14<location = "A8, C9, E17, D11, G3, C14, B16,
                         B4, H2, A25, E19, F7, G21, A11> Data;
     }
     group SPI{
      pin<location = "K7"> Clock;
      pin<location = "L2"> Data;
      pin<location = "N9"> Latch;
     }
    }

    class ADC(): STREAM(14){ // Inherits from the STREAM class
     void SetRegister(auto Address, auto Data){
      fsm(GlobalClock, GlobalReset){
       // A state-machine that implements the ADC's SPI interface.

       // This state-machine can be called externally to the ADC class as 
       // well as from within.
      }
     }

     fsm(GlobalClock, GlobalReset){
      // The state machine used to set up the ADC and receive data.

      // This state machine calls the above SetRegister function in order to
      // set the various ADC registers to default values.
     }
    }

## Inheritance

ALCHA classes support multiple inheritance.  The parent classes are specified by means of a comma-separated list after a colon, as presented in the example below:

    :::C++
    class Multiplier(A, B){
     auto Y = A * B; // The type of Y is inferred from the types of A and B
    }
    pin'8 A, B;
    Multiplier MyMultiplier = Multiplier(A, B);
    pin'16 Y = MyMultiplier.Y;

    class MultiplyAdd(A, B, C): Multiplier(A, B){
     Y += C; // This is evaluated after the constructor of `Multiplier`, 
             // which declares the type of `Y`.
    }
    pin'8  x, y, z;
    MultiplyAdd MyMultiplyAdd = MultiplyAdd(x, y, z);
    pin'16 w = MyMultiplyAdd.Y;

## Polymorphism

References to class instances must be of a type that exists in that instance's inheritance tree.  Any other type-cast will result in a compiler error.  The root of the inheritance tree is defined as the type of the class instance.  The tree branches towards the parent classes, with the original ancestors on the leaves of the tree.

All ALCHA member functions are virtual.  In order to call the function of a specific class in the inheritance tree, the class must be referenced by name.  The dot operator (`.`) is used for this purpose.  This is only possible from within the class.  Any external function call will call the function closest to the root of the inheritance tree.

If there are more than one candidate function, the compiler issues an error.  The user must cast the class to make the call unambiguous.  The tree is traversed towards the root in order to find another candidate, but not back towards the other leaves.

## Attributes

Whenever a class is instantiated, the instance can be assigned various attributes (in similar fashion to pins and nets).  A typical attribute that might be assigned is the `location`, which indicates where on the FPGA the class instance should be placed.  The exact details of this has not been finalised yet, but it is envisioned that the developer can define a rectangle in normalised coordinates, similar to the example below:

    :::C++
    class SomeModule{
     // Some members
    }
    SomeModule<location = "(0.1, 0.3)->(0.8, 0.5)"> TheInstance;

[[include repo=code path=Wiki/MarkDown/Footer.md]]

