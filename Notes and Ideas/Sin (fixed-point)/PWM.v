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

module PWM(
 input Clk,
 input Reset,

 input [9:0]A, B, C,

 output reg [2:0]Output,

 output reg Sync_Out // High for one clock every full period
);
//------------------------------------------------------------------------------

reg [9:0]Duty   [2:0];
reg [9:0]Counter[2:0];
reg [2:0]Direction;
//------------------------------------------------------------------------------

integer j;

reg Reset_1;
always @(posedge Clk) begin
 Reset_1  <=   Reset;
 Sync_Out <= ~|Counter[0];

 if(Reset_1) begin
  Counter[0] <= 10'h000;
  Counter[1] <= 10'h2AB;
  Counter[2] <= 10'h2AB;

  Direction <= 3'b100;
//------------------------------------------------------------------------------

 end else begin
  for(j = 0; j < 3; j = j + 1) begin
   if(Direction[j]) Counter[j] <= Counter[j] + 1'b1;
   else             Counter[j] <= Counter[j] - 1'b1;

   if(&Counter[j] || ~|Counter[j]) begin
    case(j)
     0: Duty[0] <= A;
     1: Duty[1] <= B;
     2: Duty[2] <= C;
    endcase

    if(Direction[j]) Counter[j] <= Counter[j] - 1'b1;
    else             Counter[j] <= Counter[j] + 1'b1;

    Direction[j] <= ~Direction[j];

   end else begin
    if(Direction[j]) Counter[j] <= Counter[j] + 1'b1;
    else             Counter[j] <= Counter[j] - 1'b1;
   end

   Output[j] <= Duty[j] > Counter[j];
  end
 end
end
//------------------------------------------------------------------------------

endmodule
//------------------------------------------------------------------------------

