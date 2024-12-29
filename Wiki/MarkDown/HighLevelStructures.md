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
- [High-level Structures](HighLevelStructures.md#high-level-structures)
  - [Megafunctions and External HDL Modules](#megafunctions-and-external-hdl-modules)
- [Simulation and Verification](Simulation.md)

--------------------------------------------------------------------------------

# High-level Structures

## Megafunctions and External HDL Modules

An FPGA project often makes use of FPGA features that are not directly supported by ALCHA.  Such features might include, among others, PCI-Express interfaces, DDR memory interfaces and embedded processors.  These are called "megafunctions", after the Altera nomenclature.  Megafunctions are both vendor and device specific.

Megafunctions and other external HDL modules can be imported into the ALCHA project by means of the `hdl` construct, as illustrated below.  Imported HDL modules are not included in simulation.

```alcha
  hdl("Library/RS232.v", "Library/RS232_Rx.v", "Library/RS232_Tx.v") RS232_V(
    CountBits =  5; // Parameters
    Count0_5  =  8;
    Count1    = 17;
    Count1_5  = 25;
  ){
    net    nReset;
    net    Clk;

    net(8) TxData;
    net    Send;
    net    Busy;

    net    DataReady;
    net(8) RxData;
    net    Ack;

    net    Tx;
    net    Rx;
  }

  class RS232(net Clock, net Reset, num BAUD): RS232_V(
    ceil(log2(round(Clock'frequency / BAUD * 1.5))),
    round(Clock'frequency / BAUD / 2),
    round(Clock'frequency / BAUD),
    round(Clock'frequency / BAUD * 1.5)
  ){
    nReset = ~Reset;
    Clk    =  Clock;
  }

  pin<frequency = 50e6> Clk;
  pin                   Reset;
  pin                   RS232_Tx;
  pin                   RS232_Rx;

  RS232(Clk, Reset, 9600) MyUART;
  RS232_Tx  = MyUART.Tx;
  MyUART.Rx = RS232_Rx;
```

Whether the nets defined in the body of the construct describe input, output or bidirectional ports is inferred from their use.  The compiler keeps track of expressions involving that net.  If it occurs only on the left-hand side of assignments, it is an input port.  If it occurs only on the right-hand side of assignments, it is an output port.  If it occurs on both sides, it is bidirectional.  An example of assigning I2C pins to an HDL module is presented below:

```alcha
  hdl("Library/LTC2991.v") LTC2991_Driver(){
    // Various other ports...

    net I2C_Clk;
    net I2C_Data;
  }

  pin LTC2991_Clk;
  pin LTC2991_Data;

  LTC2991_Driver LTC2991_Driver_Inst = LTC2991_Driver();
  LTC2991_Clk  = LTC2991_Driver_Inst.I2C_Clk;
  LTC2991_Data = LTC2991_Driver_Inst.I2C_Data;
  LTC2991_Driver_Inst.I2C_Data = LTC2991_Data;
```

--------------------------------------------------------------------------------

