[[include repo=code path=Wiki/MarkDown/Header.md]]

[TOC]

# High-level Structures
## Memory Blocks

Memory blocks can be instantiated by means of built-in classes to represent the various configurations of block RAM.  The various configurations, and the corresponding classes, are summarised in the table below:

Configuration    | Class
-------------    | -----
ROM, 1-port      | `ROM1(int Width, int Depth, clk Clock, int Init[])`
ROM, 2-port      | `ROM2(int Width, int Depth, clk Clock1, clk Clock2, int Init[])`
RAM, 1-port      | `RAM1(int Width, int Depth, clk Clock, int Init[])`
RAM, 1W, 1R port | `RAM_RW(int Width, int Depth, clk Clock1, clk Clock2, int Init[])`
RAM, full 2-port | `RAM2(int Width, int Depth, clk Clock1, clk Clock2, int Init[])`
FIFO,            | `FIFO(int Width, int Depth, clk Clock1, clk Clock2, int Init[])`

The other nets are implemented as member nets, not part of the class constructor.

## Megafunctions and External HDL Modules

An FPGA project often makes use of FPGA features that are not directly supported by ALCHA.  Such features might include, among others, PCI-Express interfaces, DDR memory interfaces and embedded processors.  These are called "megafunctions", after the Altera nomenclature.  Megafunctions are both vendor and device specific.

Megafunctions and other external HDL modules can be imported into the ALCHA project by means of the `hdl` construct, as illustrated below.  Imported HDL modules are not included in simulation.

    :::C++
    hdl("Library/RS232.v", "Library/RS232_Rx.v", "Library/RS232_Tx.v") RS232_V(
     CountBits =  5; // Parameters 
     Count0_5  =  8; 
     Count1    = 17; 
     Count1_5  = 25;
    ){
     net nReset;
     net Clk;

     net'8 TxData;
     net   Send;
     net   Busy;

     net   DataReady;
     net'8 RxData;
     net   Ack;
      
     net Tx;
     net Rx;
    }

    class RS232(clk Clock, net Reset, int BAUD): RS232_V(
     ceil(log2(round(Clock.Frequency / BAUD * 1.5))),
               round(Clock.Frequency / BAUD / 2),
               round(Clock.Frequency / BAUD),
               round(Clock.Frequency / BAUD * 1.5)
    ){
     nReset = !Reset;
     Clk    = Clock;
    }

    pin<frequency = "50 MHz"> Clk;
    pin                       Reset;
    pin                       RS232_Tx;
    pin                       RS232_Rx;

    RS232 MyUART = RS232(Clk, Reset, 9600);
    RS232_Tx  = MyUART.Tx;
    MyUART.Rx = RS232_Rx;

Whether the nets defined in the body of the construct describe input, output or bidirectional ports is inferred from their use.  The compiler keeps track of expressions involving that net.  If it occurs only on the left-hand side of assignments, it is an input port.  If it occurs only on the right-hand side of assignments, it is an output port.  If it occurs on both sides, it is bidirectional.  An example of assigning I2C pins to an HDL module is presented below:

    :::C++
    hld("Library/LTC2991.v") LTC2991_Driver(){
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

[[include repo=code path=Wiki/MarkDown/Footer.md]]

