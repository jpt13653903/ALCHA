// Platform descriptor for the Cyclone V GX Starter Kit
//------------------------------------------------------------------------------

#ifndef Platform_h
#define Platform_h
//------------------------------------------------------------------------------

// Target
target<
 vendor = "Altera",
 series = "Cyclone V GX",
 device = "5CGXFC5C6F27C7N",
 board  = "Cyclone V GX Starter Kit"
> MyTarget;
//------------------------------------------------------------------------------

/* 
 Defaults:
  voltage  : 3.3 V
  current  : Quartus default - typically 4 mA
  standard : CMOS
  direction: Bidirectional

 Names are concatenated down the hierarchy.  Properties are inherited by 
 children.

 Everything outside an pin_array must have a name.  Everything inside an 
 pin_array must have an index.  Entries cannot specify both name and index.

 Pins can be shared, i.e. two different pins can have the same number.  It
 is, however, an error when the two specifications have different voltages.
 In the design, only one of the two may be used.

 The presence of the "frequency" property marks it as a clock input pin.

 The min_delay and max_delay properties are required for synchronous pins.  
 These specify the board trace delay. The module definition should contain 
 other timing details assuming zero delay between FPGA and peripheral.  All
 other pins are assumed asynchronous and will be defined as false paths.
*/

pin<direction = "in", voltage = "3.3 V", number = "AB24"> CPU_RESET_n;

pin_group<direction = "in", voltage = "3.3 V", frequency = "50 MHz"> CLOCK{
 pin<number = "R20"> _50_B5B;
 pin<number = "N20"> _50_B6A;
}

pin_group<direction = "in"> U20{
 pin_group<voltage = "2.5 V"> _CLOCK{
  pin<frequency =  "50 MHz", number = "H12"> _50_B7A;
  pin<frequency =  "50 MHz", number = "M10"> _50_B8A;
  pin<
   frequency = "125 MHz",
   standard  = "LVDS"   ,
   number    = "U12"    ,
   pair      = "V12"
  > _125;
 }
 pin_array<voltage = "1.5 V", standard = "PCML"> _REFCLK{
  pin[0]<frequency = "125    MHz", number = "V6", pair = "W6">;
  pin[1]<frequency = "156.25 MHz", number = "N7", pair = "P6">;
 }
}

pin<direction = "out", voltage = "2.5 V", number = "B7" > I2C_SCL;
pin<                   voltage = "2.5 V", number = "G11"> I2C_SDA;

pin_array<direction = "in", voltage = "1.2 V"> KEY{
 pin[0]<number = "P11">;
 pin[1]<number = "P12">;
 pin[2]<number = "Y15">;
 pin[3]<number = "Y16">;
 pin[4]<number = "AB24", voltage = "3.3 V">;
}
pin_array<direction = "in", voltage = "1.2 V"> SW{
 pin[0]<number = "AC9" >;
 pin[1]<number = "AE10">;
 pin[2]<number = "AD13">;
 pin[3]<number = "AC8" >;
 pin[4]<number = "W11" >;
 pin[5]<number = "AB10">;
 pin[6]<number = "V10" >;
 pin[7]<number = "AC10">;
 pin[8]<number = "Y11" >;
 pin[9]<number = "AE19">;
}

pin_group<direction = "out", voltage = "2.5 V", current = "16 mA"> LED{
 pin_array R{
  pin[0]<number = "F7" >;
  pin[1]<number = "F6" >;
  pin[2]<number = "G6" >;
  pin[3]<number = "G7" >;
  pin[4]<number = "J8" >;
  pin[5]<number = "J7" >;
  pin[6]<number = "K10">;
  pin[7]<number = "K8" >;
  pin[8]<number = "H7" >;
  pin[9]<number = "J10">;
 }
 pin_array G{
  pin[0]<number = "L7">;
  pin[1]<number = "K6">;
  pin[2]<number = "D8">;
  pin[3]<number = "E9">;
  pin[4]<number = "A5">;
  pin[5]<number = "B6">;
  pin[6]<number = "H8">;
  pin[7]<number = "H9">;
 }
}

