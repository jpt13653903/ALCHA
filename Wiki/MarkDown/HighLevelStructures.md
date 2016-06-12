[[include repo=code path=Wiki/MarkDown/Header.md]]

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

The other nets are implemented as member nets.

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
     in net nReset;
     in net Clk;
    
     in  net'8 TxData;
     in  net   Send;
     out net   Busy;
    
     out net   DataReady;
     out net'8 RxData;
     in  net   Ack;
      
     out net Tx;
     in  net Rx;
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

    in  pin<frequency = "50 MHz"> Clk;
    in  pin                       Reset;
    out pin                       RS232_Tx;
    in  pin                       RS232_Rx;

    RS232(Clk, Reset, 9600) MyUART;
    RS232_Tx = MyUART.Tx;
    MyUART.Rx = RS232_Rx;

[[include repo=code path=Wiki/MarkDown/Footer.md]]

