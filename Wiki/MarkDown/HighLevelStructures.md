[[include repo=doccode path=MarkDown/Header.md]]

[TOC]

# High-level Structures
## Memory Blocks
Memory blocks can be instantiated by means of built-in classes to represent the various configurations of block RAM.  The various configuration, and the corresponding classes, are summarised in the table below:

Configuration    | Class
-------------    | -----
ROM, 1-port      | `ROM1(int Width, int Depth, clk Clock, int Init[])`
ROM, 2-port      | `ROM2(int Width, int Depth, clk Clock1, clk Clock2, int Init[])`
RAM, 1-port      | `RAM1(int Width, int Depth, clk Clock, int Init[])`
RAM, 1W, 1R port | `RAM_RW(int Width, int Depth, clk Clock1, clk Clock2, int Init[])`
RAM, full 2-port | `RAM2(int Width, int Depth, clk Clock1, clk Clock2, int Init[])`
FIFO,            | `FIFO(int Width, int Depth, clk Clock1, clk Clock2, int Init[])`

The other signals are implemented as member signals.

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

[[include repo=doccode path=MarkDown/Footer.md]]