pin_array<direction = "out", current = "16 mA"> HEX{
 pin_array[0]<voltage = "1.2 V"> {
  pin[0]<number = "V19">;
  pin[1]<number = "V18">;
  pin[2]<number = "V17">;
  pin[3]<number = "W18">;
  pin[4]<number = "Y20">;
  pin[5]<number = "Y19">;
  pin[6]<number = "Y18">;
 }
 pin_array[1]<voltage = "1.2 V"> {
  pin[0]<number = "AA18">;
  pin[1]<number = "AD26">;
  pin[2]<number = "AB19">;
  pin[3]<number = "AE26">;
  pin[4]<number = "AE25">;
  pin[5]<number = "AC19">;
  pin[6]<number = "AF24">;
 }
 pin_array[2]<voltage = "3.3 V"> {
  pin[0]<number = "AD7">;
  pin[1]<number = "AD6">;
  pin[2]<number = "U20">;
  pin[3]<number = "V22">;
  pin[4]<number = "V20">;
  pin[5]<number = "W21">;
  pin[6]<number = "W20">;
 }
 pin_array[3]<voltage = "3.3 V"> {
  pin[0]<number = "Y24" >;
  pin[1]<number = "Y23" >;
  pin[2]<number = "AA23">;
  pin[3]<number = "AA22">;
  pin[4]<number = "AC24">;
  pin[5]<number = "AC23">;
  pin[6]<number = "AC22">;
 }
}

pin_group<voltage = "2.5 V"> UART{
 pin<direction = "out", number = "L9"> _TX;
 pin<direction = "in" , number = "M9"> _RX;
}

pin_group<voltage = "3.3 V"> SRAM{
 pin_array<direction = "out"> _A{
  pin[ 0]<number = "B25">;
  pin[ 1]<number = "B26">;
  pin[ 2]<number = "H19">;
  pin[ 3]<number = "H20">;
  pin[ 4]<number = "D25">;
  pin[ 5]<number = "C25">;
  pin[ 6]<number = "J20">;
  pin[ 7]<number = "J21">;
  pin[ 8]<number = "D22">;
  pin[ 9]<number = "E23">;
  pin[10]<number = "G20">;
  pin[11]<number = "F21">;
  pin[12]<number = "E21">;
  pin[13]<number = "F22">;
  pin[14]<number = "J25">;
  pin[15]<number = "J26">;
  pin[16]<number = "N24">;
  pin[17]<number = "M24">;
 }
 pin_array _D{
  pin[ 0]<number = "E24">;
  pin[ 1]<number = "E25">;
  pin[ 2]<number = "K24">;
  pin[ 3]<number = "K23">;
  pin[ 4]<number = "F24">;
  pin[ 5]<number = "G24">;
  pin[ 6]<number = "L23">;
  pin[ 7]<number = "L24">;
  pin[ 8]<number = "H23">;
  pin[ 9]<number = "H24">;
  pin[10]<number = "H22">;
  pin[11]<number = "J23">;
  pin[12]<number = "F23">;
  pin[13]<number = "G22">;
  pin[14]<number = "L22">;
  pin[15]<number = "K21">;
 }
 pin_group<direction = "out"> {
  pin<number = "N23"> CE_n;
  pin<number = "M22"> OE_n;
  pin<number = "G25"> WE_n;
  pin<number = "H25"> LB_n;
  pin<number = "M25"> UB_n;
 }
}

pin_group<voltage = "1.2 V"> DDR2LP{
 pin_array<direction = "out", standard = "HSUL"> _CA{
  pin[0]<number = "AE6" >;
  pin[1]<number = "AF6" >;
  pin[2]<number = "AF7" >;
  pin[3]<number = "AF8" >;
  pin[4]<number = "U10" >;
  pin[5]<number = "U11" >;
  pin[6]<number = "AE9" >;
  pin[7]<number = "AF9" >;
  pin[8]<number = "AB12">;
  pin[9]<number = "AB11">;
 }
 pin_array<standard = "HSUL"> _DQ{
  pin[ 0]<number = "AA14">;
  pin[ 1]<number =  "Y14">;
  pin[ 2]<number = "AD11">;
  pin[ 3]<number = "AD12">;
  pin[ 4]<number =  "Y13">;
  pin[ 5]<number =  "W12">;
  pin[ 6]<number = "AD10">;
  pin[ 7]<number = "AF12">;
  pin[ 8]<number = "AC15">;
  pin[ 9]<number = "AB15">;
  pin[10]<number = "AC14">;
  pin[11]<number = "AF13">;
  pin[12]<number = "AB16">;
  pin[13]<number = "AA16">;
  pin[14]<number = "AE14">;
  pin[15]<number = "AF18">;
  pin[16]<number = "AD16">;
  pin[17]<number = "AD17">;
  pin[18]<number = "AC18">;
  pin[19]<number = "AF19">;
  pin[20]<number = "AC17">;
  pin[21]<number = "AB17">;
  pin[22]<number = "AF21">;
  pin[23]<number = "AE21">;
  pin[24]<number = "AE15">;
  pin[25]<number = "AE16">;
  pin[26]<number = "AC20">;
  pin[27]<number = "AD21">;
  pin[28]<number = "AF16">;
  pin[29]<number = "AF17">;
  pin[30]<number = "AD23">;
  pin[31]<number = "AF23">;
 }
 pin_array<standard = "Differential HSUL"> _DQS{
  pin[0]<number = "V13", pair = "W13">;
  pin[1]<number = "U14", pair = "V14">;
  pin[2]<number = "V15", pair = "W15">;
  pin[3]<number = "W16", pair = "W17">;
 }
 pin_array<direction = "out", standard = "HSUL"> _DM{
  pin[0]<number = "AF11">;
  pin[1]<number = "AE18">;
  pin[2]<number = "AE20">;
  pin[3]<number = "AE24">;
 }
 pin<
  direction = "out", 
  standard  = "Differential HSUL", 
  number    = "N10", 
  pair      = "P10"
 > _CK;
 pin_array<direction = "out", standard = "HSUL"> _CKE{
  pin[0]<number = "AF14">;
  pin[1]<number = "AE13">;
 }
 pin_array<direction = "out", standard = "HSUL"> _CS_n{
  pin[0]<number = "R11">;
  pin[1]<number = "T11">;
 }
 pin<standard = "HSUL", number = "AE11"> _OCT_RZQ;
}

