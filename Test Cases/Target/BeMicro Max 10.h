// Platform descriptor for the Cyclone V GX Starter Kit
//------------------------------------------------------------------------------

#ifndef Platform_h
#define Platform_h
//------------------------------------------------------------------------------

// Target
target<
 vendor = "Altera",
 series = "MAX 10",
 device = "10M08DAF484C8GES",
 board  = "BeMicro Max 10"
> MyTarget;
//------------------------------------------------------------------------------

pin_group ADXL362{
 pin<number = "L14", direction = "out"> _CS;
 pin<number = "M15", direction = "in" > _INT1;
 pin<number = "M14", direction = "in" > _INT2;
 pin<number = "L18", direction = "in" > _MISO;
 pin<number = "L19", direction = "out"> _MOSI;
 pin<number = "M18", direction = "out"> _SCLK;
}
//------------------------------------------------------------------------------

pin_group<direction = "out"> AD5681R{
 pin<number = "N18"> _LDACn;
 pin<number = "L15"> _RSTn;
 pin<number = "G17"> _SCL;
 pin<number = "H17"> _SDA;
 pin<number = "N19"> _SYNCn;
}
//------------------------------------------------------------------------------

pin_group ADT7420{
 pin<number = "P13" , direction = "in" > _CT;
 pin<number = "AB14", direction = "in" > _INT;
 pin<number = "W13" , direction = "out"> _SCL;
 pin<number = "R13"                    > _SDA;
}
//------------------------------------------------------------------------------

pin_group SDRAM{
 pin_group<direction = "out">{
  pin_array _A{
   pin[ 0]<number = "T20" >;
   pin[ 1]<number = "T19" >;
   pin[ 2]<number = "T18" >;
   pin[ 3]<number = "AA21">;
   pin[ 4]<number = "AA22">;
   pin[ 5]<number = "U22" >;
   pin[ 6]<number = "T22" >;
   pin[ 7]<number = "R22" >;
   pin[ 8]<number = "P20" >;
   pin[ 9]<number = "P22" >;
   pin[10]<number = "U21" >;
   pin[11]<number = "P19" >;
   pin[12]<number = "N20" >;
  }
  pin_array _BA{
   pin[0]<number = "R20">;
   pin[1]<number = "T21">;
  }
  pin<number = "N21"> _CASn;
  pin<number = "N22"> _CKE;
  pin<number = "M22"> _CLK;
  pin<number = "P21"> _CSn;
  pin<number = "L22"> _DQMH;
  pin<number = "L20"> _DQML;
  pin<number = "M20"> _RASn;
  pin<number = "M21"> _WEn;
 }
 pin_array _DQ{
  pin[ 0]<number = "C22">;
  pin[ 1]<number = "D22">;
  pin[ 2]<number = "E22">;
  pin[ 3]<number = "F22">;
  pin[ 4]<number = "G22">;
  pin[ 5]<number = "H22">;
  pin[ 6]<number = "J22">;
  pin[ 7]<number = "K22">;
  pin[ 8]<number = "K21">;
  pin[ 9]<number = "J21">;
  pin[10]<number = "H21">;
  pin[11]<number = "G20">;
  pin[12]<number = "F21">;
  pin[13]<number = "E21">;
  pin[14]<number = "D21">;
  pin[15]<number = "C21">;
 }
}
//------------------------------------------------------------------------------

pin_group SFLASH{
 pin<number = "R11", direction = "out"> _ASDI;
 pin<number = "R10", direction = "out"> _CSn;
 pin<number = "P10", direction = "in" > _DATA;
 pin<number = "P11", direction = "out"> _DCLK;
}
//------------------------------------------------------------------------------

pin_array<direction = "out", current = "16 mA"> USER_LED{
 pin[1]<number = "M2" >;
 pin[2]<number = "N1" >;
 pin[3]<number = "R2" >;
 pin[4]<number = "T1" >;
 pin[5]<number = "V4" >;
 pin[6]<number = "T6" >;
 pin[7]<number = "AB4">;
 pin[8]<number = "AA5">;
}
//------------------------------------------------------------------------------

pin_array<direction = "in"> SW{
 pin[1]<number = "M1" >;
 pin[2]<number = "R1" >;
 pin[3]<number = "V5" >;
 pin[4]<number = "AB5">;
}
//------------------------------------------------------------------------------

pin_array<direction = "in"> AIN{
 pin[0]<number = "F5">;
 pin[1]<number = "E4">;
 pin[2]<number = "F4">;
 pin[3]<number = "E3">;
 pin[4]<number = "J8">;
 pin[5]<number = "G4">;
 pin[6]<number = "J9">;
 pin[7]<number = "F3">;
}
//------------------------------------------------------------------------------

