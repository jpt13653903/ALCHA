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
- [Classes](Classes.md)
- [Operator Overloading](OperatorOverloading.md)
- [Scripting Features](Scripting.md)
- [Advanced Attributes](AdvancedAttributes.md#advanced-attributes)
  - [Logic Regions](#logic-regions)
  - [Partition Assignments](#partition-assignments)
  - [Compiler Behaviour](#compiler-behaviour)
    - [Encoding Strategy](#encoding-strategy)
    - [Overflow and Rounding](#overflow-and-rounding)
- [High-level Structures](HighLevelStructures.md)
- [Simulation and Verification](Simulation.md)

--------------------------------------------------------------------------------

# Advanced Attributes

## Logic Regions

In large designs, it is often useful to specify the physical location of each
entity.  This reduces the solution space of the place-and-route problem,
thereby reducing the compilation time.  In an Altera project, this is done as
shown below.

```tcl
  set_global_assignment -name LL_WIDTH 40        -section_id MyDSP
  set_global_assignment -name LL_HEIGHT 5        -section_id MyDSP
  set_global_assignment -name LL_ORIGIN X44_Y145 -section_id MyDSP
  set_global_assignment -name LL_ENABLED ON      -section_id MyDSP
  set_global_assignment -name LL_AUTO_SIZE OFF   -section_id MyDSP
  set_global_assignment -name LL_STATE LOCKED    -section_id MyDSP
  set_global_assignment -name LL_RESERVED OFF    -section_id MyDSP
  set_global_assignment -name LL_CORE_ONLY OFF   -section_id MyDSP
  set_global_assignment -name LL_PR_REGION OFF   -section_id MyDSP

  set_instance_assignment -name       LL_MEMBER_OF MyDSP \
                          -to         *MyDSP*            \
                          -section_id MyDSP
  set_instance_assignment -name       LL_PRIORITY 1 \
                          -to         *MyDSP*       \
                          -section_id MyDSP
```

The ALCHA equivalent of this is shown below, which is significantly less
verbose.  The region is defined by means of an origin coordinate, width and
height.  The origin is on the bottom-left of the bounding box.  If no
coordinate is specified, the compiler sets the `LL_STATE` property to
`FLOATING`.  If no width or height is specified, the compiler sets the
`LL_AUTO_SIZE` property to `ON`.  The name of the region is chosen, by the
compiler, based on the name of the object the attribute is assigned to.

```alcha
    DPS(/* Parameters */)<region = "X44, Y145, W40, H5"> MyDSP;
```

## Partition Assignments

Xilinx Vivado and Altera Quartus both allow the user to break large designs
into partitions.  The vendor compiler only recompiles a partition if the
source files that describe the contents of that partition have changed, which
reduces compilation time.  Partitions come in one of three flavours, depending
of what stage of compilation is being re-used:

- **Source:**    The partition is always compiled from the source files &ndash;
                 nothing is reused.
- **Synthesis:** The synthesis net-list is reused from a previous compile, and
                 the place-and-route stage is processed again.
- **Fitter:**    The placed and routed result from the previous compile is
                 reused.  This saves significant compilation time, but
                 sometimes makes it more difficult to optimally place the
                 components of other partitions.

ALCHA attributes can be used to perform this partitioning, thereby making it
easier to move modules from one partition to another.  Any module or net can
be placed within a design partition, which is referenced by name.  The code
below shows an example where the `DFE` and `DataPacker` modules are placed in
the same partition, and the `DSP` module in another.  The `DFE` and
`DataPacker` modules uses the fitter netlist, whereas the `DSP` module uses
the synthesis netlist.

```alcha
    // Define a default -- also applies to the "Top" partition
    'partition_type = "Fitter";

    DFE()<partition = "DFE_Packer"> MyDFE;

    // This one has a type other than the defualt
    DSP()<partition = "DSP", partition_type = "Synthesis"> MyDSP;

    DataPacker()<partition = "DFE_Packer"> MyDataPacker;
```

## Compiler Behaviour

In addition to specifying various aspects of design constraints, ALCHA
attributes can also be used to locally modify compiler behaviour.

### Encoding Strategy

The ALCHA grammar does not allow the user so specify the encoding strategy
employed by enumerations and state machines.  The default encoding for
enumerations is binary (i.e. start at zero and continue counting for each
member).  The default encoding for FSMs are compiler-specific, because state
machine encoding can be optimised [in various ways][Shubert].  This behaviour
can be modified by means of the `encoding` attribute.  Valid encodings include
"binary", "Gray", "Johnson" and "one-hot".

### Overflow and Rounding

By default ALCHA expressions follow the same truncation, overflow and rounding
rules as Verilog does.  Large vectors are assigned to shorter vectors by
stripping the most-significant end, rounding is achieved through truncation
and there are no checks for overflow conditions.

If this behaviour must be changed for the current application, the user of
traditional HDL must manually implement the rounding, clipping and
sanity-check circuits.  In ALCHA, the user have the option to either implement
it manually, or make use of attributes to locally modify the compiler behaviour.

The "rounding" attribute can take one of two values: "truncate" and
"nearest".  The "overflow" attribute can take one of two values:
"wrap" and "clip".

Where overflow and rounding are concerned, assignment behaviour follows the
attribute of the target net, and type-casting behaviour follow the attribute
of the current scope.  Without type-casting, expressions are evaluated, as far
as possible, without bit-loss.

The concept is illustrated below.  The rounding operation includes an
addition, which might overflow.  If the target width is not enough to store
the carry bit, the compiler uses the current "overflow" mode to handle the
situation.

```alcha
    net(8, 16) A, B;
    net(8,  1) C, D;
    net(8,  4) X = 0.11; // binary "00.000111"

    A'rounding = "nearest";
    // B is using default "truncate" rounding

    C'overflow = "clip";
    // D is using the default "wrap" overflow

    A = X; B = X; C = X; D = X;
```

The Verilog equivalent of the above is:

```verilog
    wire [ 3:-4] A, B;
    wire [-1:-8] C, D;
    wire [ 1:-6] X = 8'b00_000111;

    // Round to nearest:
    assign A = {2'd0, X[1:-4]} + X[-5];
    // Truncate:
    assign B = {2'd0, X[1:-4]};

    always @(*) begin // Clipping circuit
        if(X[1]) begin
            if(&X[0:-1]) C = {X[-1:-6], 2'd0};
            else         C = 8'h80;
        end else begin
            if(|X[0:-1]) C = 8'h3F;
            else         C = {X[-1:-6], 2'd0};
        end
    end

    // Stripping the most significant bits
    assign D = {X[-1:-6], 2'd0};
```

--------------------------------------------------------------------------------

[Shubert]: http://users.etech.haw-hamburg.de/users/schubert/dsy/Notes/DigSys6.pdf