pin_group<voltage = "3.3 V", min_delay = "500 ps", max_delay = "1 ns"> SD{
 pin     <direction = "out", number = "AB6"> _CLK;
 pin     <                   number = "W8" > _CMD;
 pin_array _DAT{
  pin[0]<number = "U7">;
  pin[1]<number = "T7">;
  pin[2]<number = "V8">;
  pin[3]<number = "T8">;
 }
}

pin_group HDMI_TX{
 pin_array<direction = "out", voltage = "3.3 V"> _D{
  pin[ 0]<number =  "V23">;
  pin[ 1]<number = "AA26">;
  pin[ 2]<number =  "W25">;
  pin[ 3]<number =  "W26">;
  pin[ 4]<number =  "V24">;
  pin[ 5]<number =  "V25">;
  pin[ 6]<number =  "U24">;
  pin[ 7]<number =  "T23">;
  pin[ 8]<number =  "T24">;
  pin[ 9]<number =  "T26">;
  pin[10]<number =  "R23">;
  pin[11]<number =  "R25">;
  pin[12]<number =  "P22">;
  pin[13]<number =  "P23">;
  pin[14]<number =  "N25">;
  pin[15]<number =  "P26">;
  pin[16]<number =  "P21">;
  pin[17]<number =  "R24">;
  pin[18]<number =  "R26">;
  pin[19]<number = "AB26">;
  pin[20]<number = "AA24">;
  pin[21]<number = "AB25">;
  pin[22]<number = "AC25">;
  pin[23]<number = "AD25">;
 }
 pin<direction = "out", voltage = "3.3 V", number = "Y25"> _CLK;
 pin<direction = "out", voltage = "3.3 V", number = "Y26"> _DE;
 pin<direction = "out", voltage = "3.3 V", number = "U26"> _HS;
 pin<direction = "out", voltage = "3.3 V", number = "U25"> _VS;
 pin<direction = "in" , voltage = "1.2 V", number = "T12"> _INT;
}

pin_group<voltage = "2.5 V"> AUD{
 pin<                   number = "C7" > _ADCLRCK;
 pin<direction = "in" , number = "D7" > _ADCDAT;
 pin<                   number = "G10"> _DACLRCK;
 pin<direction = "out", number = "H10"> _DACDAT;
 pin<direction = "out", number = "D6" > _XCK;
 pin<                   number = "E6" > _BCLK;
}

