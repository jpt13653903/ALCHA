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
- [Scripting Features](Scripting.md)
- [Advanced Attributes](AdvancedAttributes.md)
- [High-level Structures](#high-level-structures)
  - [Megafunctions and External HDL Modules](#megafunctions-and-external-hdl-modules)
- [Simulation and Verification](Simulation.md)

--------------------------------------------------------------------------------

# High-level Structures

## Megafunctions and External HDL Modules

An FPGA project often makes use of FPGA features that are not directly
supported by ALCHA.  Such features might include, among others, PCI-Express
interfaces, DDR memory interfaces and embedded processors.  These are called
"megafunctions", after the Altera nomenclature.  Megafunctions are both vendor
and device specific.

Megafunctions and other external HDL modules can be imported into the ALCHA
project by means of the `hdl` construct, as illustrated below.  Imported HDL
modules are not included in simulation.

```alcha
    hdl("Library/RS232.v", "Library/RS232_Rx.v", "Library/RS232_Tx.v") RS232_V(
        CountBits =  5; // Parameters
        Count0_5  =  8;
        Count1    = 17;
        Count1_5  = 25;
    ){
        net    nReset;
        net    clk;

        net(8) txData;
        net    send;
        net    busy;

        net    dataReady;
        net(8) rxData;
        net    ack;

        net    tx;
        net    rx;
    }

    class RS232(net clock, net reset, num baud): RS232_V(
        ceil(log2(round(clock'frequency / baud * 1.5))),
        round(clock'frequency / baud / 2),
        round(clock'frequency / baud),
        round(clock'frequency / baud * 1.5)
    ){
        nReset = ~reset;
        clk    =  clock;
    }

    pin<frequency = 50e6> ipClk;
    pin                   ipReset;
    pin                   opRS232_Tx;
    pin                   ipRS232_Rx;

    RS232(ipClk, ipReset, 9600) myUART;
    opRS232_Tx = myUART.tx;
    myUART.rx  = ipRS232_Rx;
```

Whether the nets defined in the body of the construct describe input, output
or bidirectional ports is inferred from their use.  The compiler keeps track
of expressions involving that net.  If it occurs only on the left-hand side of
assignments, it is an output port.  If it occurs only on the right-hand side of
assignments, it is an input port.

Bidirectional port mapping can be done by means of the `hdl_map` built-in
function, which performs the same operation as the port-mapping syntax in
VHDL and Verilog.

```alcha
    hdl("Library/LTC2991.v") LTC2991_Driver(){
        // Various other ports...

        net I2C_Clk;
        net I2C_Data;

        net error;
    }

    pin ipLTC2991_Clk;
    pin bpLTC2991_Data;
    pin opLTC2991_Error;

    LTC2991_Driver() LTC2991_Driver_Inst;
    LTC2991_Driver_Inst.I2C_Clk = ipLTC2991_Clk;
    bpLTC2991_Data.hdl_map(LTC2991_Driver_Inst.I2C_Data);
    opLTC2991_Error = LTC2991_Driver_Inst.error;
```

--------------------------------------------------------------------------------

