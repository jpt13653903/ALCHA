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

module Mult(
 input Clk,

 input      [18:0]A, // 19-bit, 2's compliment
 input      [18:0]B, // 19-bit, 2's compliment
 output reg [36:0]Y  // 37-bit, 2's compliment
);
//------------------------------------------------------------------------------

wire [18:0]neg_A = -A;
wire [18:0]neg_B = -B;

reg            A_1,     A_2; // Sign bit only
reg            B_1,     B_2; // Sign bit only
reg  [18:0]neg_A_1, neg_A_2;
reg  [18:0]neg_B_1, neg_B_2;

reg  [17:0]abs_A;
reg  [17:0]abs_B;
reg  [35:0]abs_Y;
//------------------------------------------------------------------------------

// Stage 0 -- Pipeline input and absolute value
always @(posedge Clk) begin
     A_1 <=     A[18];
     B_1 <=     B[18];
 neg_A_1 <= neg_A;
 neg_B_1 <= neg_B;

 abs_A <= A[18] ? neg_A[17:0] : A[17:0];
 abs_B <= B[18] ? neg_B[17:0] : B[17:0];
//------------------------------------------------------------------------------

 // Stage 1 -- Pipeline input and multiply
     A_2 <=     A_1;
     B_2 <=     B_1;
 neg_A_2 <= neg_A_1;
 neg_B_2 <= neg_B_1;

 abs_Y   <= abs_A * abs_B;
//------------------------------------------------------------------------------

 // Stage 2 -- Apply the correct sign to the output
 case({(neg_A_2[18] & A_2), (neg_B_2[18] & B_2)})
  2'b00  : Y <= (A_2 ^ B_2) ? (-{1'b0, abs_Y}) : {1'b0, abs_Y};
  2'b01  : Y <= {neg_A_2, 18'd0};
  2'b10  : Y <= {neg_B_2, 18'd0};
  default: Y <= 37'hF_FFFF_FFFF;
 endcase
end
//------------------------------------------------------------------------------

endmodule
//------------------------------------------------------------------------------

