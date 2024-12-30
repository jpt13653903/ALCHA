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
- [Expressions](Expressions.md#declarations)
  - [Nets](#nets)
    - [Definition](#definition)
    - [Initialisers](#initialisers)
    - [Attributes](#attributes)
    - [Timing Constraints](#timing-constraints)
    - [Bus Connections](#bus-connections)
  - [Pins](#pins)
    - [Definition](#definition)
    - [Attributes](#attributes)
    - [Pin Vectors](#pin-vectors)
  - [Groups](#groups)
  - [Structures](#structures)
  - [Enumerations](#enumerations)
    - [Base Type and Encoding](#base-type-and-encoding)
    - [ALCHA Enumeration Semantics](#alcha-enumeration-semantics)
  - [Scripting Data Types](#scripting-data-types)
  - [Built-in Members](#built-in-members)
- [Statements](Statements.md)
- [Arrays](Arrays.md)
- [Functions](Functions.md)
- [Synchronous Circuits](SynchronousCircuits.md)
- [Classes](Classes.md)
- [Operator Overloading](OperatorOverloading.md)
- [Scripting Features](Scripting.md)
- [Advanced Attributes](AdvancedAttributes.md)
- [High-level Structures](HighLevelStructures.md)
- [Simulation and Verification](Simulation.md)

--------------------------------------------------------------------------------

# Declarations

## Nets

### Definition

Nets relate to physical wires and / or registers on the FPGA.  They are
specified by means of the `net` keyword.  If no format is specified, a net is
a single-bit unsigned integer, which also represents a boolean.  The format is
specified by means of optional instantiation parameters.  Some examples are
shown below:

```alcha
  net          a; // Single-bit unsigned integer
  net(8)       b; // 8-bit unsigned integer
  net(7, -128) c; // 8-bit signed integer

  net(4, 4)    d; // 4-bit unsigned fixed point with 2 integer bits
                  // and 2 fraction bits -- in the range [0, 4)
  net(4, -4)   e; // 5-bit signed fixed-point in the range [-4, 4)
```

Signed nets have one more bit than specified in the format.  This is a
convenient convention for fixed-point digital signal processing, but can be
confusing.

### Initialisers

All nets, pins and variables can take an optional initialiser, as illustrated below:

```alcha
    net(8   ) a  = 7;       // The binary value "0000 0111"
    net(8, 4) pi = 355/113; // The binary value "1100 1001"
```

### Attributes

Nets take optional attributes, which are summarised in the table below.
External IC parameters and PCB trance parameters are split into separate
attributes so that they can be specified at different points in the design.
The PCB trace parameters might be specified with the pin definition, for
instance, while the external IC parameters are specified in the abstraction
module that controls that IC.

Attribute            | Default | Description
---------            | ------- | -----------
`global`             | `false` | Indicates whether or not the net should be routed on the global clock network.  Takes only `true` or `false` values.
`external_clock`     | `none`  | Specifies the clock used for external setup and hold constraints.
`external_setup`     | `0`     | Specifies the minimum setup time of an external register, in relation to `external_clock`
`external_hold`      | `0`     | Specifies the minimum hold time of an external register, in relation to `external_clock`
`external_min_delay` | `0`     | Specifies the minimum output delay of an external register, in relation to `external_clock`
`external_max_delay` | `0`     | Specifies the maximum output delay of an external register, in relation to `external_clock`
`trace_input_delay`  | `0`     | Specifies the electrical delay between the FPGA pin and the external device
`trace_uncertainty`  | `0`     | Specifies the electrical delay uncertainty between the FPGA pin and the external device

### Timing Constraints

As indicated in the table above, net attributes can be used to specify
external peripheral timing requirements.  These timing attributes are
specified as if there are no PCB or trace delays.  When the net is connected
to a pin, the trace delays and pin capacitance specified by the pin attributes
are used in conjunction with the net timing attributes to calculate the
appropriate external timing constraints.  An example is provided below:

```alcha
  // PCB trace delays in the pin definitions:
  pin(4)<voltage = 3.3, capacitance = 10e-12,
         external_min_delay = 500e-12, external_max_delay = 1e-9,
         location = ["U7", "T7", "V8", "T8"]> SD_DAT;

  // Peripheral specifications in the driver class:
  net(4)<external_clock = ~Clock, external_max_delay = 14e-9                      > DataIn;
  net(4)<external_clock =  Clock, external_setup     =  5e-9, external_hold = 5e-9> DataOut;

  // Assign the nets to the pin
  DataIn         = SD_DAT.pad;
  SD_DAT.driver  = DataOut;
  SD_DAT.enable  = DataEnable;
```

External delays are always referenced to a clock, even if the external delay
is purely combinational.  This is so that the timing constraints are
compatible with the Synopsis Design Constraints format, which is the
industry-preferred standard.

### Bus Connections

ALCHA does not support high-impedance nets directly.  Pins can be set to
high-impedance by disabling the driver (see the Pins section for details).  In
order to emulate a bus that has a set of tri-state drivers, the nets should be
gated through AND gates and then combined through an OR gate to drive the bus.

## Pins

### Definition

Pins in ALCHA are specified by means of the `pin` keyword.  Pins can have
various attributes.  A short example is presented below:

```alcha
  pin<frequency = 50e6, voltage = 2.5, location = "H12"> Clock;
  pin<                  voltage = 1.2, location = "P11"> Key;
  pin<                  voltage = 2.5, location = "F7" > LED;
```

A pin is a structured object that consists of three nets: the `pad` (physical
FPGA pin), the `driver` (input to the pin buffer) and the `enable` (driver
enable).  In order to set a pin to output, the driver must be enabled by
assigning a constant high to the `enable` net.  Bidirectional pins are created
by assigning an expression to the `enable` net.

When, at the time of circuit synthesis, the `enable` net is undefined, the
compiler infers the direction of the pin.  If there is an expression assigned
to the `driver` net, the pin is an output and the driver is enabled.  If the
`driver` net is also undefined, the pin is an input.

When the developer assigns directly to the pin name, the expression is
actually assigned to the `driver` net.  When the pin is read, the `pad` net is
read.  This is sometimes problematic, as illustrated in the following example:

```alcha
  // Define I2C bus pins (initialisers assign to the driver net)
  pin S_Clk  = 1;
  pin S_Data = 0;

  // Assign pin direction
  S_Data.enable = 0;

  // Add a device to the bus (wrong)
  S_Clk  &=  ThisClock; // Equivalent to S_Clk .driver = S_Clk .pad &  ThisClock;
  S_Data |= ~ThisData;  // Equivalent to S_Data.driver = S_Data.pad | ~ThisData;

  // Add a device to the bus (correct)
  S_Clk .driver &=  ThisClock;
  S_Data.enable |= ~ThisData;

  // Add another device to the bus
  S_Clk.driver  &=  ThatClock;
  S_Data.enable |= ~ThatData;

  // Read the Data line
  pin LED = S_Data; // Equivalent to LED.driver = S_Data.pad
```

When ignoring the first (wrong) assignment, the code above is equivalent to
the following Verilog:

```verilog
  module TopLevel(
    output S_Clk,
    inout  S_Data,
    output LED
  );

  wire   S_Data_enable;
  assign S_Data = S_Data_enable ? 1'b0 : 1'bZ;

  assign S_Clk         = 1'b1 &  ThisClock &  ThatClock;
  assign S_Data_enable = 1'b0 | ~ThisData  | ~ThatData;

  assign LED = S_Data;
```

Typically, the compiler will remove the and-with-one and or-with-zero.  It is
kept here to show the relationship between the ALCHA and Verilog more clearly.

### Attributes

The table below summarises pin attributes and their default values:

Attribute     | Default    | Description
---------     | -------    | -----------
`location`    | `"None"`   | The physical pin number on the target device.  In the case of a differential pair, the pin location is specified in `"PP-NN"` format, where `PP` is the positive pin number and `NN` the negative pin number.
`voltage`     | `3.3`      | The voltage of the I/O bank.
`current`     | `5e-3`     | The intended maximum current of the pin, in case of an output.
`capacitance` | `10e-12`   | The load capacitance of the pin, in case of an output.
`standard`    | `"LVCMOS"` | The logic standard of the pin.
`termination` | `false`    | Indicates whether or not the pin should enable the internal termination.  Takes only `"true"` or `"false"` values.
`frequency`   | `"None"`   | In the case of a clock input, the frequency of the clock.
`jitter`      | `0`        | In the case of a clock input, the clock jitter.

### Pin Vectors

When specifying pin locations for bit-vectors or pin arrays, the `location`
attribute contains a comma-separated list of pins.  Pin locations are
specified most-significant bit first, as follows:

```alcha
  // Assign H9 to LED(7), H8 to LED(6), ..., L7 to LED(0)
  pin(8)<location = ["H9", "H8", "B6", "A5", "E9", "D8", "K6", "L7"]> LED;
```

Pin arrays are handled in similar fashion.  The first (0-index) pin
location(s) are specified first, then the next (1-index), etc.  All the
locations are comma-separated.

The same location can be assigned to different pin objects.  This is useful
when the physical board can have different functions on the same pin, such as
a choice between single-ended or LVDS.  This is illustrated below:

```alcha
  pin(4) <
    standard    = "LVDS",
    termination =  true,
    location    = ["L12-K11", "H18-H17", "M11-L11", "N12-M12"],
  > HSMC_RX;

  group <standard = "LVCMOS">{
    pin(4) <location = ["L12", "H18", "M11", "N12"]> HSMC_RX_p;
    pin(4) <location = ["K11", "H17", "L11", "M12"]> HSMC_RX_n;
  }
```

## Groups

Often there are attributes that applies to many objects.  In this case, the
definitions can be grouped.  All child definitions inherit the attributes of
the group.  When a child definition includes an attribute that is already
defined in the group, the child definition takes precedence.  Below is an
example of a named group for pin definitions.

```alcha
  group<voltage = 3.3, capacitance = 10e-12,
        external_min_delay = 500e-12, external_max_delay = 1e-9> SD{
    pin   <location = "AB6"                   > CLK;
    pin   <location = "W8"                    > CMD;
    pin(4)<location = ["U7", "T7", "V8", "T8"]> DAT;
  }
```

In this case, the pins that are actually defined are `SD.CLK`, `SD.CMD` and
`SD.DAT`.  The group can also be anonymous, as given below. In this case, each
pin is a child of the parent group object (global, in this case).

```alcha
  group <voltage = 3.3, frequency = 50e6>{
    pin<location = "R20"> CLOCK_B5B;
    pin<location = "N20"> CLOCK_B6A;
  }
```

Nets, class instances, derived clocks, etc. can be grouped in similar fashion.

## Structures

ALCHA structures are "packed", which makes it possible to treat the structure
as if it is a bit-vector.  It is therefore possible, for example, to assign a
multi-member structure to the input port of a RAM block and then assign the
output of the RAM block to a different instance of the same structure.  The
user does not need to use concatenation in the process.

Structures are defined by means of the `struct` keyword, as follows:

```alcha
  struct double{
    net     Sign;
    net(11) Exponent;
    net(52) Mantissa;
  }
```

These structures follow the same rules as SystemVerilog structures, i.e. they
are specified from most-significant to least-significant bit.  An instance of
a structure can be used in as if it is a bit-vector.

## Enumerations

An enumeration type can be defined by means of the `enum` keyword, as illustrated below.
```alcha
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
```

### Base Type and Encoding

By default, the numerical constants associated with the enumeration start at 0
for the first element and increase by 1 for each element.  The user can,
however, assign arbitrary constants to the enumeration members.  Ordinarily,
enumerations are equivalent to the non-synthesisable type `num`. The
enumeration can, however, be used to define a synthesisable enumeration, as
shown below.

```alcha
  enum STATE {Idle, Writing, Done, Others}
  pin(STATE) PinState; // Pin enumeration
  net(STATE) SigState; // Net enumeration
```

In this case, the number of bits, or width, of the net (or pin array) is the
number of bits required to uniquely identify each enumeration value.  In the
above example, this is 2&nbsp;bits.  If the enumeration was declared with the
`encoding = "one-hot"` attribute, the vectors would be 4~bits wide.

### ALCHA Enumeration Semantics

A variable declared as an enumeration can only take values from that
enumeration.  If the LHS of an assignment is an enumeration instance, the
compiler pushes that enumeration onto the name-space stack.  If an enumeration
is used outside a variable of that type, the values of the enumeration can be
referenced through its type.  It is illegal to assign anything else to an
enumeration type. It is legal to compare an enumeration to other types,
however.  The code below shows various examples of legal and illegal statements.

```alcha
  enum Enum{A, B, C}

  num  a;
  Enum e, n;

  a = A;          // Illegal: A does not exist in this name-space
  a = Enum.A;     // Legal  : A represents an integer value
  e = B;          // Legal  : assigning a value to the enumeration
  n = e;          // Legal  : the enumeration types are the same
  e = 2;          // Illegal: must assign a value from the defined list
  a = e;          // Legal  : e is automatically cast to an integer
  if(e == B){...} // Legal  : comparing enumeration values
  if(a == B){...} // Illegal: B does not exist in this name-space
  if(e == 2){...} // Legal  : e is automatically cast to an integer
```

## Scripting Data Types

The table below summarises ALCHA scripting variable types:

Keyword | Description
------- | -----------
`byte`  | 8-bit unsigned integer typically used for binary file I/O
`char`  | 32-bit Unicode character typically used for text file I/O
`num`   | Infinite precision, rational, signed complex numbers (see [GNU MP](https://gmplib.org/) and [GNU MPFR](http://www.mpfr.org/))

When a floating-point is assigned to a `num` type, the same precision as the
floating-point is used to determine the size of the components of the rational
number.

More on scripting in the [Scripting Section](p/alcha/wiki/Scripting)

## Built-in Members

Various ALCHA types have built-in attributes.  The list below is not exhaustive.

Member      | Types           | Description
------      | -----           | -----------
`width`     | `pin`, `net`    | The number of bits in the vector.  Returns `1` for scalers.
`length`    | All array types | The number of elements in the array.
`frequency` | Clock nets      | The frequency of the clock, in Hz.

--------------------------------------------------------------------------------

