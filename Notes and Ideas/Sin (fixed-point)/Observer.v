//==============================================================================
// Copyright (C) John-Philip Taylor
// jpt13653903@gmail.com
//
// This file is part of Motor Driver
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

module Observer(
 input Clk,
 input Sync,
 input Reset,

 input      [ 2:0]Hall,
 output reg [17:0]Phase,     // [0, 2*pi)
 output reg [18:0]Frequency, // 2's compliment [-50e6/2^18, 50e6/2^18) Hz

 output [17:0]ApproxPhase
);
//------------------------------------------------------------------------------

assign ApproxPhase = Hall_Phase;
//------------------------------------------------------------------------------

reg  [31:0]Period_t[5:0]; // [0, 6*2^32/50e6)
reg  [31:0]Period;        // [0,   2^32/50e6)
reg  [31:0]Period_Count;  // [0, 6*2^32/50e6)

wire [31:0]Frequency_t;  // [0, 50e6/2^12)

Divide Divide1(
 Clk, Reset,
 64'h1000_0000_0000, // 1
 Period,
 Frequency_t
);
//------------------------------------------------------------------------------

wire [2:0]Hall_1;
reg  [2:0]Hall_2;

Debounce #(3, 15) Hall_Debounce(Clk, Reset, Hall, Hall_1);

always @(posedge Clk) Hall_2 <= Hall_1;
//------------------------------------------------------------------------------

reg [17:0]Hall_Phase;
reg [17:0]Next_Phase;
reg [17:0]Diff_Phase;

reg Edge;
reg Direction_t;
reg Direction;

always @(*) begin
 Period = (Period_t[0] + Period_t[1]) +
          (Period_t[2] + Period_t[3]) +
          (Period_t[4] + Period_t[5]);

 case(Hall_1)
  3'b011 : Hall_Phase = 18'h0_0000; //   0 deg
  3'b010 : Hall_Phase = 18'h0_AAAB; //  60 deg
  3'b110 : Hall_Phase = 18'h1_5555; // 120 deg
  3'b100 : Hall_Phase = 18'h2_0000; // 180 deg
  3'b101 : Hall_Phase = 18'h2_AAAB; // 240 deg
  3'b001 : Hall_Phase = 18'h3_5555; // 300 deg
  default: Hall_Phase = 18'hX_XXXX;
 endcase

 if(Direction) Next_Phase = Phase - {1'b0, Frequency_t[31:15]};
 else          Next_Phase = Phase + {1'b0, Frequency_t[31:15]};
 Diff_Phase = Next_Phase - Hall_Phase;

 case({Hall_2, Hall_1})
  6'b011_010,
  6'b010_110,
  6'b110_100,
  6'b100_101,
  6'b101_001,
  6'b001_011: begin // Positive
   Edge        = 1'b1;
   Direction_t = 1'b0;
  end

  6'b011_001,
  6'b001_101,
  6'b101_100,
  6'b100_110,
  6'b110_010,
  6'b010_011: begin // Negative
   Edge        = 1'b1;
   Direction_t = 1'b1;
  end

  default: begin
   Edge        = 1'b0;
   Direction_t = 1'bX;
  end
 endcase
end
//------------------------------------------------------------------------------

always @(posedge Clk) begin
 if(Edge && (|Period_Count[31:15])) begin
  Period_t[0]  <= Period_Count;
  Period_t[1]  <= Period_t[0];
  Period_t[2]  <= Period_t[1];
  Period_t[3]  <= Period_t[2];
  Period_t[4]  <= Period_t[3];
  Period_t[5]  <= Period_t[4];

  Direction    <= Direction_t;
  Period_Count <= 0;

  if(Direction_t) Phase <= Hall_Phase + 18'h0_5555;
  else            Phase <= Hall_Phase - 18'h0_5555;

 end else begin
  if(Sync) begin // Synchronised to PWM @ 24 kHz
   if(Diff_Phase[17]) begin
    if(Diff_Phase < 18'h3_AAAB) Phase <= Hall_Phase - 18'h0_5555;
    else                        Phase <= Next_Phase;
   end else begin
    if(Diff_Phase > 18'h0_5555) Phase <= Hall_Phase + 18'h0_5555;
    else                        Phase <= Next_Phase;
   end
  end

  if(Period_Count > Period_t[0] && (&Period_Count[19:0])) begin
   Period_t[0] <= Period_Count;
   Period_t[1] <= Period_t[0];
   Period_t[2] <= Period_t[1];
   Period_t[3] <= Period_t[2];
   Period_t[4] <= Period_t[3];
   Period_t[5] <= Period_t[4];
  end

  if(~&Period_Count) Period_Count <= Period_Count + 1'b1;
 end

 // No need for limiting, because it's within practical values
 if(Direction) Frequency <= -{1'b0, Frequency_t[25:8]};
 else          Frequency <=  {1'b0, Frequency_t[25:8]};
end
//------------------------------------------------------------------------------

endmodule
//------------------------------------------------------------------------------

