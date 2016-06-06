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

module Divide(
 input Clk,
 input Reset,

 input      [63:0]A, // Unsigned
 input      [31:0]B, // Unsigned
 output reg [31:0]Y  // Unsigned
);
//------------------------------------------------------------------------------

reg [63:0]Rem;
reg [62:0]Den;
reg [31:0]Q;
//------------------------------------------------------------------------------

reg [4:0]Count;

reg        State;
localparam Start = 1'b0;
localparam Go    = 1'b1;

reg Reset_1;
always @(posedge Clk) begin
 Reset_1 <= Reset;

 if(Reset_1) begin
  State <= Start;
//------------------------------------------------------------------------------

 end else begin
  case(State)
   Start: begin
    Y     <=  Q;
    Rem   <=  A;
    Den   <= {B, 31'd0};
    Q     <=  0;
    Count <=  0;
    State <=  Go;
   end

   Go: begin
    if(Den <= Rem) begin
     Q   <= {Q[30:0], 1'b1};
     Rem <= Rem - Den;
    end else begin
     Q <= {Q[30:0], 1'b0};
    end
    Den <= {1'b0, Den[62:1]};

    if(&Count) State <= Start;
    Count <= Count + 1'b1;
   end

   default:;
  endcase
 end
end
//------------------------------------------------------------------------------

endmodule
//------------------------------------------------------------------------------

