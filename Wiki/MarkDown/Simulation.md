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
- [Advanced Attributes](AdvancedAttributes.md)
- [High-level Structures](HighLevelStructures.md)
- [Simulation and Verification](Simulation.md#simulation-and-verification)
  - [Simple Test Benches](#simple-test-benches)
    - [Stimulus-only Test Benches](#stimulus-only-test-benches)
    - [Concurrent Processes](#concurrent-processes)
    - [Simulation Scripting](#simulation-scripting)
  - [Model Based Verification](#model-based-verification)
    - [Emulating Modules](#emulating-modules)
    - [Emulating Imported HDL](#emulating-imported-hdl)
  - [Assertion Based Verification](#assertion-based-verification)
    - [Simple Assertions](#simple-assertions)
    - [Sequence Based Assertions](#sequence-based-assertions)
    - [Advanced Sequences](#advanced-sequences)
  - [Formal Verification](#formal-verification)
  - [Hardware in the Loop Verification](#hardware-in-the-loop-verification)
  - [Coverage](#coverage)
    - [Code Coverage](#code-coverage)
    - [Expression Coverage](#expression-coverage)
    - [Functional Coverage](#functional-coverage)
  - [Verification Frameworks](#verification-frameworks)

--------------------------------------------------------------------------------

# Simulation and Verification

The ALCHA language has been designed to have simulation and verification features that are as close to the [SystemVerilog industry standard](http://dx.doi.org/10.1109/IEEESTD.2013.6469140) as possible.  Minor modifications were applied in order to reduce verbosity and fit in with the rest of the ALCHA language.

## Simple Test Benches

It is possible, in ALCHA, to include the unit test in the same source file as the module it is meant to test.  After synthesis, the resulting net-list is simulated and verified by using the simulation and verification constructs present in the source code.

### Stimulus-only Test Benches

In order to perform a unit test, the module under test must have its inputs stimulated by test signals.  The code below shows the basics of an ALCHA `stimulus` construct, which is comparable to the body of a Verilog `initial` block.  The delay operators (`#`, `##` and `@`) mean the same thing in both languages.  There is no equivalent to a Verilog `always` block, because the same thing can achieved by using a loop.

```C++
  group<waveform = "green">{
    net(8) A = 0, B = 0, C = 0;
  }

  // Simple stimulus
  stimulus(1e-9){   // Use nanoseconds as the basic time-step
    A = 5;          // At the start of simulation, make A = 5
    #5 B = 7;       // After 5 time-steps, make B = 7
    @(posedge Clk); // Wait until the positive edge of the Clk signal
    loop #7 C++;    // Forever afterwards, wait 7 time-steps and increment C
  }

  // Clock-based stimulus
  stimulus(1e-9, Clk){ // Use nanoseconds as the basic time-step,
                       // and Clk as the default clock
    #7  A = 5;    // Wait 7 ns, then make A = 5
    ##3 B = 7;    // After 3 rising-edges of Clk, make B = 7
    loop ##1 C++; // Forever afterwards, wait 1 clock edge and increment C
  }
```

This construct is run, by the simulator, after the rest of the code has been synthesised to a circuit net-list.  If the output of the compiler is an FPGA project, rather than a simulation, the compiler produces SystemVerilog test-bench files based on the simulation constructs.

The `waveform` attribute informs the simulator that the signal should be included in the log and displayed on the waveform viewer.  Signals without this attribute are not logged, which saves time and memory.  The value can be used to format the visual representation.  In this case, the waveform should be displayed in green.

The parameters of the `stimulus` construct takes the place of the Verilog `timescale` directive and `default clocking` declaration.  The latter is optional, in which case the most recent `@(posedge ...)` statement is used to determine the current clock used by the `##` operator.

Stimulus constructs can be defined anywhere, including class and function bodies.  The stimulus construct is ignored by the synthesis tool, but implemented as a concurrent process by the simulation tool.  Every instance of the function (or class) then has its own instance of that stimulus process.

### Concurrent Processes

It is often useful to split stimulus execution into multiple threads without using a completely separate `stimulus` block.  Verilog implements this concept by means of the `fork`-`join` construct.  The code below shows the ALCHA syntax of the same structure.  The Verilog `disable` statement can be implemented by means of a `break` statement in ALCHA.

```C++
  stimulus(1e-9){
    // fork ... join_all
    { /* The first  fork */ } &&
    { /* The second fork */ } ;

    // fork ... join_any
    { /* The first  fork */ } ||
    { /* The second fork */ } ;

    // fork ... join_none
    { /* The first  fork */ } ||
    { /* The second fork */ } || {};
    /* The rest of the stimulus body */

    // Wait for Condition, or a delay, and stop the other thread when done
    {wait(Condition); break;} || {#100 break;} ;
  }
```

### Simulation Scripting

Scripting statements within simulation constructs are run during simulation time, not compile-time.  This makes it possible to write advanced test-benches, including file I/O, casting `net` types to `num` types, making use of transcendental mathematical functions, etc.

File I/O can be used to implement data injection and monitoring.  The test vectors can be generated by external tools, read by the ALCHA simulator and injected into the module under test.  The results can then be logged to file and compared with expected results.

## Model Based Verification

Model-based verification is a design methodology where the user would first write an emulation of the intended module functionality.  The user would then develop the synthesisable equivalent of the emulation (or model).  During verification, the same stimulus is injected into both synthesisable and emulation modules, and the results compared.

### Emulating Modules

Emulation also has another use.  It is often, if not always, faster to emulate a module than simulating the full synthesised net-list.  The `emulate` construct can be used for this purpose, as well as model-based verification.

It has the same syntax and semantics as the `stimulus` construct, but allows the user to overwrite synthesised nets with simulated signals.  The `stimulus` construct can only assign to nets that have constant-expressions assigned to them (the constant-expression is taken as the value at the start of simulation).  The `emulate` construct can assign to any net, thereby overwriting the RTL behaviour.

```C++
  class FFT{
    net(31, -1) Input   [256];
    net(31, -1) Output_I[256];
    net(31, -1) Output_Q[256];

    net Go, Busy;

    !! RTL implementation of the FFT...

    // This is run during simulation, instead of the RTL above
    emulate(1e-9, Clk){
      loop{
        ##1;
        if(Go){
          Busy = 1;
          num X = fft(Input) / 256;

          ##8; // The latency of the FFT engine
          Output_I = real(X);
          Output_Q = imag(X);

          while(Go) ##1;
          Busy = 0;
        }
      }
    }
  }
```

The user can make use of conditional compilation to switch between the emulated and simulated models.

### Emulating Imported HDL

The same construct can be used to emulated imported HDL.  The ALCHA simulator does not support the simulation of external VHDL or Verilog code, so the user must include an emulator in those modules that must be included in the simulation.  The code below shows an example of how to emulate an Altera ROM block.

```C++
  hdl() altsyncram(
    num  widthad_a  = 12,
    num  width_a    = 16,
    num  numwords_a = 2**widthad_a,
    char init_file[],
    !! Other "altsyncram" parameter declarations go here
  ){
    net            clock0;
    net(widthad_a) address_a;
    net(width_a  ) q_a;
    !! Other "altsyncram" port declarations go here
  }

  class CPU_ROM(net(16) Opcodes[4096]){
    !! Use scripting to write Opcodes to "CPU_ROM.mif"

    // Instantiate the ROM block
    private altsyncram ROM = altsyncram(init_file = "CPU_ROM.mif");

    // Emulate the ROM block during simulation
    ROM.{ // For convenience, push the ROM namespace onto the stack
      emulate(1e-9, clock0){
        loop ##1 q_a = Opcodes[address_a];
      }
    }

    // Declare local signals (for the convenience of users of this class)
    net     Clk;
    net(12) Address;
    net(16) Data;

    // Connect the ports
    ROM.clock0    = Clk;
    ROM.address_a = Address;
    Data          = ROM.q_a;
  }
```

## Assertion Based Verification

One of the more powerful methods of verification, provided by SystemVerilog, is assertion-based verification.  The verification engineer would write a list of rules, based on the system specification.  The verification tool then checks that those rules hold true for every step of the simulation.

### Simple Assertions

The simplest way to write an assertion is to include it in a `stimulus` construct.  It checks the state of the system at that point in the simulation, by means of a semicolon-separated list of boolean expressions.  The code below provides an example.

```C++
  net(8) Adder(net(8) A, net(8) B, net C){
    :(C, Adder) = A + B;
  }
  net    c;
  net(8) a, b, y = Adder(a, b, c);

  stimulus(1e-9){
    #1 a = 0x00; b = 0x00; assert{y == 0x00; c == 0; };
    #1 a = 0xFF; b = 0x01; assert{y == 0x00; c == 1; };
    #1 a = 0x01; b = 0xFF; assert{y == 0x00; c == 1; };
    #1 a = 0xFF; b = 0xFF; assert{y == 0xFE; c == 1; };
    #1 a = 0x7F; b = 0x80; assert{y == 0xFF; c == 0; };
  }
```

Another option is to write the assertions outside the stimulus construct.  Assertion statements of this form would typically take the form shown below.  The `|->` operator is used to specify rules of the form: ``when the left-hand-side is true, then the right-hand-side must also hold true''.

```C++
  assert{
    a == 0x00 && b == 0x00 |-> y == 0x00 && c == 0;
    a == 0xFF && b == 0x01 |-> y == 0x00 && c == 1;
    a == 0x01 && b == 0xFF |-> y == 0x00 && c == 1;
    a == 0xFF && b == 0xFF |-> y == 0xFE && c == 1;
    a == 0x7F && b == 0x80 |-> y == 0xFF && c == 0;
  }
```

### Sequence Based Assertions

SystemVerilog sequences can be compared to regular expressions.  The verification tool matches the given sequence expression against the simulated waveforms.  A hand-shake transaction might be specified as shown in below.  As with SystemVerilog, the signals are sampled one simulation time-step before the clock edge being tested, thereby ensuring that the system state is consistent with a hardware implementation.

```C++
  // A simple sequence:
  sequence Handshake{
    Go ##[1..5] Busy ##[1.. 10] !Go ##[1..100] !Busy;
  }

  // A sequence-based assert:
  assert(1e-9, Clk){
    if(~Reset){
           Go      |->  Handshake;
      rose(Go  )   |-> !Busy;
      fell(Busy)   |-> !Go;
      (Go & !Busy) |=>  stable(Data);
    }
  }
```

In this case, the sequence inherits the time-step and clock signal from the assertions that calls it.  It is also possible to specify a time-step and, optionally, a clock signal in the sequence declaration, which will override the assertion time-step and clock signals.

Sequences are useful in other aspects as well.  The entire sequence is essentially a boolean expression, albeit over multiple clock-cycles.  This means that it can be used within a `stimulus` construct wait statement.  The wait statement will then wait until the sequence has occurred, instead of a simple condition.

### Advanced Sequences

SystemVerilog sequences can be combined by using the `and`, `or` and `intersect` keywords.  The ALCHA equivalents are the `&&`, `||` and `&&&` operators, respectively.  These operators are generally used, in other languages, to distinguish between bit-wise and logical binary operations.  This is not required in ALCHA, because it does not support logical operations directly: the user is expected to or-reduce the operands and then use the binary operator instead.

In order to make the sequence expression syntax more expressive, SystemVerilog provides the user with helper functions, as well as repetition operators.  These include `$rose(...)`, `$fell(...)`, `$stable(...)`, `$past(...)`, `[* ... ]`,  `[-> ... ]` and  `[= ... ]`.  ALCHA supports the same functions and operators, but without the `$` prefix to the functions.

## Formal Verification

Formal verification is related to assertion-based verification.  Assertions make sure that the design adhere to the specified rules for every iteration of the simulation.  Formal verification runs the process in reverse.  It starts with the rules specified by the assertion statements, and attempts to find a case that violates those rules, without a time-based simulation.  It can be implemented as a feature of the simulation tool.

## Hardware in the Loop Verification

Hardware-in-the-loop verification entails loading the code onto a physical FPGA and verifying functionality by means of data-injection.  The concept is depicted below.

![Topology of typical hardware-in-the-loop verification](../Figures/HardwareInTheLoop.svg)

The logic analyser can be a laboratory instrument, embedded logic analyser within the FPGA firmware, or a combination of the two.  Within the ALCHA language, the user can add the `logic_analyser` attribute to any signal, which will cause the compiler to automatically generate an embedded logic analyser project file (ChipScope in Xilinx Vivado or SignalTap in Altera Quartus) that includes that signal in the sample set.

## Coverage

Code and functional coverage reports can be used to determine dead code and insufficient test cases, among other uses.

### Code Coverage

For code coverage, the simulation process logs a counter on every statement in the source code, and reports the number of times each statement was executed during simulation.  This also includes branch coverage, that tests that every possible branch of `if` and `case` statements have been tested.  ALCHA enables this by means of an attribute, as shown below.

```C++
  group<code_coverage = true>{
    !! All the code that should be tested for coverage goes here
  }

  stimulus(1e-9, Clk){
    !! Describe the stimulus here
  }
```

### Expression Coverage

Expression coverage tests that all possible combinations and permutations of an expression has been tested.  This is generally reported as a percentage, because it is often impractical to test every possibility.  Any expression that carry the `expression_coverage` attribute is included in the expression coverage report.

### Functional Coverage

For functional coverage, the process logs a counter for every functionality that the test engineer specified.  Functions include typical valid use cases, edge cases and boundary conditions, protocols, etc.

As an example, consider the case of a simplified floating-point operation.  Each input operand, as well as the output, can fall into one of six categories: NAN, -&infin;, <0, 0, >0, &infin;.  This means that there are 6<sup>3</sup>&nbsp;(216) possible combinations of functionality.  The code below shows the ALCHA syntax for specifying functional coverage.

```C++
  // Instances of the Float class
  Float() A;
  Float() B;
  Float() Y;

  // Instance of the FPU_Div class
  FPU_Div(Clk, A, B, Y) MyDiv;

  // Define coverage bins
  coverbins FloatingPointFunctions(x){
    NAN      =               x.exponent == 0xFF & x.fraction >  0;
    NegInfty = x.sign == 1 & x.exponent == 0xFF & x.fraction == 0;
    Negative = x.sign == 1 & x.exponent >  0    & x.exponent <  0xFF;
    Zero     =               x.exponent == 0    & x.fraction == 0;
    Positive = x.sign == 0 & x.exponent >  0    & x.exponent <  0xFF;
    PosInfty = x.sign == 0 & x.exponent == 0xFF & x.fraction == 0;
  }
  // Define a coverage group
  covergroup(Clk) MyDivCoverage{
    FloatingPointFunctions(A);
    FloatingPointFunctions(B);
    FloatingPointFunctions(Y);
  }
```

The `coverbins` construct defines a list of bins, defined by boolean expressions, that applies to the input parameters.  There is only one parameter (`x`) in the example, but there can in fact be any number of parameters.  The simulator issues an error when the current system state falls into more than one bin, or no bins.

The boolean expression used to define bins also supports sequences, which can be used to test protocol coverage.

The `covergroup` construct is then used to group multiple `coverbins`&nbsp;constructs into a functional coverage description.  In the example, it defines the 216&nbsp;bins that cover all combinations of input and output categories.

## Verification Frameworks

There are various simulation methodologies and industry standards.  One of the more popular ones is [UVM](https://s3.amazonaws.com/verificationacademy-news/DVCon2015/Papers/dvcon-2015_UVM-Rapid-Adoption-A-Practical-Subset-of-UVM-Paper.pdf) (Universal Verification Methodology).  It is an Accellera standard, implemented by means of a SystemVerilog library of classes.  It would be possible to implement an ALCHA based UVM library that is equivalent to the current SystemVerilog version.

--------------------------------------------------------------------------------