pin_group HSMC{
 pin_group<voltage = "2.5 V"> _CLK{
  pin_group _IN{
   pin_array{
    pin[0]<direction = "in"  , number = "N9" >;
    pin[1]<standard  = "LVDS", number = "G15", pair = "G14">;
    pin[2]<standard  = "LVDS", number = "L8" , pair = "K9" >;
   }
   pin<number = "G14"> _n1;
   pin<number = "K9" > _n2;
   pin<number = "G15"> _p1;
   pin<number = "L8" > _p2;
  }
  pin_group _OUT{
   pin_array<direction = "out"> {
    pin[0]<                    number = "A7" >;
    pin[1]<standard  = "LVDS", number = "A19", pair = "A18">;
    pin[2]<standard  = "LVDS", number = "A17", pair = "A16">;
   }
   pin_array _n{
    pin[1]<number = "A18">;
    pin[2]<number = "A16">;
   }
   pin_array _p{
    pin[1]<number = "A19">;
    pin[2]<number = "A17">;
   }
  }
 }
 pin_array<voltage = "2.5 V"> _D{
  pin[0]<number = "D11">;
  pin[1]<number = "H14">;
  pin[2]<number = "D12">;
  pin[3]<number = "H13">;
 }
 pin_group<voltage = "1.5 V", standard = "PCML"> _GXB{
  pin_array<direction = "in"> _RX{
   pin[0]<number = "AD2", pair = "AD1">;
   pin[1]<number = "AB2", pair = "AB1">;
   pin[2]<number = "Y2" , pair = "Y1" >;
   pin[3]<number = "V2" , pair = "V1" >;
  }
  pin_array<direction = "out"> _TX{
   pin[0]<number = "AE4", pair = "AE3">;
   pin[1]<number = "AC4", pair = "AC3">;
   pin[2]<number = "AA4", pair = "AA3">;
   pin[3]<number = "W4" , pair = "W3" >;
  }
 }
 pin_group<voltage = "2.5 V"> _RX{
  pin_array<standard = "LVDS"> {
   pin[ 0]<number = "N12", pair = "M12">;
   pin[ 1]<number = "M11", pair = "L11">;
   pin[ 2]<number = "H18", pair = "H17">;
   pin[ 3]<number = "L12", pair = "K11">;
   pin[ 4]<number = "H15", pair = "J16">;
   pin[ 5]<number = "J12", pair = "J11">;
   pin[ 6]<number = "G16", pair = "G17">;
   pin[ 7]<number = "G12", pair = "F12">;
   pin[ 8]<number = "E18", pair = "F18">;
   pin[ 9]<number = "F16", pair = "E15">;
   pin[10]<number = "E13", pair = "D13">;
   pin[11]<number = "C14", pair = "D15">;
   pin[12]<number = "E16", pair = "D16">;
   pin[13]<number = "D18", pair = "D17">;
   pin[14]<number = "E20", pair = "E19">;
   pin[15]<number = "D21", pair = "D20">;
   pin[16]<number = "B24", pair = "A24">;
  }
  pin_array _p{
   pin[ 0]<number = "N12">;
   pin[ 1]<number = "M11">;
   pin[ 2]<number = "H18">;
   pin[ 3]<number = "L12">;
   pin[ 4]<number = "H15">;
   pin[ 5]<number = "J12">;
   pin[ 6]<number = "G16">;
   pin[ 7]<number = "G12">;
   pin[ 8]<number = "E18">;
   pin[ 9]<number = "F16">;
   pin[10]<number = "E13">;
   pin[11]<number = "C14">;
   pin[12]<number = "E16">;
   pin[13]<number = "D18">;
   pin[14]<number = "E20">;
   pin[15]<number = "D21">;
   pin[16]<number = "B24">;
  }
  pin_array _n{
   pin[ 0]<number = "M12">;
   pin[ 1]<number = "L11">;
   pin[ 2]<number = "H17">;
   pin[ 3]<number = "K11">;
   pin[ 4]<number = "J16">;
   pin[ 5]<number = "J11">;
   pin[ 6]<number = "G17">;
   pin[ 7]<number = "F12">;
   pin[ 8]<number = "F18">;
   pin[ 9]<number = "E15">;
   pin[10]<number = "D13">;
   pin[11]<number = "D15">;
   pin[12]<number = "D16">;
   pin[13]<number = "D17">;
   pin[14]<number = "E19">;
   pin[15]<number = "D20">;
   pin[16]<number = "A24">;
  }
 }
 pin_group<voltage = "2.5 V"> _TX{
  pin_array<standard = "LVDS"> {
   pin[ 0]<number = "E10", pair = "E11">;
   pin[ 1]<number = "C9" , pair = "B9" >;
   pin[ 2]<number = "D10", pair = "C10">;
   pin[ 3]<number = "A12", pair = "B11">;
   pin[ 4]<number = "B10", pair = "A11">;
   pin[ 5]<number = "C20", pair = "B19">;
   pin[ 6]<number = "B15", pair = "C15">;
   pin[ 7]<number = "B22", pair = "A21">;
   pin[ 8]<number = "C13", pair = "C12">;
   pin[ 9]<number = "A8" , pair = "A9" >;
   pin[10]<number = "B12", pair = "A13">;
   pin[11]<number = "C23", pair = "C22">;
   pin[12]<number = "A14", pair = "B14">;
   pin[13]<number = "A23", pair = "A22">;
   pin[14]<number = "C17", pair = "B17">;
   pin[15]<number = "C19", pair = "C18">;
   pin[16]<number = "B21", pair = "B20">;
  }
  pin_array _p{
   pin[ 0]<number = "E10">;
   pin[ 1]<number = "C9" >;
   pin[ 2]<number = "D10">;
   pin[ 3]<number = "A12">;
   pin[ 4]<number = "B10">;
   pin[ 5]<number = "C20">;
   pin[ 6]<number = "B15">;
   pin[ 7]<number = "B22">;
   pin[ 8]<number = "C13">;
   pin[ 9]<number = "A8" >;
   pin[10]<number = "B12">;
   pin[11]<number = "C23">;
   pin[12]<number = "A14">;
   pin[13]<number = "A23">;
   pin[14]<number = "C17">;
   pin[15]<number = "C19">;
   pin[16]<number = "B21">;
  }
  pin_array _n{
   pin[ 0]<number = "E11">;
   pin[ 1]<number = "B9" >;
   pin[ 2]<number = "C10">;
   pin[ 3]<number = "B11">;
   pin[ 4]<number = "A11">;
   pin[ 5]<number = "B19">;
   pin[ 6]<number = "C15">;
   pin[ 7]<number = "A21">;
   pin[ 8]<number = "C12">;
   pin[ 9]<number = "A9" >;
   pin[10]<number = "A13">;
   pin[11]<number = "C22">;
   pin[12]<number = "B14">;
   pin[13]<number = "A22">;
   pin[14]<number = "B17">;
   pin[15]<number = "C18">;
   pin[16]<number = "B20">;
  }
 }
}

