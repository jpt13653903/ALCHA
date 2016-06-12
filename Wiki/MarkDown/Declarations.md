[[include repo=code path=Wiki/MarkDown/Header.md]]

[TOC]

# Declarations
## Nets
### Definition
Nets relate to physical wires and / or registers on the FPGA.  They are specified by means of the `net` keyword.  If no format is specified, a net is a single-bit unsigned integer, which also represents a boolean.  The format is specified by means of a "fixed-point cast" using the `'` (single-quote) operator.  Some examples are shown below:

    :::C++
    net           a; // Single-bit unsigned integer
    net'8         b; // 8-bit unsigned integer
    net'(7, -128) c; // 8-bit signed integer

    net'(4, 4)    d; // 4-bit unsigned fixed point with 2 integer bits
                     // and 2 fraction bits (in the range [0, 4)
    net'(4, -4)   e; // 5-bit signed fixed-point in the range [-4, 4)

### Initialisers
All nets, pins and variables can take an optional initialiser, as illustrated below:

    :::C++
    net' 8     a  = 7;       // The binary value "0000 0111"
    net'(8, 4) pi = 355/113; // The binary value "1100 1001"

### Attributes
Nets take optional attributes, which are summarised in the table below:

Attribute      | Default   | Description
---------      | -------   | -----------
`global`       | `"False"` | Indicates whether or not the net should be routed on the global clock network.  Takes only `"True"` or `"False"` values.
`output_clock` | `"None"`  | Specifies the clock used for external setup and hold constraints.
`setup`        | `"0"`     | Specifies the minimum setup time of an external register, in relation to `output_clock`
`hold`         | `"0"`     | Specifies the minimum hold time of an external register, in relation to `output_clock`
`input_clock`  | `"None"`  | Specifies the clock used for external delay constraints.
`min_delay`    | `"0"`     | Specifies the minimum output delay of an external register, in relation to `input_clock`
`max_delay`    | `"0"`     | Specifies the maximum output delay of an external register, in relation to `input_clock`

### Timing Constraints
As indicated in the table above, net attributes can be used to specify external peripheral timing requirements.  These timing attributes are specified as if there are no PCB or trace delays.  When the net is connected to a pin, the trace delays and pin capacitance specified by the pin attributes are used in conjunction with the net timing attributes to calculate the appropriate external timing constraints.  An example is provided below:

    :::C++
    // PCB trace delays in the pin definitions:
    pin'4<voltage   = "3.3 V" , capacitance = "10 pf",
          min_delay = "500 ps", max_delay   =  "1 ns"
          number    = "U7, T7, V8, T8"> SD_DAT;

    // Peripheral specifications in the driver class:
    net'4< input_clock = "!Clock", max_delay = "14 ns"          > Data_in;
    net'4<output_clock =  "Clock", setup = "5 ns", hold = "5 ns"> Data_out;

    // Assign the nets to the pin
    Data_in = SD_DAT;
    SD_DAT  = Data_out;

External delays are always referenced to a clock, even if the external delay is purely combinational.  This is so that the timing constraints are compatible with the Synopsis Design Constraints format, which is the industry-preferred standard.

### Bus Connections
ALCHA does not support high-impedance nets directly.  Pins can be set to high-impedance by disabling the driver (see the Pins section for details).  In order to emulate a bus that has a set of tri-state drivers, the nets should be gated through AND gates and then combined through an OR gate to drive the bus.

## Pins
### Definition
Pins in ALCHA are specified by means of the `pin` keyword.  Pins can have various attributes.  A short example is presented below:

    :::C++
    in  pin<frequency = "50 MHz", voltage = "2.5 V", location = "H12"> Clock;
    in  pin<                      voltage = "1.2 V", location = "P11"> Key;
    out pin<                      voltage = "2.5 V", location = "F7" > LED;

If the `in` or `out` keyword is absent, the pin is bidirectional.

### Attributes
The table below summarises pin attributes and their default values:

Attribute     | Default    | Description
---------     | -------    | -----------
`location`    | `"None"`   | The physical pin number on the target device.  In the case of a differential pair, the pin location is specified in `"PP-NN"` format, where `PP` is the positive pin number and `NN` the negative pin number.
`voltage`     | `"3.3 V"`  | The voltage of the I/O bank.
`current`     | `"5 mA"`   | The intended maximum current of the pin, in case of an output.
`capacitance` | `"10 pF"`  | The load capacitance of the pin, in case of an output.
`standard`    | `"LVCMOS"` | The logic standard of the pin.
`min_delay`   | `"0"`      | The minimum physical trace delay of the pin.
`max_delay`   | `"0"`      | The maximum physical trace delay of the pin.
`frequency`   | `"None"`   | In the case of a clock input, the frequency of the clock.
`jitter`      | `"0"`      | In the case of a clock input, the clock jitter.

The unit is important when specifying attributes.  If the unit is absent, a compilation error occurs.  Any SI unit prefix is valid, including `T`, `G`, `M`, `k`, `m`, `u`, &mu; (U+03BC), &micro; (U+00B5), `n`, `p` and `f`.