pin_array EG_P{
 pin[ 1]<number = "C1" >;
 pin[ 2]<number = "D2" >;
 pin[ 3]<number = "D1" >;
 pin[ 4]<number = "D3" >;
 pin[ 5]<number = "E1" >;
 pin[ 6]<number = "F2" >;
 pin[ 7]<number = "F1" >;
 pin[ 8]<number = "G1" >;
 pin[ 9]<number = "H1" >;
 pin[10]<number = "J1" >;
 pin[11]<number = "K2" >;
 pin[12]<number = "L2" >;
 pin[13]<number = "N2" >;
 pin[14]<number = "P3" >;
 pin[15]<number = "P1" >;
 pin[16]<number = "R3" >;
 pin[17]<number = "T2" >;
 pin[18]<number = "R4" >;
 pin[19]<number = "T5" >;
 pin[20]<number = "Y1" >;
 pin[21]<number = "Y2" >;
 pin[22]<number = "AA1">;
 pin[23]<number = "AA2">;
 pin[24]<number = "Y3" >;
 pin[25]<number = "Y4" >;
 pin[26]<number = "AB6">;
 pin[27]<number = "AB7">;
 pin[28]<number = "AA8">;
 pin[29]<number = "AB8">;
 pin[35]<number = "E11">;
 pin[36]<number = "E10">;
 pin[37]<number = "D9" >;
 pin[38]<number = "E9" >;
 pin[39]<number = "E8" >;
 pin[40]<number = "D8" >;
 pin[41]<number = "E6" >;
 pin[42]<number = "D7" >;
 pin[43]<number = "C8" >;
 pin[44]<number = "C7" >;
 pin[45]<number = "D6" >;
 pin[46]<number = "C6" >;
 pin[47]<number = "D5" >;
 pin[48]<number = "C5" >;
 pin[49]<number = "C4" >;
 pin[50]<number = "H11">;
 pin[51]<number = "J10">;
 pin[52]<number = "M9" >;
 pin[53]<number = "M8" >;
 pin[54]<number = "N9" >;
 pin[55]<number = "N8" >;
 pin[56]<number = "N5" >;
 pin[57]<number = "N4" >;
 pin[58]<number = "N3" >;
 pin[59]<number = "P5" >;
 pin[60]<number = "P4" >;
}
pin<number = "R5"> EXP_PRESENT;
pin<number = "C2"> RESET_EXPn;
//------------------------------------------------------------------------------

pin_array GPIO{
 pin[ 1]<number = "B2" >;
 pin[ 2]<number = "B1" >;
 pin[ 3]<number = "C3" >;
 pin[ 4]<number = "A2" >;
 pin[ 5]<number = "B3" >;
 pin[ 6]<number = "A3" >;
 pin[ 7]<number = "B4" >;
 pin[ 8]<number = "A4" >;
 pin[ 9]<number = "B5" >;
 pin[10]<number = "A5" >;
 pin[11]<number = "B7" >;
 pin[12]<number = "A6" >;
 pin[13]<number = "A7" >;
 pin[14]<number = "A8" >;
 pin[15]<number = "A9" >;
 pin[16]<number = "B8" >;
 pin[17]<number = "B10">;
 pin[18]<number = "C9" >;
 pin[19]<number = "H12">;
 pin[20]<number = "J11">;
 pin[21]<number = "E12">;
 pin[22]<number = "D13">;
 pin[23]<number = "D14">;
 pin[24]<number = "E13">;
 pin[25]<number = "A14">;
 pin[26]<number = "B14">;
 pin[27]<number = "C14">;
 pin[28]<number = "C13">;
 pin[29]<number = "H14">;
 pin[30]<number = "J13">;
 pin[31]<number = "D17">;
 pin[32]<number = "C17">;
 pin[33]<number = "E16">;
 pin[34]<number = "E15">;
 pin[35]<number = "K14">;
 pin[36]<number = "K15">;
}
pin_array<standard = "LVDS", direction = "in"> DIFF_RX_P{
 pin[ 0]<number = "K14", pair = "K15">;
 pin[ 1]<number = "E16", pair = "E15">;
 pin[ 2]<number = "D17", pair = "C17">;
 pin[ 3]<number = "H14", pair = "J13">;
 pin[ 4]<number = "C14", pair = "C13">;
 pin[ 5]<number = "A14", pair = "B14">;
 pin[ 6]<number = "D14", pair = "E13">;
 pin[ 7]<number = "E12", pair = "D13">;
 pin[ 8]<number = "H12", pair = "J11">;
 pin[ 9]<number = "B10", pair = "C9" >;
 pin[10]<number = "A9" , pair = "B8" >;
 pin[11]<number = "A7" , pair = "A8" >;
}
pin_group I2C{
 pin<number = "AA14"                   > _SDA;
 pin<number = "AB15", direction = "out"> _SCL;
}
pin<number = "AA15"> GPIO_A;
pin<number = "Y16" > GPIO_B;
pin_array<standard = "LVDS", direction = "out"> LVDS_TX_P{
 pin[ 0]<number = "V17" , pair = "W17">;
 pin[ 1]<number = "V16" , pair = "U15">;
 pin[ 2]<number = "W15" , pair = "V14">;
 pin[ 3]<number = "W14" , pair = "V13">;
 pin[ 4]<number = "Y14" , pair = "Y13">;
 pin[ 5]<number = "AA10", pair = "Y10">;
 pin[ 6]<number = "V10" , pair = "V9" >;
 pin[ 7]<number = "AA7" , pair = "AA6">;
 pin[ 8]<number = "W8"  , pair = "W7" >;
 pin[ 9]<number = "U7"  , pair = "U6" >;
 pin[10]<number = "W6"  , pair = "W5" >;
 pin[11]<number = "W3"  , pair = "W4" >;
}
//------------------------------------------------------------------------------

pin_group PMOD{
 pin_array _A{
  pin[0]<number = "C20">;
  pin[1]<number = "D19">;
  pin[2]<number = "D18">;
  pin[3]<number = "E18">;
 }

 pin_array _B{
  pin[0]<number = "E19">;
  pin[1]<number = "F18">;
  pin[2]<number = "F20">;
  pin[3]<number = "G19">;
 }

 pin_array _C{
  pin[0]<number = "U18">;
  pin[1]<number = "U17">;
  pin[2]<number = "R18">;
  pin[3]<number = "P18">;
 }

 pin_array _D{
  pin[0]<number = "R14">;
  pin[1]<number = "R15">;
  pin[2]<number = "P15">;
  pin[3]<number = "P14">;
 }
}
//------------------------------------------------------------------------------

pin<direction = "in", frequency = "50 MHz", number = "N14"> SYS_CLK;
pin<direction = "in",                       number = "N15"> USER_CLK;
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

