[[include repo=code path=Wiki/MarkDown/Header.md]]

[TOC]

# High-level Structures
## Memory Blocks

Memory blocks can be instantiated by means of the built-in `BRAM` class.  The basic configuration is specified by means of class parameters.  The class is used as if it was defined as follows:

    :::C++
    class BRAM(num Width1, num Depth1, num Width2 = 0, num Init[] = 0){
     num Depth2;
     num AddressWidth;

     AddressWidth = ceil(log2(Depth1));

     clk              Clock1; // Can optionally include a clock-enable signal
     net'AddressWidth Address1;
     net'Width1       ReadData1;
     net'Width1       WriteData1;
     net              WriteEnable1;

     if(Width2){
      Depth2       = Depth1 * Width1 / Width2;
      AddressWidth = ceil(log2(Depth2));
     }else{
      Width2 = Width1;
     }

     clk              Clock2; // Can optionally include a clock-enable signal
     net'AddressWidth Address2;
     net'Width2       ReadData2;
     net'Width2       WriteData2;
     net              WriteEnable2;
    }

All configurations of RAM blocks can be created from this class.

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

