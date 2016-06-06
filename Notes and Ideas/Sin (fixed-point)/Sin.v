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

module Sin(
 input Clk,
 input Reset,

 input      [18:0]Amplitude, // 19-bit, [-1, 1   )
 input      [17:0]Phase,     // 18-bit, [ 0, 2*pi)
 output reg [18:0]Output1,   // 19-bit, [-1, 1   ), 2's compliment
 output reg [18:0]Output2,   // 19-bit, [-1, 1   ), 2's compliment, +120 deg
 output reg [18:0]Output3    // 19-bit, [-1, 1   ), 2's compliment, +240 deg
);
//------------------------------------------------------------------------------

reg  [18:0]A, B;
wire [36:0]AB;
Mult Mult1(A, B, AB);
//------------------------------------------------------------------------------

reg [18:0]x; // 19-bit, [-pi/2, pi/2), 2's compliment

reg [3:0]State;
reg [3:0]Return;

localparam S1 = 4'b0000;
localparam S2 = 4'b0001;
localparam S3 = 4'b0011;

localparam Sin_Start = 4'b1000;
localparam Sin_1     = 4'b1001;
localparam Sin_2     = 4'b1011;
localparam Sin_3     = 4'b1010;
localparam Sin_4     = 4'b1110;
localparam Sin_5     = 4'b1111;
localparam Sin_6     = 4'b1101;
//------------------------------------------------------------------------------

reg Reset_1;
always @(posedge Clk) begin
 Reset_1 <= Reset;

 if(Reset_1) begin
  State  <= S1;
  Return <= 0;
//------------------------------------------------------------------------------

 end else begin
  case(State)
   S1: begin
    Output3 <= AB[36:18];
    x[0]    <= 0;
    x[17:0] <= Phase;
    Return  <= S2;
    State   <= Sin_Start;
   end

   S2: begin
    Output1 <= AB[36:18];
    x[0]    <= 0;
    x[17:0] <= Phase + 18'h1_5555;
    Return  <= S3;
    State   <= Sin_Start;
   end

   S3: begin
    Output2 <= AB[36:18];
    x[0]    <= 0;
    x[17:0] <= Phase + 18'h2_AAAB;
    Return  <= S1;
    State   <= Sin_Start;
   end
//------------------------------------------------------------------------------

   Sin_Start: begin
    case(x[17:0])
     18'h10000, 
     18'h10001, 
     18'h10010, 
     18'h10011: begin
      A     <= Amplitude;
      B     <= 19'h3_FFFF;
      State <= Return;
     end

     default: begin
      if     (x[17:0] < 18'h1_0000) x <=              {x, x[0], x[0]};
      else if(x[17:0] < 18'h3_0000) x <= 21'h8_0000 - {x, x[0], x[0]};
      else                          x <= {x, x[0], x[0]} - 21'h10_0000;

      State <= Sin_1;
     end
    endcase
   end
   
   Sin_1: begin
    A     <= x;
    B     <= x;
    State <= Sin_2;
   end
   
   Sin_2: begin
    A     <= AB[35:18];
    B     <= 19'd614;
    State <= Sin_3;
   end
   
   Sin_3: begin
    B     <= 19'd10445 - AB[36:18];
    State <= Sin_4;
   end
   
   Sin_4: begin
    B     <= 19'd84668 - AB[36:18];
    State <= Sin_5;
   end
   
   Sin_5: begin
    A     <= x;
    B     <= 19'd205887 - AB[36:18];
    State <= Sin_6;
   end
   
   Sin_6: begin
    A     <= Amplitude;
    B     <= AB[35:17]; // No clipping required
    State <= Return;
   end
//------------------------------------------------------------------------------

   default:;   
  endcase
 end
end
//------------------------------------------------------------------------------

endmodule
//------------------------------------------------------------------------------