### Pin Vectors
When specifying pin locations for bit-vectors or pin arrays, the `location` attribute contains a comma-separated list of pins.  Pin locations are specified most-significant bit first, as follows:

    :::C++
    out pin'8<location = "H9, H8, B6, A5, E9, D8, K6, L7"> LED;

Pin arrays are handled in similar fashion.  The first (0-index) pin location(s) are specified first, then the next (1-index), etc.  All the locations are comma-separated.

### High-impedance Pins
Output and bidirectional pins have a built-in `enable` property, which represents the "driver-enable" wire.  Output pin drivers are enabled by default, whereas bidirectional pin drivers are disabled by default.

## Groups
Often there are attributes that applies to many objects.  In this case, the definitions can be grouped.  All child definitions inherit the attributes of the group.  When a child definition includes an attribute that is already defined in the group, the child definition takes precedence.  Below is an example of a named group for pin definitions.

    :::C++
    <voltage   = "3.3 V" , capacitance = "10 pF",
     min_delay = "500 ps", max_delay = "1 ns"> SD{
     out pin  <location = "AB6"           > CLK;
         pin  <location = "W8"            > CMD;
         pin'4<location = "U7, T7, V8, T8"> DAT;
    }

In this case, the pins that are actually defined are `SD.CLK`, `SD.CMD` and `SD.DAT`.  The group can also be anonymous, as given below:

    :::C++
    <voltage = "3.3 V", frequency = "50 MHz">{
     in pin<location = "R20"> CLOCK_B5B;
     in pin<location = "N20"> CLOCK_B6A;
    }

Nets, class instances, derived clocks, etc. can be grouped in similar fashion.

## Scripting Data Types
The table below summarises ALCHA scripting variable types:

Keyword   | Description
-------   | -----------
`byte`    | 8-bit unsigned integer typically used for binary file I/O
`char`    | 32-bit Unicode character typically used for text file I/O
`int`     | Infinite precision signed integers ([GNU MP](https://gmplib.org/))
`rat`     | Infinite precision signed rational numbers ([GNU MP](https://gmplib.org/))
`float`   | Arbitrary precision floating point ([GNU MPFR](http://www.mpfr.org/))
`complex` | Arbitrary precision complex floating point ([GNU MPFR](http://www.mpfr.org/))

The `float` and `complex` keywords can optionally be followed by the `'` operator and an integer, which specifies the number of bits in the mantissa.  The exponent is limited to 32-bit by the MPFR library.  If no format is specified, double-precision is used.

More on scripting in the [Scripting Section](p/alcha/wiki/Scripting)

## Enumerations
An enumeration type can be defined by means of the `enum` keyword, as illustrated below.  The numerical constants associated with the enumeration start at `0` for the first element and increase by `1` for each element.

    :::C++
    enum STATE {Idle, Writing, Done, Others}
    STATE State;

    switch(State){
     case(Idle){
      // Do some stuff
      State = Writing;
     }
     case(Writing){
      // Do some stuff
      State = Done;
     }
     case(Done){
      // Do some stuff
      State = Idle;
     }
     default{
      // Do some stuff
     }
    }

Ordinarily, enumerations are equivalent to the non-synthesisable type `int`.  The enumeration can, however, be used to define a synthesisable enumeration, as follows:

    :::C++
    enum STATE {Idle, Writing, Done, Others}
    pin'STATE PinState; // Pin enumeration
    net'STATE SigState; // Net enumeration

In this case, the number of bits, or width, of the net (or pin array) is the number of bits required to uniquely identify each enumeration value.  In the above example, this is 4&nbsp;bits.

If an enumeration is used outside a variable of that type, the values of the enumeration can be referenced through its type.  The following statements are all legal:

    :::C++
    enum Enum{A, B, C}

    int  a;
    Enum e;

    a = Enum.A;
    e = B;
    a = e;

A variable declared as an enumeration can only take values from that enumeration.  It is illegal to assign anything else to an enumeration type.  It is legal to compare an enumeration to other types, however.

    :::C++
    enum Enum{A, B, C}

    int  a;
    Enum e, n;

    a = A;          // Illegal: A does not exist in this namespace
    e = A;          // Legal  : assigning a value to the enumeration
    n = e;          // Legal  : the enumeration types are the same
    e = 2;          // Illegal: must assign a value from the defined list
    a = e;          // Legal  : e is automatically cast to an int
    if(e == B){...} // Legal  : comparing enumeration values
    if(a == B){...} // Illegal: B does not exist in this namespace
    if(e == 2){...} // Legal  : e is automatically cast to an int

## Built-in Members
Various ALCHA types have built-in members, which can be accessed similarly to class members.  The list below is not exhaustive.

Member      | Types           | Description
------      | -----           | -----------
`width`     | `pin`, `net`    | The number of bits in the vector.  Returns `1` for scalers.
`length`    | All array types | The number of elements in the array.
`frequency` | Clock nets      | The frequency of the clock, in Hz.

[[include repo=code path=Wiki/MarkDown/Footer.md]]

