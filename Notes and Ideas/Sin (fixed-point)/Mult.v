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

module Mult(
 input      [18:0]A, // 19-bit, 2's compliment
 input      [18:0]B, // 19-bit, 2's compliment
 output reg [36:0]Y  // 37-bit, 2's compliment
);
//------------------------------------------------------------------------------

wire [18:0]neg_A = -A;
wire [18:0]neg_B = -B;

wire [17:0]abs_A = A[18] ? neg_A[17:0] : A[17:0];
wire [17:0]abs_B = B[18] ? neg_B[17:0] : B[17:0];
wire [35:0]abs_Y = abs_A * abs_B;

always @(*) begin
 case({(neg_A[18] & A[18]), (neg_B[18] & B[18])})
  2'b00  : Y = (A[18] ^ B[18]) ? (-{1'b0, abs_Y}) : {1'b0, abs_Y};
  2'b01  : Y = {neg_A, 18'd0};
  2'b10  : Y = {neg_B, 18'd0};
  default: Y = 37'hF_FFFF_FFFF;
 endcase
end
//------------------------------------------------------------------------------

endmodule
//------------------------------------------------------------------------------