pin_array<voltage = "3.3 V"> GPIO{
 pin[ 0]<number = "T21" >;
 pin[ 1]<number = "D26" >;
 pin[ 2]<number = "K25" >;
 pin[ 3]<number = "E26" >;
 pin[ 4]<number = "K26" >;
 pin[ 5]<number = "M26" >;
 pin[ 6]<number = "M21" >;
 pin[ 7]<number = "P20" >;
 pin[ 8]<number = "T22" >;
 pin[ 9]<number = "T19" >;
 pin[10]<number = "U19" >;
 pin[11]<number = "U22" >;
 pin[12]<number = "P8"  >;
 pin[13]<number = "R8"  >;
 pin[14]<number = "R9"  >;
 pin[15]<number = "R10" >;
 pin[16]<number = "F26" >;
 pin[17]<number = "Y9"  >;
 pin[18]<number = "G26" >;
 pin[19]<number = "Y8"  >;
 pin[20]<number = "AA7" >;
 pin[21]<number = "AA6" >;
 pin[22]<number = "AD7" >;
 pin[23]<number = "AD6" >;
 pin[24]<number = "U20" >;
 pin[25]<number = "V22" >;
 pin[26]<number = "V20" >;
 pin[27]<number = "W21" >;
 pin[28]<number = "W20" >;
 pin[29]<number = "Y24" >;
 pin[30]<number = "Y23" >;
 pin[31]<number = "AA23">;
 pin[32]<number = "AA22">;
 pin[33]<number = "AC24">;
 pin[34]<number = "AC23">;
 pin[35]<number = "AC22">;
}

pin_group<voltage = "3.3 V"> Arduino{
 pin_array _IO{
  pin[ 0]<number = "E26">;
  pin[ 1]<number = "K26">;
  pin[ 2]<number = "M26">;
  pin[ 3]<number = "M21">;
  pin[ 4]<number = "P20">;
  pin[ 5]<number = "T22">;
  pin[ 6]<number = "T19">;
  pin[ 7]<number = "U19">;
  pin[ 8]<number = "U22">;
  pin[ 9]<number = "P8" >;
  pin[10]<number = "R8" >;
  pin[11]<number = "R9" >;
  pin[12]<number = "R10">;
  pin[13]<number = "F26">;
  pin[14]<number = "Y9" >;
  pin[15]<number = "G26">;
 }
 pin<number = "E26" > _RXD;
 pin<number = "K26" > _TXD;
 pin<number = "R8"  > _SS;
 pin<number = "R9"  > _MOSI;
 pin<number = "R10" > _MISO;
 pin<number = "F26" > _SCK;
 pin<number = "Y9"  > _SDA;
 pin<number = "G26" > _SCL;
 pin<number = "AB24", direction = "in"> _Reset_n;
}

pin_group<voltage = "1.2 V"> ADC{
 pin<direction = "out", number = "AB22"> _CONVST;
 pin<direction = "out", number = "AA21"> _SCK;
 pin<direction = "out", number =  "Y10"> _SDI;
 pin<direction = "in" , number =  "W10"> _SDO;
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

