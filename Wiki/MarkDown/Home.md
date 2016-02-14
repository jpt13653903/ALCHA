[[img src=https://openclipart.org/download/3850/dchandlr-dchandlr-work.svg height="70"]]
The ALCHA project, including the language grammar and, by extension, this wiki, is under active development.  Until the first stable prototype compiler is published, the details of the language are subject to change without notice.  After first prototype publication, a detailed change log will be kept.

[TOC]

# 1. Introduction and Overview
ALCHA (architectural level computational hardware abstraction) is intended to be implemented as a command-line tool to generate vendor-specific FPGA projects from largely vendor-neutral text-based source code.  The image below provides a rough overview of the intended tool-chain.

<center markdown>[[img src=https://sourceforge.net/p/alcha/doccode/ci/master/tree/Figures/ConceptBlock.svg?format=raw]]</center>

ALCHA is aimed at unifying the various aspects of FPGA firmware design (RTL design, finite state machines, timing and design constraints, as well as scripting) into a single language.

ALCHA is designed to be as portable, concise and expressive as possible, without the loss of low-level control. It provides a development platform that promises reduced development time and maintenance effort.

Visit [EBNF](/p/alcha/wiki/EBNF) for a formal specification of the language grammar.

# 2. Definitions
## 2.1 Identifiers
Non-digits in ALCHA are defined as the '\_' character, as well as any character in the ranges 'a' to 'z', 'A' to 'Z' and any Unicode character above U+7F, as long as it is not a white-space or newline character.

Digits in ALCHA are defined as any character in the range '0' to '9'.

Identifiers start with a non-digit, which may then be followed with any digit or non-digit.

## 2.2 Target Descriptor
Every ALCHA design must have a target descriptor.  There are two types: simulation and project.  A simulation target indicates a waveform database of all the pins in the design.  A project target indicates a vendor-specific project, including the project file, design constraints and HDL source.

To specify a target, the *target* keyword is used, as follows:

    :::C++
    target<
     type = "Simulation",
     time = "100 ms"
    >

Or, in the case of a project target:

    :::C++
    target<
     type   = "Project",
     vendor = "Altera",
     series = "Cyclone V GX",
     device = "5CGXFC5C6F27C7N",
     board  = "Cyclone V GX Starter Kit"
    >

After definition, the target attributes can be accessed by means of the *target* object and used in conditional compilation, as follows:

    :::C++
    if(target.type == "Simulation"){
     // Some simulation-specific code
    }else if(target.vendor == "Altera"){
     // Some Altera-specific code
    }else if(target.vendor == "Xilinx"){
     // Some Xilinx-specific code
    }

In order to promote portability, the attribute values should be standardised within a developer community.

## 2.3 Signals
### 2.3.1 Definition
Signals relate to physical wires and / or registers on the FPGA.  They are specified by means of the *sig* keyword.  If no format is specified, a signal is a single-bit unsigned integer, which also represents a boolean.  The format is specified by means of a "fixed-point cast" using the "'" (grave accent / back-tick) operator.  An optional *signed* modifier can be used to define 2's compliment signed numbers.  Some examples are shown below:

    :::C++
    sig          a; // Single-bit unsigned integer
    sig'8        b; // 8-bit unsigned integer
    signed sig'8 c; // 8-bit signed integer

    sig'(4,4)    d; // 8-bit unsigned fixed point with 4 integer bits
                    // and 4 fraction bits
    sig'(4,-4)   e; // 4-bit integer where the least significant bit
                    // represents 16 and the most significant bit 128.
    sig'(-4,4)   f; // 4-bit fixed-point where the least significant bit
                    // represents 2^(-8) and the most significant 2^(-5).
    sig'(-4,-4)  g; // Illegal: both cannot be negative.

### 2.3.2 Attributes
Signals take optional attributes, which are summarised in the table below:

Attribute     | Default | Description
---------     | ------- | -----------
global        | False   | Indicates whether or not the signal should be routed on the global clock network.  Takes only "True" or "False" values.
output\_clock | None    | Specifies the clock used for external setup and hold constraints.
setup         | 0       | Specifies the minimum setup time of an external register, in relation to "clock"
hold          | 0       | Specifies the minimum hold time of an external register, in relation to "clock"
input\_clock  | None    | Specifies the clock used for external delay constraints.
min\_delay    | 0       | Specifies the minimum output delay of an external register, in relation to "clock"
max\_delay    | 0       | Specifies the maximum output delay of an external register, in relation to "clock"

### 2.3.3 Timing Constraints
As indicated in the table above, signal attributes can be used to specify external peripheral timing requirements.  These timing attributes are specified as if there are no PCB or trace delays.  When the signal is connected to a pin, the trace delays and pin capacitance specified by the pin attributes are used in conjunction with the signal timing attributes to calculate the appropriate external timing constraints.  An example is provided below:

    :::C++
    // PCB trace delays in the pin definitions:
    pin'4<voltage   = "3.3 V" , capacitance = "10 pf",
          min_delay = "500 ps", max_delay   =  "1 ns"
          number    = "U7, T7, V8, T8"> SD_DAT;

    // Peripheral specifications in the driver class:
    sig'4< input_clock = "!Clock", max_delay = "14 ns"          > Data_in;
    sig'4<output_clock =  "Clock", setup = "5 ns", hold = "5 ns"> Data_out;

    // Assign the signals to the pin
    Data_in = SD_DAT;
    SD_DAT  = Data_out;

External delays are always referenced to a clock, even if the external delay is purely combinational.  This is so that the timing constraints are compatible with the Synopsis Design Constraints format, which is the industry-preferred standard.

### 2.3.4 Bus Connections
ALCHA does not support high-impedance signals directly.  Pins can be set to high-impedance by disabling the driver (see the Pins section for details).  In order to emulate a bus that has a set of tri-state drivers, the signals should be gated through AND gates and then combined through an OR gate to drive the bus.

## 2.4 Pins
### 2.4.1 Definition
Pins in ALCHA are specified by means of the *pin* keyword.  Pins can have various attributes.  A short example is presented below:

    :::C++
    in  pin<frequency = "50 MHz", voltage = "2.5 V", location = "H12"> Clock;
    in  pin<                      voltage = "1.2 V", location = "P11"> Key;
    out pin<                      voltage = "2.5 V", location = "F7" > LED;

If the *in* or *out* keyword is absent, the pin is bidirectional.

### 2.4.2 Attributes
The table below summarises pin attributes and their default values:

Attribute   | Default | Description
---------   | ------- | -----------
location    | None    | The physical pin number on the target device.  In the case of a differential pair, the pin location is specified in "PP-NN" format.
voltage     | 3.3 V   | The voltage of the I/O bank.
current     | 5 mA    | The intended maximum current of the pin, in case of an output.
capacitance | 10 pF   | The load capacitance of the pin, in case of an output.
standard    | LVCMOS  | The logic standard of the pin.
min\_delay  | 0       | The minimum physical trace delay of the pin.
max\_delay  | 0       | The maximum physical trace delay of the pin.
frequency   | None    | In the case of a clock input, the frequency of the clock.
jitter      | 0       | In the case of a clock input, the clock jitter.

The unit is important when specifying attributes.  If the unit is absent, a compilation error occurs.  Any SI unit prefix is valid, including *T*, *G*, *M*, *k*, *m*, *u*, *&mu;*, *n*, *p* and *f*.

### 2.4.3 Pin Vectors
When specifying pin locations for bit-vectors or pin arrays, the *location* attribute contains a comma-separated list of pins.  Pin locations are specified most-significant bit first, as follows:

    :::C++
    out pin'8<location = "H9, H8, B6, A5, E9, D8, K6, L7"> LED;

Pin arrays are handled in similar fashion.  The first (0-index) pin location(s) are specified first, then the next (1-index), etc.  All the locations are comma-separated.

### 2.4.4 High-impedance Pins
Output and bidirectional pins have a built-in *enable* property, which represents the "driver-enable" wire.  Output pin drivers are enabled by default, whereas bidirectional pin drivers are disabled by default.

## 2.5 Groups
Often there are attributes that applies to many objects.  In this case, the definitions can be grouped.  All child definitions inherit the attributes of the group.  When a child definition includes an attribute that is already defined in the group, the child definition takes precedence.  Below is an example of a named group for pin definitions.

    :::C++
    group<voltage   = "3.3 V" , capacitance = "10 pF",
          min_delay = "500 ps", max_delay = "1 ns"> SD{
     out pin  <location = "AB6"           > CLK;
         pin  <location = "W8"            > CMD;
         pin'4<location = "U7, T7, V8, T8"> DAT;
    }

In this case, the pins that are actually defined are "SD.CLK", "SD.CMD" and "SD.DAT".  The group can also be anonymous, as given below:

    :::C++
    group<voltage  = "3.3 V", frequency = "50 MHz">{
     in pin<location = "R20"> CLOCK_B5B;
     in pin<location = "N20"> CLOCK_B6A;
    }

Signals, class instances, derived clocks, etc. can be grouped in similar fashion.

## 2.6 Classes
### 2.6.1 Definition and Inheritance
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

class Base(N){
 sig'N x;
};
class Base: Base(8){} // Define a default constructor for class Base

### 2.6.2 Attributes
Whenever a class is instantiated, the instance can be assigned various attributes (in similar fashion to signals and pins).  A typical attribute that might be assigned is the *location*, which indicates where on the FPGA the class instance should be placed.  The exact details of this has not been finalised yet, but it is envisioned that the developer can define a rectangle in normalised coordinates, similar to the example below:

    :::C++
    class SomeModule{
     // Some members
    }
    SomeModule<location = "(0.1, 0.3)->(0.8, 0.5)"> TheInstance;

## 2.7 Literals
ALCHA literals are all stored as infinite-precision rational numbers (by means of the [GNU MP library](https://gmplib.org/)). An imaginary literal can be specified by using either a "j" or "i" postfix.  Literals can be binary ("0b" prefix), octal ("0o" prefix), hexadecimal ("0x" prefix) or decimal (no prefix).  Decimal numbers have an optional decimal exponent by means of an "e" postfix.  Binary, octal, decimal and hexadecimal numbers have an optional binary exponent by means of a "p" postfix.  All literals can be cast to a fixed-point format by means of the "'" operator.  Within numerical literals, the underscore character ("\_") is ignored.  Character literals are string literals with only one character.  Below are some examples:

    :::C++
    123.456      // decimal constant
    123.456'(8,8)  // Decimal constant of fixed-point 8.8 type
    0b101.0101   // Binary constant
    0o123.456    // Octal constant
    0xABC.DEF    // Hexadecimal constant
    123_456_e7   // Decimal constant with decimal exponent (10^7)
    123_456_p7   // Decimal constant with binary exponent (2^7)
    0x123_ABC_p7 // Hexadecimal constant with binary exponent (2^7)
    7.3 + 8.9j   // Decimal complex constant
    "&ohm;"          // Unicode character constant
    "ABC &alpha;&beta;&gamma;"    // Unicode string literal (character array)

Certain non-printable characters can be used, within character and string literals, by means of escaping.  ALCHA escape sequences are the same as C escape sequences.  The following table provides a summary:

Sequence    | Character
--------    | ---------
\\a         | Alarm (\\x07)
\\b         | Backspace (\\x08)
\\f         | Form-feed (\\x0C)
\\n         | New-line (\\x0A)
\\r         | Carriage return (\\x0D)
\\t         | Horizontal tab (\\x09)
\\v         | Vertical tab (\\x0B)
\\\\        | Backslash (\\x5C)
\\'         | Single quotation mark (\\x27)
\\"         | Double quotation mark (\\x22)
\\?         | Question mark (\\x3F)
\\nnn       | up to 11-digit octal Unicode
\\xHH       | 2-digit hexadecimal Unicode
\\uHHHH     | 4-digit hexadecimal Unicode
\\UHHHHHHHH | 8-digit hexadecimal Unicode
\\&amp;nnnn;| Named character, where nnnn is any of the [HTML-5 character names](https://w3.org/TR/html5/syntax.html#named-character-references)

## 2.8 Import and Name-space
Conceptually, ALCHA processes the source as a whole. It is convenient, however, to break the source into a hierarchical structure of files, as presented in the figure below.  In the context of ALCHA, a "module" refers to a source file, rather than an HDL module. An ALCHA module might or might not compile to a corresponding HDL module, depending on the source contents.

<center markdown>[[img src=https://sourceforge.net/p/alcha/doccode/ci/master/tree/Figures/ProgrammingModel.svg?format=raw]]</center>

The import statement is used to import a child module into a parent module. There are two forms, as shown below. In both forms, the objects defined within the child module are in their own name-space, but have direct access to objects in the parent module name-space.

    :::Python
    import "UART"
    import "Ethernet" as Eth

In this example, the UART module is imported anonymously, whereas the Ethernet module is imported into the "Eth" name-space.  Name-spaces are stored on a stack and the module being evaluated is always on top of the stack. Each anonymously imported module adds a level to the stack, whereas the name of a named import resides in the parent module name-space.  It is illegal to import two modules into the same name within the same module.

This name-space model allows the developer to define an environment in a parent module. This environment is visible to all child modules, without the need of ports or parameter passing. The target and pin definitions are examples of where this is useful. The developer might, for instance, define a *Round* function, which is used by child modules whenever signals must be rounded. The rounding behaviour of the design can then be changed by simply modifying the behaviour of this *Round* function.

Specifying the class instance, or name-space, can become tedious. Instead, the developer can temporarily push a name-space onto the name-space stack by means of a special syntax, as shown below. The A.{ construct pushes the A name-space onto the stack, whereas the B.{ construct pushes the B name-space. The closing curly braces pop the name-spaces from the stack.

    :::C++
    class S1{sig'8 r, g, b, a;}
    class S2{sig'8 x, y, z, w;}
    S1 A;
    S2 B;

    // Instead of saying:
    A.r:A.g:A.b:A.a = B.x:B.y:B.z:B.w + 1234;

    // Say:
    A.{
     B.{
      r:g:b:a := x:y:z:w + 1234;
     }
    }

# 3. Combinational Statements
Combinational statements in ALCHA are evaluated in order.  In terms of HDL, the statements are considered blocking.

Assignments come in two flavours.  Normal assignments use the "=" operator and take the fixed-point format into account, whereas raw assignments use the ":=" operator and assigns the raw bits only.

During normal assignment, the source is type-cast to the target type before assignment.  If the target does not have sufficient bits to store the source, the source is truncated.  When most-significant bits are removed, the compiler issues a warning.  When least-significant bits are removed, no warning is issued.  It is up to the developer to ensure that correct rounding is applied, if required.

It the target of an assignment has not been defined, it is dynamically defined according to the type of the source of that assignment.  Once defined in such a manner, that variable cannot change its type, and redefining it is illegal.

## 3.1 Operators
The following table summarises ALCHA operators, in order of precedence:

Group | Operator | Function
----- | -------- | --------
Group | (...)    | Grouping
||
Post-fix | A++    | Increment after use
         | A--    | Decrement after use
         | A.B    | Access B in namespace (or class instance) A
         | A(B)   | Call function A with parameter(s) B
         | A\[B\] | Slice array A with elements in array B
||
Unary | -A | Negate
      | ~A | Bitwise NOT
||
Array | @{A, B, C} | Concatenate arrays A, B and C
      | A -> B     | Builds an array of elements from A to B, inclusive
      | A -> B # C | Builds an array of elements from A to B, in steps of C
||
Reduction |  &A      | AND-reduce
          | ~&A      | NAND-reduce
          |  &#124;A | OR-reduce
          | ~&#124;A | NOR-reduce
          |  ^A      | XOR-reduce
          | ~^A      | XNOR-reduce
          |  !A      | Logical compliment
||
Cast | A ' N    | Casts A to an N-bit integer
     | A '(N,M) | Casts A to an N.M-bit fixed-point
||
Concatenate | A:B:C | Bit-wise concatenate
||
Replicate   | A \ B | Bit-wise replicate A, B times
||
Arithmetic | A * B | Multiply
           | A / B | Divide
           | A % B | Modulus
||
           | A + B  | Add
           | A - B  | Subtract
||
Shift      | A << B | Shift A left by B bits
           | A >> B | Shift A right by B bits
||
Relational | A > B  | Greater than
           | A < B  | Less than
           | A >= B | Greater than or equal to
           | A <= B | Less than or equal to
||
           | A == B | Equal to
           | A != B | Not equal to
||
Bit-wise | A & B      | Bit-wise AND
         | A ~& B     | Bit-wise NAND
||
         | A ^ B      | Bit-wise XOR
         | A ~^ B     | Bit-wise XNOR
||
         | A &#124; B  | Bit-wise OR
         | A ~&#124; B | Bit-wise OR
||
Logical | A && B           | Logical AND
||
        | A &#124;&#124; B | Logical OR
||
Conditional | A ? B : C | If A, then B, else C
||
Assignment | A  = B      | Normal assign (automatically casts to the target fixed-point format)
           | A := B      | Raw assign (assigns bit-for-bit, ignoring the fixed-point format)
           | A @= B      | Append array B to A (equivalent to "A = @{A, B}" and only applicable to dynamic arrays)
           | A += B      | Add and assign (normal assign)
           | A -= B      | Subtract and assign (normal assign)
           | A \*= B     | Multiply and assign (normal assign)
           | A /= B      | Divide and assign (normal assign)
           | A %= B      | Modulus and assign (normal assign)
           | A &= B      | Bit-wise AND and assign (raw assign)
           | A &#124;= B | Bit-wise OR and assign (raw assign)
           | A ^= B      | Bit-wise XOR and assign (raw assign)
           | A <<= B     | Shift left and assign (normal assign)
           | A >>= B     | Shift right and assign (normal assign)

The unary, arithmetic, shift and relational operators take the fixed-point format of the operands into account, whereas the other operators do not.  Arithmetic operators result in a fixed-point number which is of such a format that no bits are lost in the calculation.  Other operators result in an unsigned integer, regardless of input format.

The target of an assignment can be an array, array slice or concatenation.

The usual flow-control structures (*if*, *for*, *while*, etc.) are supported.  More detail is provided later.

## 3.2 Signal use Before Assignment
When describing FPGA firmware it is often necessary to use a signal before a value is assigned.  This could occur when using parametrised classes, or clocked structures with external feedback.  The code below provides two examples:

    :::C++
    class Adder(N){ // Parametrisation is presented in the scripting section
     in  sig'N A, B;
     out sig'N Y;
     Y = A + B;
    }
    Adder(8) MyAdder; // Instance of an 8-bit adder.  This statement also evaluates "Y = A + B"
    sig'8 x, y, z;
    MyAdder.A = x; // These assignments assign values to A and B after use
    MyAdder.B = y;
    // Now the adder is complete.

And here is another example:

    :::C++
    sig'8 Counter;
    UserClk = (Counter == 125); // "Counter" has no value yet
    rtl(CPUClock){
     if(UserClk) Counter  = 0;
     else        Counter += 1;
    }

Internally, ALCHA implements the blocking nature of combinational assignments by using a copy of the source expression-tree, rather that the source signal itself.  When that source has no expression-tree yet, a reference to the signal is used instead.  A simplified example is presented below:

    :::C++
    sig'8 A, B, C, D;
    A = C;
    B = 5 * D;
    C = B + 7;

    // At this point, the expression trees look as follows:
    A = C;
    B = 5 * D;
    C = (5 * D) + 7;
    D = null

It is possible to create circular references, which is illegal.  This is illustrated below:

    :::C++
    sig'8 A, B, C;
    A = C;
    B = 5 * C;
    C = B + 7;

    // At this point, the expression trees look as follows:
    A = C;
    B = 5 * C;
    C = (5 * C) + 7; // Circular reference => illegal

This only applies to combinational circuits.  Clocked structures are evaluated as blocking assignments, so clocked circular references are legal.

An important consequence of this mechanism is that the developer must keep the dependencies in mind.  This is illustrated below:

    :::C++
    sig'8 A, B, C, D, E;
    A  = C;
    B  = 5 * D;
    C  = B + 7;
    E  = C - 9;
    B  = 2;
    C  = 3;
    D  = 4;
    A += D;

    // At this point, the expression trees look as follows:
    A = C + 4;
    B = 2;
    C = 3;
    D = 4;
    E = ((5 * D) + 7) - 9;
    // Any further changes to C will affect A, but not E.
    // Any futther changes to D will affect E, but not A.

This can become quite confusing.  Developers are therefore encouraged to avoid signal use before assignment when describing combinational circuits.

## 3.3 Control Flow Structures
### 3.3.1 if

    :::C++
    if(A){
     // if A is non-zero, do this
    }else{
     // otherwise do this
    }

### 3.3.2 switch

    :::C++
    switch(A){
     case(B, C, D){
      // if A is equal to B, C or D, do this
     }
     case(E){
      // if A is equal to E, do this
     }
     default{
      // otherwise do this
     }
    }

### 3.3.3 while

    :::C++
    while(A){
     // while A is non-zero, do this
    }

### 3.3.4 for

    :::C++
    for(x in A){
     // for every element of array A, set x to that element and do this
    }

One can use array literals to make this easier:

    :::C++
    for(j in 0->7) A += S[j]; // Sums the first 8 elements of S

The above is equivalent to:

    :::C++
    for(s in S[0->7]) A += s;

### 3.3.5 loop

    :::C++
    loop(N){
     // Do this N times
    }
    loop{
     // Do this forever
    }

# 4. Arrays
ALCHA only supports fixed-length arrays.  Arrays can be defined with any number of dimensions.

## 4.1 Defining Arrays
Each new dimension is defined by means of a positive integer in square brackets, representing the number of elements in that dimension.  An example is presented below:

    :::C++
    sig'8 A[16]; // A 16-element array of 8-bit unsigned integers
    sig'8 B[3][4][5]; // A 3-dimensional array of 8-bit unsigned integers

## 4.2 Slices
Array elements can be addressed by means of slices.  The bits are modelled as the deepest dimension, with indices equal to the power-of-two representation of that bit.  The element indices are zero-based.  A colon (":") can be used to indicate "all elements".  Any bit-slice removes the fixed-point format.  The result of a bit-slice is always an unsigned integer with as many bits as in the slice.  Some examples are presented below:

    :::C++
    sig'(16,8) A[16];
    A[5]                 // Element 5 of A (the 6th element).
    A[:][15:0]           // All the elements of A, but only the integer bits
    A[3, 7, 2][-1:-8]    // Elements 3, 7 and 2 of A (in that order), and only the fraction bits
    A[0->15#3]           // Every third element of A (0, 3, 6, 9, 12 and 15)

    A[5, 3, 7][-6, 7, 2] // Bits -6, 7 and 2 (in that order) of elements 5, 3 and 7 (in that order)
                         // The result is a 3-element array of 3-bit integers

The indices can also be specified by means of an integer array, as follows:

    :::C++
    sig'8 A[16];
    int   B[5] = @{4, 2, 7, 1, 12};
    A[B] // Elements 4, 2, 7, 1 and 12 of A, in that order
    A[B, 0->15] // Elements 4, 2, 7, 1, 12 and 0 to 15 of A

## 4.3 Array Literals and Concatenation
Array literals are useful for vectorised operations.  There are many forms, which are illustrated in the example below:

    :::C++
    @{0, 1, 2, 3}    // 4-element array
    0->3             // Same as above
    0->4#2           // The elements 0, 2 and 4
    3->0             // The elements 3, 2, 1 and 0
    @{0->3, 4->0#-2} // The elements 0, 1, 2, 3, 4, 2 and 0

## 4.4 Vectorised Statements
Array slices can be used in vectorised statements.  Vectorised operations apply the operation to each element of the array as if it was specified separately for each element.  This works for scalar functions as well.  Below are some examples:

    :::C++
    sig'8 Add(A, B){
     return A + B;
    }

    sig'8 A[16], B[16], Y[16];
    Y = A + B;     // Perform an element-wise addition
    Y = Add(A, B); // Equivalent to the addition above

    Y[0->15] = A[15->0] + B[0->15#2, 1->15#2];
    // Equivalent to:
    Y[ 0] = A[15] + B[ 0];
    Y[ 1] = A[14] + B[ 2];
    Y[ 2] = A[13] + B[ 4];
    ...
    Y[13] = A[ 2] + B[11];
    Y[14] = A[ 1] + B[13];
    Y[15] = A[ 0] + B[15];

When an array is used as the condition in an *if* statement or *while* loop, the array is AND-reduced.  This allows a concise means by which to compare all elements in an array (or string).

When a scalar is involved in a vectorised assignment, the scalar is repeated for every instance of the assignment.  This can be used, for instance, to assign a scalar to every element of an array.

# 5. Synchronous Circuits
## 5.1 Clock Networks
### 5.1.1 Definition
Clock pins can be used as base clocks to derive various types of derived clocks.  ALCHA supports three types of derived clocks: phase-locked loop (PLL) generated, internally generated and synchronous clock-enable type clocks.

PLL generated clocks are generally used for high frequency clocks (above 10 MHz), whereas clock-enable type clocks are generally used for lower frequency clocks. PLL generated clocks can have a frequency that is higher than that of the base clock and related by a rational constant. Synchronous clock-enable type clock frequencies are always some integer factor lower than the base clock.

Internally generated clocks include ripple, gated and multiplexed clocks. These are generally discouraged due to timing analysis difficulties, but can be useful when device power consumption is of critical importance to the design.  If the internally generated clock is based on the output of a combinational circuit, the ALCHA compiler automatically includes a register buffer to remove clock glitches.

Most FPGA have built-in PLL units with hardware clock counters at the output, which have multiple taps. All the clocks that are generated by one PLL unit have guaranteed phase relationships. These clocks can therefore be considered to be in the same clock domain and does not require special synchronisation structures to cross clock domains.  Synchronous clock-enable type clocks are in the same clock domain of the base clock. Ripple clocks are each in their own clock domain, because the phase relationship between the generating clock and the resulting ripple clock cannot be guaranteed.

ALCHA supports two variants of clock-enable type clocks.  The first uses an auto-generated enable signal that is high once every N clock cycles, where N is a developer-provided integer constant. The second simply links a developer-provided enable signal to the clock. For the second type, the frequency attribute must be specified for timing specification purposes and represents the maximum frequency of enabled clock edges.  A user-defined enable signal is specified by assigning a value to the clock signal.

Some examples of clock definitions are presented below:

    :::C++
    pin  <frequency =  "50 MHz"                                        > BaseClock;
    clock<frequency = "133 MHz", tolerance = "-5, 0", type = "PLL"     > DDRClock(BaseClock);
    clock<frequency =  "10 MHz",                      type = "PLL"     > CPUClock(BaseClock);
    clock<frequency =   "1 Hz" ,                      type = "Enable"  > RTC     (CPUClock );
    clock<frequency =   "5 MHz",                      type = "Enable"  > UserClk (CPUClock );
    clock<frequency =  "10 MHz",                      type = "Internal"> CPUGated(CPUClock );

    // Define the gated clock
    sig CPUGate; // Controled externally
    CPUGated = CPUGate & CPUClock;

    // Define a user-defined synchronous clock-enble type clock
    sig'5 UserClk_Limit; // Controled externally
    sig'5 UserClk_Counter;
    UserClk = (UserClk_Counter == UserClk_Limit);
    rtl(CPUClock){
     if(UserClk) UserClk_Counter  = 0;
     else        UserClk_Counter += 1;
    }

    // Use the RTC clock in a state machine
    sig'6 Seconds;
    fsm(RTC){
     loop{
      Seconds++;
     }
    }

And the equivalent Verilog:

    :::Verilog
    wire DDRClock;
    wire CPUClock;
    // Vendor-specific PLL instances to generate the two PLL clocks

    // Auto-generate the RTC enable-type clock
    reg [23:0]RTC_Counter;
    reg       RTC = (RTC_Counter == 24'd_9_999_999);
    always @(posedge CPUClock) begin
     if(RTC) RTC_Counter <= 0;
     else    RTC_Counter <= RTC_Counter + 1'b1;
    end

    // Implement the combinational circuit for the gated clock
    wire  CPUGate;
    wire tCPUGated = CPUGate & CPUClock;

    // Register the gated clock to remove ripples
    reg CPUGated;
    always @(posedge CPUClock) CPUGated <= tCPUGated;

    // Implement the UserClk code
    wire [4:0]UserClk_Limit;
    reg  [4:0]UserClk_Counter;
    wire      UserClk = (UserClk_Counter == UserClk_Limit);
    always @(posedge CPUClock) begin
     if(UserClk) UserClk_Counter <= 0;
     else        UserClk_Counter <= UserClk_Counter + 1'b1;
    end

    // Implement the state-machine
    reg tReset;
    always @(posedge CPUClock) begin
     tReset <= Reset; // Localise the reset

     if(tReset) begin
      // Reset signals here

     end else if(RTC) begin
      Seconds <= Seconds + 1'b1;
     end
    end

### 5.1.2 Global Clock Networks
Most modern FPGAs contain dedicated high-fanout, low-skew clock tree networks. ALCHA automatically assigns clocks to the available clock networks. If there are more clocks defined than clock networks available, the higher-frequency clocks take priority.  This automated global clock network assignment can be bypassed by explicitly specifying the *global* attribute.

### 5.1.3 Attributes
The *clock* keyword can take various attributes.  These are summarised in the below:

Attribute   | Default | Description
---------   | ------- | -----------
frequency   | None    | Compulsory: the frequency (or maximum intended frequency) of the clock
type        | None    | Compulsory: can be "PLL", "Enable" or "Internal"
location    | Auto    | In the case of a PLL-derived clock, the physical PLL to use
global      | Auto    | Indicates whether or not the signal should be routed on the global clock network.  Takes only "True", "False" or "Auto" values.

### 5.1.4 Built-in Properties
All clocks (including clock pins) have a *frequency* member (of type *rat*) that can be used by scripts to calculate clock-frequency dependent parameters.  A button denouncer might, for instance, be defined as follows:

    :::C++
    class Debouncer(clock Clk, sig Input){
     out sig Output;

     rat Count = Clk.Frequency * 20e-3 - 1; // 20 ms dead-time
     int N     = ceil(log2(Count));

     sig'N Counter;

     fsm(Clk){
      loop{
       while(Output == Input);
       
       Output  = Input,
       Counter = Count;

       while(|Counter) Counter--;
      }
     }

## 5.2 Register Transfer Level
Most HDL designers are familiar with RTL design.  In ALCHA, the *rtl* construct can be used to describe RTL logic.  It takes one parameter: the clock, which must be a clock type.  All statements within an *rtl* construct are non-blocking.  Below is an example:

    :::C++
    in pin<frequency = "50 MHz"> Clk;
    in pin                       Reset;

    sig A, B;

    sig'27 Count;

    rtl(Clk){
     if(Reset) Count = 0;
     else      Count++;

     if(&Count){
      A = B;
      B = A;
     }
    }

The equivalent Verlog of the above ALCHA is:

    :::Verilog
    reg       A, B;
    reg [26:0]Count;

    always @(posedge Clk) begin
     if(Reset) Count <= 0;
     else      Count <= Count + 1'b1;

     if(&Count) begin
      A <= B;
      B <= A;
     end
    end

The *rtl* construct essentially provides a means to write code that translates directly to Verilog.  It is not meant for general use, but rather for special cases where the low-level control of RTL is required.  It does provide the developer with vectorised notation and combinational function calls, however.

## 5.3 Finite State Machines
Most FPGA firmware is based on finite state machines (FSMs).  ALCHA provides a concise FSM description syntax by means of the *fsm* construct.  It takes two parameters: the clock and the reset.  The reset is optional: if a reset signal is not specified, an auto-generated power-on reset signal is used.

Each statement within an *fsm* construct that ends in a comma (","), is considered to be in the same state, or cycle, as the next statement.  A semicolon (";") ends a cycle (or state).  All statements are considered non-blocking.  A simple example is provided below:

    :::C++
    in pin<frequency = "50 MHz"> Clk;
    in pin                       Reset;

    sig'8 A, B, C;

    B = 123;

    fsm(Clk, Reset){
     loop{
      A = B + C, B = C - A;

      if(A < B) C++,
      else      C--,
      ;
     }
    }

This state machine has two states and translates to the following Verilog:

    :::Verilog
    reg[7:0] A, B, C;

    reg tReset;
    reg State;

    always @(posedge Clk) begin
     tReset <= Reset

     if(tReset) begin
      B     <= 8'd123;
      State <= 0;

     end else begin
      case(State)
       1'b0: begin
        A     <= B + C;
        B     <= C - A;
        State <= 1'b1;
       end

       1'b1: begin
        if(A < B) C <= C + 1'b1;
        else      C <= C - 1'b1;
        State <= 1'b0;
       end

       default:;
      endcase
     end
    end

All reset signals are localised to the state machine.  All signals that enter the *fsm* construct with a value (that must evaluate to a run-time constant) are reset to that value.

The ALCHA *if* and *while* statements follow the same syntax as in C.  The state (or cycle) boundaries are controlled by means of semicolons (";").  

The *for* loop is used to iterate through elements in an array, as follows:

    :::C++
    sig'8  x;
    sig'10 A;

    fsm(Clk){
     for(x in 0->200){
      A = 3 * x + 7;
     }
    }

This translates to:

    :::Verilog
    reg [7:0]x;
    reg [9:0]A;
    reg      State;

    always @(posedge Clk) begin
     tReset <= Reset;

     if(tReset) begin
      x     <= 0;
      State <= 0;

     end else begin
      case(State)
       1'b0: begin
        A <= 2'd3 * x + 3'd7;
        x <= x + 1'b1;
        if(x == 8'd200) State <= 1'b1;
       end

       default:;
      endcase
     end
    end

The *loop* loop is used to repeat something.  Without a parameter, a *loop* loop will run forever (or until a *break* statement is encountered).  With a parameter, the *loop(N)* loop will run for *N* iterations.

## 5.4 Pipelines
Finite state machines can also be used to implement pipelines.  Say, for instance, the developer wants to implement a pipelined version of the function

    y = a*x*x + b*x + c,

where a, b and c are constants.  The intended non-pipelined behaviour is given below, in which case there would be a new result every 4 cycles.

    :::C++
    fsm(Clk){
     loop{
      t =     a*x;
      t =     t*x;
      t = t + b*x;
      y = t + c;
     }
    }

The pipelined version (that provides a new result every cycle, with a 4-cycle latency) is:

    :::C++
    // t  is a 3-element array
    // tx is a 2-element array
    fsm(Clk){
     loop{
      tx = @{x, tx[0]}, // array concatenation

      t[0] =        a   * x   ,
      t[1] =        t[0]*tx[0],
      t[2] = t[1] + b   *tx[1],
      y    = t[2] + c;
     }
    }

# 6. Functions and Procedures
ALCHA provides a procedural programming model for combinational circuits, as well as state machines.  Any function that does not contain an embedded state machine is considered combinational and can be called from anywhere.  A function that contains an embedded state machine can only be called from within another state machine.

## 6.1 Combinational Functions
Functions in ALCHA are defined in similar fashion as C, as illustrated in the example below.  The function name is used as a temporary signal (or variable) for the return value.  The parameter types do not need to be defined in the function, but can be inferred from the input parameters when the function is called.  When the types are specified, the parameter types form part of the function name, so that many functions of the same name, but different parameter types, can be defined.

    :::C++
    sig'8 Add(A, B){
     return A + B; // This is short-hand for "Add = A + B; return;"
    }
    sig'8 x, y, z;
    z = Add(x, y);

Combinational functions can be called from anywhere, including the bodies of clocked structures.  Functions can also be called in vectorised form, which is especially useful when combined with array slicing.  The adder above can be called as follows:

    :::C++
    sig'8 x[4], y[4], z[4];
    z = Add(x, y);

    // This is equavalent to:
    z[0] = Add(x[0], y[0]);
    z[1] = Add(x[1], y[1]);
    z[2] = Add(x[2], y[2]);
    z[3] = Add(x[3], y[3]);

Arrays and scalars can be mixed in the call, as follows:

    :::C++
    sig'8 x, y[4], z[4];
    z = Add(x, y);

    // This is equavalent to:
    z[0] = Add(x, y[0]);
    z[1] = Add(x, y[1]);
    z[2] = Add(x, y[2]);
    z[3] = Add(x, y[3]);

If the function takes an array as a parameter, the same rules apply.  The function can be called with a higher-dimension array, as follows:

    :::C++
    sig'18 Dot(A[], B[]){
     Dot  = A[1] * B[1];
     Dot += A[2] * B[2];
     Dot += A[3] * B[3];
     Dot += A[4] * B[4];
     return;
    }
    sig'18 Y[16];
    sig'8  A[4][16], B[4][16];
    Y = Dot(A, B);

To return an array, the function name should be defined as an array, as follows:

    :::C++
    signed sig'(16,8) Mult[4][4](A[][], B[][]){
     int i, j, k;
     for(i in 0->3){
      for(j in 0->3){
       Mult[i][j] = 0;
       for(k in 0->3){
        Mult[i][j] += A[i][k] * B[k][j];
       }
      }
     }
    }

The loops above are discussed in greater detail in the Scripting section.  Such combinational loops should be used with care, as very little code can result in very large circuits.

## 6.2 Clocked Functions
Clocked functions contain optional combinational code, as well as an embedded state machine.  The embedded state machine does not take parameters, as the clock is inherited from the calling state machine.  The calling state also implements the reset.

There has to be a cycle-boundary after a clocked function call (i.e. the call must end with a semicolon, not a comma).

The body of the embedded state machine is only implemented once, so that many states in the calling state machine can make use of the function without further resource usage.

The combinational code is evaluated during the calling state, after which the function parameters, as well as the return state, are saved to temporary registers.  The next cycle executes the first state of the embedded state machine.

A return statement in the embedded state machine will cause the target signal to be assigned, and the next state to be loaded from the temporarily-stored state register.  To illustrate this process, consider the following ALCHA (and its corresponding Verilog) code:

    :::C++
    sig'4 BitCount(A){
     sig'3 x;
     BitCount = 0;

     fsm{
      for(x in 0->7){
       BitCount += A[x]; // The ";" forces every addition into a new cycle
      }
      return;
     }
    }

    sig'8 A, B;

    fsm(Clk, Reset){
     A = 123;
     B = 93;
     A = BitCount(A);
     B = BitCount(B);
    }

Which translates to:

    :::Verilog
    reg [2:0]BitCount_RetState;
    reg [3:0]BitCount;
    reg [7:0]BitCount_A;
    reg [2:0]BitCount_x;

    reg [7:0]A;
    reg [7:0]B;

    reg [2:0]State;
    reg      tReset;

    always @(posedge Clk) begin
     tReset <= Reset;

     if(tReset) begin
      State <= 0;

     end else begin
      case(State)
       3'd0: begin
        A     <= 8'd123;
        State <= 3'd1;
       end
       
       3'd1: begin
        B     <= 8'd93;
        State <= 3'd2;
       end
       
       3'd2: begin
        BitCount_RetState <= 3'd3;
        BitCount_A        <= A;
        BitCount_x        <= 0;
        BitCount          <= 0;
        State             <= 3'd7;
       end
       
       3'd3: begin
        A                 <= BitCount;
        BitCount_RetState <= 3'd4;
        BitCount_A        <= B;
        BitCount_x        <= 0;
        BitCount          <= 0;
        State             <= 3'd7;
       end

       3'd4: begin
        A     <= BitCount;
        State <= 3'd5;
       end
       
       3'd7: begin // The BitCount function
        BitCount   <= BitCount + BitCount_A[BitCount_x];
        BitCount_x <= BitCount_x + 1'b1;
        if(BitCount_x == 3'd7) State <= BitCount_RetState;
       end

       default:;
      endcase
     end
    end

## 6.3 Operator Overloading
Operator overloading is handled in similar fashion to C.  It can be used to define abstract signal types, such as complex numbers and floating point implementations.  Operator overloading can be clocked functions, but then they cannot be used in combinational statements.  A simple fixed-point complex number class could be partially implemented as follows:

    :::C++
    class Complex{
     sig'(8,8) x = 0;
     sig'(8,8) y = 0;

     Complex operator+(complex C){
      Complex r;
      r.x = x + real(C);
      r.y = y + imag(C);
      return r;
     }
     Complex operator+(sig'(8,8) X){
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

The pattern continues for other relevant operators.  The *complex* type is a built-in scripting type and will be presented in more detail in the scripting section.

# 7. Scripting
ALCHA provides a powerful scripting platform to control the generation of firmware.  Any scripting variable (or scripting expression) can be used where synthesisable code expects a constant, thereby providing powerful parametrisation features.  Scripting can further be used for conditional compilation based on various global parameters, including the *target* attributes.

## 7.1 Data Types
The table below summarises ALCHA scripting variable types:

Keyword | Description
------- | -----------
byte    | 8-bit unsigned integer typically used for binary file I/O
char    | 32-bit Unicode character typically used for text file I/O
int     | Infinite precision signed integers ([GNU MP](https://gmplib.org/))
rat     | Infinite precision signed rational numbers ([GNU MP](https://gmplib.org/))
float   | Arbitrary precision floating point ([GNU MPFR](http://www.mpfr.org/))
complex | Arbitrary precision complex floating point ([GNU MPFR](http://www.mpfr.org/))

The *float* and *complex* keywords can optionally be followed by the "'" operator and an integer, which specifies the number of bits in the mantissa.  The exponent is limited to 32-bit by the MPFR library.  If no format is specified, double-precision is used.

## 7.2 Flow Control
*If* statements and *while* loops use the same syntax as C.  The grammar of *case* statements have not been finalised yet.  *For* and *loop* loops use the same syntax as described in the finite state machine section.

## 7.3 Mathematical Functions
ALCHA scripting has the following built-in mathematical functions:

Function | Description
-------- | -----------
sin      | Sine (radians)
cos      | Cosine (radians)
tan      | Tangent (radians)
asin     | Arc-sine
acos     | Arc-cosine
atan     | Arc-tangent
sinh     | Hyperbolic sine
cosh     | Hyperbolic cosine
tanh     | Hyperbolic tangent
asinh    | Hyperbolic arc-tangent
acosh    | Hyperbolic arc-cosine
atanh    | Hyperbolic arc-tangent
log      | Natural logarithm
log2     | Logarithm to base 2
log10    | Logarithm to base 10
exp      | Natural exponent
pow      | Exponent (takes 2 parameters)
round    | Round to nearest integer
fix      | Drops the part after the decimal point
floor    | Round to negative infinity
ceil     | Round to positive infinity
rand     | Random number between 0 and 1, inclusive
real     | The real part of a *complex* type
imag     | The imaginary part of a *complex* type
abs      | The magnitude of a *complex* type
angle    | The phase of a *complex* type, in radians
fft      | Returns the fast Fourier transform of an array of *float* or *complex*

Fast Fourier transforms (FFTs) are implemented by means of the [FFTW library](http://www.fftw.org/).  The implication is that all FFTs are double-precision.

## 7.4 File I/O
File I/O is accomplished by means of four functions, described below:

Function  | Description
--------  | -----------
read      | Reads the given file into a *byte* array buffer
write     | Writes the given *byte* array to file
textread  | Reads the given UTF-8 file into an UTF-32 *char* array buffer
textwrite | Writes the given UTF-32 *char* array to an UTF-8 file

The file contents are kept in arrays, which are buffered in RAM.  This limits the number, and size, of files that can be handled at the same time.  The memory is freed when the array life-time expires.  Developers are therefore encouraged to keep arrays meant for large files local to functions, or make use of the same array for multiple files.

## 7.5 Dynamic Arrays
Scriping arrays can be declared as dynamic by specifying empty square brackets.  The array size is adjusted depending on the assignment.  While building the contents of a text file, for instance, the code might look as follows:

    :::C++
    char File[] = "";
    File @= "Header\n";
    File @= "Some Body...\n";
    File @= "Some Body...\n";
    File @= "Some Body...\n";
    textwrite("My Log File.log", File);

## 7.6 Shell Commands
ALCHA has a built-in function *shell* to execute shell commands.  It takes a single parameter (a string) that is executed as if called on the command-line.  This can be used, for instance, to call an external compiler for CPU code.  The resulting CPU binary can be imported by means of file I/O functions and used to initialise the CPU ROM.

# 8. High-level Structures
## 8.1 Memory Blocks
Memory blocks can be instantiated by means of built-in classes to represent the various configurations of block RAM.  The various configuration, and the corrosponding classes, are summarised in the table below:

Configuration    | Class
-------------    | -----
ROM, 1-port      | ROM1(int Width, int Depth, clk Clock, int Init[])
ROM, 2-port      | ROM2(int Width, int Depth, clk Clock1, clk Clock2, int Init[])
RAM, 1-port      | RAM1(int Width, int Depth, clk Clock, int Init[])
RAM, 1W, 1R port | RAM_RW(int Width, int Depth, clk Clock1, clk Clock2, int Init[])
RAM, full 2-port | RAM2(int Width, int Depth, clk Clock1, clk Clock2, int Init[])
FIFO,            | FIFO(int Width, int Depth, clk Clock1, clk Clock2, int Init[])

The other signals are implemented as member signals.

## 8.2 Megafunctions and External HDL Modules
An FPGA project often makes use of FPGA features that are not directly supported by ALCHA.  Such features might include, among others, PCI-Express interfaces, DDR memory interfaces and embedded processors.  These are called "megafunctions", after the Altera nomenclature.  Megafunctions are both vendor and device specific.

Megafunctions and other external HDL modules can be imported into the ALCHA project by means of the *hdl* construct, as illustrated below.  Imported HDL modules are not included in simulation.

    :::C++
    hdl("Library/RS232.v", "Library/RS232_Rx.v", "Library/RS232_Tx.v") RS232_V(
     CountBits =  5; // Parameters 
     Count0_5  =  8; 
     Count1    = 17; 
     Count1_5  = 25;
    ){
     in sig nReset;
     in sig Clk;
    
     in  sig'8 TxData;
     in  sig   Send;
     out sig   Busy;
    
     out sig   DataReady;
     out sig'8 RxData;
     in  sig   Ack;
      
     out sig Tx;
     in  sig Rx;
    }
    
    class RS232(clk Clock, sig Reset, int BAUD): RS232_V(
     ceil(log2(round(Clock.Frequency / BAUD * 1.5))),
               round(Clock.Frequency / BAUD / 2),
               round(Clock.Frequency / BAUD),
               round(Clock.Frequency / BAUD * 1.5)
    ){
     nReset = !Reset;
     Clk    = Clock;
    }

    in  pin<frequency = "50 MHz"> Clk;
    in  pin                       Reset;
    out pin                       RS232_Tx;
    in  pin                       RS232_Rx;

    RS232(Clk, Reset, 9600) MyUART;
    RS232_Tx = MyUART.Tx;
    MyUART.Rx = RS232_Rx;

# 9. Simulation and Verification
ALCHA supports both simulation and hardware testing.

## 9.1 Simulation
When compiling to a simulation target, the developer must specify a test-bench. ALCHA automatically generates waveforms for clock pins and derived clock signals.  The developer can generate other waveforms by using the *fsm* construct, as illustrated below:

    :::C++
    // Generating a test waveform of a bouncing button
    pin<frequency = "1 kHz"> ButtonClk;
    pin Button = 1;
    fsm(ButtonClk){
     loop(100); // Wait 100 ms
     Button = 0,
     loop(floor(rand()*5+3)){ // 3 to 8 bounces
      loop(floor(rand()*5+1)); // 1 to 5 ms delay
      Button = 1,
      loop(floor(rand()*5+1)); // 1 to 5 ms delay
      Button = 0,
     }; // End of waveform
    }

During simulation, the ALCHA compiler writes all pin signals to a wave-form database that can be viewed with a GUI or analysed by other means. The simulation only supports cycle-accurate behavioural simulation. Timing-accurate simulation can be achieved by simulating the output of a project target with existing vendor tools.

## 9.2 Hardware Testing
The waveform generator above is hardware synthesisable. The same test-bench can therefore be used to test the system on real hardware, optionally using a firmware-based logic analyser.

# 10. Automated Optimisation
The ALCHA compiler applies various optimisations before project synthesis, most of which are related to finite state machine resource usage. The details of this is outside the scope of this wiki. One notable optimisation, however, is automated multi-cycle analysis.

Unless specified otherwise, the circuit synthesis process assumes that all register-to-register paths have to have a delay of less than one clock period. This is not true for all paths in a typical state-machine. It is likely that a particular signal path have multiple clock-cycles in which to pass through the combinational cloud, thereby requiring less stringent timing constraints than the synthesis tool initially assumes.

Manually specifying multi-cycle paths is a labour-intensive and tedious process avoided by most developers. ALCHA performs this task automatically whenever the design is compiled, thereby generating code that is easier to synthesise, resulting in reduced compilation times.

# 11. Implementation Time-frame

Target Date   | Feature
-----------   | -------
October 2016  | Prototype compiler with elementary simulation
December 2016 | Compilation to Altera projects
June 2017     | Compilation to Xilinx projects

# References
## Similar Projects

- [Synflow Cx](http://cx-lang.org/)
- [Migen](https://github.com/m-labs/migen)
- [MyHDL](http://www.myhdl.org/)

## Publications

- J Taylor and S Winberg
  [ALCHA: New Object Oriented Approach to FPGA Project Development](http://www.icit2016.org/)
  Accepted for publication at the 2016 IEEE International Conference on Industrial Technology

## Bibliography
- K Chapman
  [Get your Priorities Right -- Make your Design Up to 50% Smaller](http://www.xilinx.com/support/documentation/white_papers/wp275.pdf)
  Xilinx white paper, 2007

- K Chapman
  [Get Smart About Reset: Think Local, Not Global](http://www.xilinx.com/support/documentation/white_papers/wp272.pdf)
  Xilinx white paper, 2008

- P Coussy and A Morawiec
  [High-Level Synthesis: from Algorithm to Digital Circuit](http://www.amazon.com/High-Level-Synthesis-Algorithm-Digital-Circuit/dp/9048179238)
  Springer, 2008, ISBN 978-1-4020-8587-1, e-ISBN 978-1-4020-8588-8

- J DeLaere and S Zammattio
  [Top 7 Reasons to Replace Your Microcontroller with a MAX 10 FPGA](https://www.altera.com/content/dam/altera-www/global/en_US/pdfs/literature/wp/wp-01255-top-7-reasons-to-replace-your-microcontroller-with-a-max-10-fpga.pdf)
  Altera, June 2015

- A Feller, R Noto and A M Smith
  [Standard Cell Approach for Generating Custom CMOS/SOS Devices Using a Fully Automatic Layout Program](http://dx.doi.org/10.1109/MCAS.1981.6323756)
  IEEE Circuits and Systems Magazine, September 1981, Volume 3, Number 3, Pages&nbsp;9&nbsp;&endash;&nbsp;13

- L Fousse, G Hanrot, V Lef&232;vre, P P&233;lissier and P Zimmermann
  [MPFR: A Multiple-precision Binary Floating-point Library with Correct Rounding](http://doi.acm.org/10.1145/1236463.1236468)
  ACM Transactions on Mathematical Software (TOMS), June 2007, Volume 33, Number 2

- M Frigo and S G Johnson
  [The Design and Implementation of FFTW3](http://dx.doi.org/10.1109/JPROC.2004.840301)
  Proceedings of the IEEE, February 2005, Volume 93, Number 2, Pages&nbsp;216&nbsp;&endash;&nbsp;231

- T Granlund and the GMP development team
  [GNU MP: The GNU Multiple Precision Arithmetic Library](http://gmplib.org/)
  2015, Version 6.1.0

- P Luksch, U Maier, S Rathmayer, M Weidmann, F Unger, P Bastian, V Reichenberger and A Haas
  [Software Engineering in Parallel and Distributed Scientific Computing: a Case Study from Industrial Practice](http://dx.doi.org/10.1109/PDSE.1998.668179)
 Proceedings of International Symposium on Software Engineering for Parallel and Distributed Systems, IEEE, April 1998, Pages&nbsp;187&nbsp;&endash;&nbsp;1997

- R Nane, V M Sima, C Pilato and J Choi
  [A Survey and Evaluation of FPGA High-Level Synthesis Tools](http://dx.doi.org/10.1109/TCAD.2015.2513673)
  IEEE Transactions on Computer-Aided Design of Integrated Circuits and Systems, December 2015

- R R Seban
  [An Overview of Object-Oriented Design and C++](http://dx.doi.org/10.1109/AERO.1994.291202)
  Proceedings of Aerospace Applications Conference, IEEE, February 1994, Pages&nbsp;65&nbsp;&endash;&nbsp;86

- J Stephenson
  [Design Guidelines for Optimal Results in FPGAs](http://notes-application.abcelectronique.com/038/38-21414.pdf)
  Altera Corporation, 2005

- IEEE 1800-2012
  [IEEE Standard for SystemVerilog &endash; Unified Hardware Design, Specification, and Verification Language](http://dx.doi.org/10.1109/IEEESTD.2013.6469140)
  IEEE, February 2013

- T van Court and M C Herbordt
  [Requirements for any HPC/FPGA Application Development Tool Flow](http://dx.doi.org/10.1155/ASP/2006/97950)
  Proceedings of the fourth Annual Boston Area Computer Architecture Workshop, 2006

- H Zheng, S T Gurumani, L Yang, D Chen and K Rupnow
  [High-Level Synthesis with Behavioral-Level Multicycle Path Analysis](http://dx.doi.org/10.1109/TCAD.2014.2361661)
  Transactions on Computer-Aided Design of Integrated Circuits and Systems, IEEE, December 2014, Volume 33, Number 12, Pages&nbsp;1832&nbsp;&endash;&nbsp;1845

--------------------------------------------------------------------------------

[[members limit=20]]
[[download_button]]

