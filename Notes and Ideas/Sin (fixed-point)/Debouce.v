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

module Debounce #(
 parameter N = 1, // Number of signals
 parameter T = 20 // Dead-time = 2^T / 50e6
)(
 input Clk,
 input Reset,

 input      [N-1:0]Input,
 output reg [N-1:0]Output
);
//------------------------------------------------------------------------------

reg [N-1:0]Input_1;
reg [T-1:0]Counter;

always @(posedge Clk) begin
 Input_1 <= Input;

 if(&Counter) begin
  if(Input_1 != Output) begin
   Output  <= Input_1;
   Counter <= 0;
  end
  
 end else begin
  Counter <= Counter + 1'b1;
 end
end
//------------------------------------------------------------------------------

endmodule
//------------------------------------------------------------------------------

