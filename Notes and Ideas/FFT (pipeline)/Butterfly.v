//==============================================================================
// Copyright (C) John-Philip Taylor
// jpt13653903@gmail.com
//
// This file is part of a library
//
// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
//============================================================================== 

module Butterfly(
 input  Clk,
 input  Reset,

 input  Data_Valid_In,  // One sample every second clock, so data valid
 output Data_Valid_Out, // inverts every clock cycle (caller must endure this)
 output Busy,           // There is still data in the pipe

 input      [36:0]real_A , imag_A , // Input A       , 37-bit, 2's compliment [-1, 1)
 input      [36:0]real_B , imag_B , // Input B       , 37-bit, 2's compliment [-1, 1)
 input      [18:0]real_w , imag_w , // Twiddle factor, 19-bit, 2's compliment [-1, 1)
 output reg [36:0]real_Y1, imag_Y1, // (A - w*B)/2   , 37-bit, 2's compliment [-1, 1)
 output reg [36:0]real_Y2, imag_Y2  // (A + w*B)/2   , 37-bit, 2's compliment [-1, 1)
);
//------------------------------------------------------------------------------

reg  [18:0]A1, B1, A2, B2;
wire [36:0]Y1;
wire [36:0]Y2;

// Multiplier latency = 3, and the architecture is pipelined
Mult M1(Clk, A1, B1, Y1);
Mult M2(Clk, A2, B2, Y2);
//------------------------------------------------------------------------------

reg [36:0]real_A_1, imag_A_1;
reg [18:0]real_B_1, imag_B_1;
reg [18:0]real_w_1, imag_w_1;

reg [36:0]real_A_2, imag_A_2;
reg [36:0]real_A_3, imag_A_3;

reg [37:0]real_wB , imag_wB;
reg [38:0]real_Y1_t, imag_Y1_t, real_Y2_t, imag_Y2_t;
//------------------------------------------------------------------------------

reg    [8:0]Data_Valid; // The index refers to the stage number
always @(*) Data_Valid[0] = Data_Valid_In;

assign Data_Valid_Out =  Data_Valid[8];
assign Busy           = |Data_Valid;
//------------------------------------------------------------------------------

always @(posedge Clk) begin
 if(Reset) Data_Valid[8:1] <= 0;
 else      Data_Valid[8:1] <= Data_Valid[7:0];
//------------------------------------------------------------------------------

 // Stage 0 -- latch data from RAM and set up first multiplication set
 if(Data_Valid[0]) begin
  real_A_1 <= real_A;
  imag_A_1 <= imag_A;
  real_B_1 <= real_B[36:18];
  imag_B_1 <= imag_B[36:18];
  real_w_1 <= real_w;
  imag_w_1 <= imag_w;

  A1 <= real_w;
  B1 <= real_B[36:18];

  A2 <= imag_w;
  B2 <= imag_B[36:18];
//------------------------------------------------------------------------------

 // State 1 -- Set up second set of multiplication
 end else if(Data_Valid[1]) begin
  A1 <= real_w_1;
  B1 <= imag_B_1;

  A2 <= imag_w_1;
  B2 <= real_B_1;
 end
//------------------------------------------------------------------------------
 
 // Stage 2 -- Wait for multiplier and pipeline A
 if(Data_Valid[2]) begin
  real_A_2 <= real_A_1;
  imag_A_2 <= imag_A_1;
 end
//------------------------------------------------------------------------------

 // Stage 3 -- Wait for multiplier
 // No need to pipeline A here, because Data_Valid oscillates
//------------------------------------------------------------------------------

 // Stage 4 -- Pipeline A and read first multiply result
 if(Data_Valid[4]) begin
  real_A_3 <= real_A_2;
  imag_A_3 <= imag_A_2;

  real_wB <= {Y1[36], Y1} - {Y2[36], Y2};
 end
//------------------------------------------------------------------------------

 // Stage 5 -- Read second multiply result and add real parts
 if(Data_Valid[5]) begin
  imag_wB <= {Y1[36], Y1} + {Y2[36], Y2};

  real_Y1_t <= {{2{real_A_3[36]}}, real_A_3} - {real_wB[37], real_wB};
  real_Y2_t <= {{2{real_A_3[36]}}, real_A_3} + {real_wB[37], real_wB};
 end
//------------------------------------------------------------------------------

 // Stage 6 -- Add imag parts
 if(Data_Valid[6]) begin
  imag_Y1_t <= {{2{imag_A_3[36]}}, imag_A_3} - {imag_wB[37], imag_wB};
  imag_Y2_t <= {{2{imag_A_3[36]}}, imag_A_3} + {imag_wB[37], imag_wB};
 end
//------------------------------------------------------------------------------

 // Stage 7 -- do the limit and output valid (RAM latches write on next clock)
 if(Data_Valid[7]) begin
  if(real_Y1_t[38] != real_Y1_t[37]) begin
   real_Y1 <= real_Y1_t[38] ? 37'h10_0000_0000 : 37'hF_FFFF_FFFF;
  end else begin
   real_Y1 <= real_Y1_t[37:1]; // divide by 2
  end
  if(imag_Y1_t[38] != imag_Y1_t[37]) begin
   imag_Y1 <= imag_Y1_t[38] ? 37'h10_0000_0000 : 37'hF_FFFF_FFFF;
  end else begin
   imag_Y1 <= imag_Y1_t[37:1]; // divide by 2
  end
  if(real_Y2_t[38] != real_Y2_t[37]) begin
   real_Y2 <= real_Y2_t[38] ? 37'h10_0000_0000 : 37'hF_FFFF_FFFF;
  end else begin
   real_Y2 <= real_Y2_t[37:1]; // divide by 2
  end
  if(imag_Y2_t[38] != imag_Y2_t[37]) begin
   imag_Y2 <= imag_Y2_t[38] ? 37'h10_0000_0000 : 37'hF_FFFF_FFFF;
  end else begin
   imag_Y2 <= imag_Y2_t[37:1]; // divide by 2
  end
 end
end
//------------------------------------------------------------------------------

endmodule
//------------------------------------------------------------------------------

