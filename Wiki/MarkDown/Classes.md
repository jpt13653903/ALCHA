# ALCHA

<img src="https://openclipart.org/download/3850/dchandlr-dchandlr-work.svg" height="70" alt="Work in Progress"/>
The ALCHA project, including the language grammar and, by extension, this
wiki, is under development.  This wiki serves as a documentation of the
project goals and aspirations, which are inherently unstable and subject to
change without notice.

--------------------------------------------------------------------------------

## Table of Contents

- [Introduction](Introduction.md)
- [Lexical](Lexical.md)
- [Grammar](Grammar.md)
- [Modules](Modules.md)
- [Declarations](Declarations.md)
- [Expressions](Expressions.md)
- [Statements](Statements.md)
- [Arrays](Arrays.md)
- [Functions](Functions.md)
- [Synchronous Circuits](SynchronousCircuits.md)
- [Classes](#classes)
  - [Constructor](#constructor)
  - [Member Functions](#member-functions)
  - [Instantiation](#instantiation)
  - [Abstract Signal Types](#abstract-signal-types)
  - [Inheritance](#inheritance)
  - [Polymorphism](#polymorphism)
  - [Attributes](#attributes)
- [Scripting Features](Scripting.md)
- [Advanced Attributes](AdvancedAttributes.md)
- [High-level Structures](HighLevelStructures.md)
- [Simulation and Verification](Simulation.md)

--------------------------------------------------------------------------------

# Classes

ALCHA supports a simple class structure that has a single constructor form.

## Constructor

The body of the class is the constructor.  Any statements within the body of
the class are evaluated within the name-space of the class.  Any declarations
within the class body declare class members, be it nets, scripting variables
or functions.

## Member Functions

ALCHA classes can have member functions.  These behave the same as any other
ALCHA function, except that they have direct access to the class members
(instead of accessing members through a class instance).

## Instantiation

Classes can be instantiated in two ways:

- Use the class name as the type in a definition expression
  (optionally followed by constructor parameters)
- Calling the class constructor as if it is a function,
  and assigning the resulting reference to a variable.

A variable defined with a class type is actually a reference to that class.
Calling the class, as if it is a function, executes the constructor and
returns a reference to a new instance of that class.  The class instance is
automatically destroyed when no more references to it exist (similar to
Python).  This mechanism enables expressions such as:

```alcha
    class vec4(x, y, z, w){
        num dot(vec4 V){
            result = V.x*x + V.y*y + V.z*z + V.w*w;
        }
    };
    vec4(1, 2, 3, 4) A;
    num B = A.dot(vec4(9, 8, 7, 6));

    num C = vec4(1, 2, 3, 4).dot(vec4(9, 8, 7, 6));
```

## Abstract Signal Types

When calling a function, the `result` keyword becomes a reference to the
target variable, and the parameters are references to the input parameters.
This can be used to define abstractions of streaming processors, for
example.  An example of how a digital signal processing chain might be
implemented is presented below:

```alcha
    class Stream(num width = 8){
        net(width) data;
        net        valid;
        net        ready;
    }

    Stream filter(Stream stream){
        // Creates a new stream object which has double the width of the input.
        result = Stream(stream.data'width * 2);

        stream.ready = result.ready; // Implements back pressure

        rtl(ipClk){ // Uses global clock
            if(result.ready & stream.valid){
                // Implement the body of the filter here, controlling the
                // output data and valid fields as required.
            }
        }
    }

    Stream decimate(Stream stream, num N = 128){
        // Similarly defined as the filter above
    }

    Stream fft(Stream stream){
        // Implements a streaming FFT
    }

    // Hook up the chain:
    ADC() adc;
    adc.outputStream;

    Stream outputStream = fft(decimate(filter(adc)));
```

This can also be rewritten with dot-syntax, but all processing steps must be
member functions of the `Stream` class:

```alcha
    class Stream(num width = 8){
        net(width) data;
        net        valid;
        net        ready;

        Stream filter(){
            // Creates a new stream object which has double the width of the input.
            result = Stream(stream.data'width * 2);

            stream.ready = result.ready; // Implements back pressure

            rtl(ipClk){ // Uses global clock
                if(result.ready & stream.valid){
                    // Implement the body of the filter here, controlling the
                    // output data and valid fields as required.
                }
            }
        }

        Stream decimate(num N = 128){
            // Similarly defined as the filter above
        }

        Stream fft(Stream stream){
            // Implements a streaming FFT
        }
    }

    // Hook up the chain:
    ADC() adc;

    Stream outputStream = adc.filter  ()
                             .decimate()
                             .fft     ();
```

## Inheritance

ALCHA classes support multiple inheritance.  The parent classes are specified
by means of a comma-separated list after a colon.  The ADC class used in the
example above might, for instance, be defined as follows:

```alcha
    group<standard = "CMOS", voltage = "2.5 V"> ADC{
        group Data{
            pin    <location = "B7"> opClock = 0; // From FPGA to ADC
            pin(14)<location = ["A8", "C9", "E17", "D11", "G3", "C14", "B16",
                                "B4", "H2", "A25", "E19", "F7", "G21", "A11"]> ipData;
        }
        group SPI{
            pin<location = "K7"> opClock = 1; // These initialisers are reset conditions
            pin<location = "L2"> opData  = 1;
            pin<location = "N9"> opLatch = 1;
        }
    }

    class ADC(): Stream(14){ // Inherits from the Stream class
        void setRegister(address, data){
            fsm(globalClock, globalReset){
                // A state-machine that implements the ADC's SPI interface.

                // This state-machine can be called externally to the ADC class as
                // well as from within.
            }
        }

        fsm(globalClock, globalReset){
            // The state machine used to set up the ADC and receive data.

            // This state machine calls the above setRegister function in
            // order to set the various ADC registers to default values.
        }
    }
```

## Polymorphism

References to class instances must be of a type that exists in that instance's
inheritance tree.  Any other type-cast will result in a compiler error.  The
root of the inheritance tree is defined as the type of the class instance.
The tree branches towards the parent classes, with the original ancestors on
the leaves of the tree.

All ALCHA member functions are virtual.  In order to call the function of a
specific class in the inheritance tree, the class must be referenced by name.
The dot operator (`.`) is used for this purpose.

If there are more than one candidate function, the compiler issues an error.
The user must cast the class to make the call unambiguous.  The tree is
traversed towards the root in order to find another candidate, but not back
towards the other leaves.

## Attributes

Whenever a class is instantiated, the instance can be assigned various
attributes (in similar fashion to pins and nets).  A typical attribute that
might be assigned is the `region`, which indicates where on the FPGA the
class instance should be placed.  The exact details of this has not been
finalised yet, but it is envisioned that the developer can define a rectangle
in normalised coordinates, similar to the example below:

```alcha
    class SomeModule{
        // Some members
    }
    SomeModule() <region = "X44, Y145, W40, H5"> theInstance;
```

## The `finally` function

The `finally` method is called automatically at the end of interpreting the
source, so that the user does not need to.  This is especially useful for
classes that describe some or other internal structure that is built by the
rest of the modules in the circuit, such as modules that add themselves to the
I<sup>2</sup>C bus or add their registers to the global register
infrastructure.  Once all the code as been interpreted, all the `finally`
functions are called.

--------------------------------------------------------------------------------

