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
- [Classes](Classes.md#classes)
  - [Definition and Inheritance](#definition-and-inheritance)
  - [Constructor](#constructor)
  - [Member Functions](#member-functions)
  - [Instantiation](#instantiation)
  - [Abstract Signal Types](#abstract-signal-types)
  - [Inheritance](#inheritance)
  - [Polymorphism](#polymorphism)
  - [Attributes](#attributes)
- [Operator Overloading](OperatorOverloading.md)
- [Scripting Features](Scripting.md)
- [Advanced Attributes](AdvancedAttributes.md)
- [High-level Structures](HighLevelStructures.md)
- [Simulation and Verification](Simulation.md)

--------------------------------------------------------------------------------

# Classes

## Definition and Inheritance

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
  (optionally followed by contructor parameters)
- Calling the class constructor as if it is a function,
  and assigning the resulting reference to a variable.

A variable defined with a class type is actually a reference to that class.
Calling the class, as if it is a function, executes the constructor and
returns a reference to a new instance of that class.  The class instance is
automatically destroyed when no more references to it exists (similar to
Python).  This mechanism enables expressions such as:

```alcha
    class vec4(x, y, z, w){
        num dot(vec4 V){
            dot = V.x*x + V.y*y + V.z*z + V.w*w;
        }
    };
    vec4(1, 2, 3, 4) A;
    num B = A.dot(vec4(9, 8, 7, 6));

    num C = vec4(1, 2, 3, 4).dot(vec4(9, 8, 7, 6));
```

## Abstract Signal Types

When calling a function, the function name becomes a reference to the target
variable, and the parameters are references to the input parameters.  This can
be used to define abstractions of streaming processors, for instance.  An
example of how a digital signal processing chain might be implemented is
presented below:

```alcha
    class STREAM(num Width = 8){
        net(Width) Data;
        net        Valid;
        net        Ready;
    }

    STREAM Filter(STREAM Input){
        // Creates a new stream object which has double the width of the input.
        Filter = STREAM(Input.Data'width * 2);

        Input.Ready = Filter.Ready; // Implements back pressure

        rtl(Clk){ // Uses global clock
            if(Filter.Ready & Input.Valid){
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
    ADC()  Input;
    STREAM Output = FFT(Decimate(Filter(Input)));
```

## Inheritance

ALCHA classes support multiple inheritance.  The parent classes are specified
by means of a comma-separated list after a colon.  The ADC class used in the
example above might, for instance, be defined as follows:

```alcha
    group<standard = "CMOS", voltage = "2.5 V"> ADC{
        group Data{
            pin    <location = "B7"> Clock = 0; // From FPGA to ADC
            pin(14)<location = ["A8", "C9", "E17", "D11", "G3", "C14", "B16",
                                "B4", "H2", "A25", "E19", "F7", "G21", "A11"]> Data;
        }
        group SPI{
            pin<location = "K7"> Clock = 1; // These initialisers are reset conditions
            pin<location = "L2"> Data  = 1;
            pin<location = "N9"> Latch = 1;
        }
    }

    class ADC(): STREAM(14){ // Inherits from the STREAM class
        void SetRegister(Address, Data){
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
might be assigned is the `location`, which indicates where on the FPGA the
class instance should be placed.  The exact details of this has not been
finalised yet, but it is envisioned that the developer can define a rectangle
in normalised coordinates, similar to the example below:

```alcha
    class SomeModule{
        // Some members
    }
    SomeModule()<region = "X44, Y145, W40, H5"> TheInstance;
```

## The `finally` function

The `finally` method is called automatically at the end of parsing the source,
so that the user does not need to.  This is especially useful for classes that
describe some or other internal structure that is built by the rest of the
modules in the circuit, such as modules that add themselves to the
I<sup>2</sup>C bus or add their registers to the global register
infrastructure.  Once all the code as been interpreted, all the `finally`
functions are called.

--------------------------------------------------------------------------------

