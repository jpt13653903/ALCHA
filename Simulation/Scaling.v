`timescale 1ns/100ps
//------------------------------------------------------------------------------

module Scaling;
//------------------------------------------------------------------------------

reg [7:0]uA;
reg [7:0]uB;

always begin
  uA = 8'h00; uB = 8'h00; #1;
  uA = 8'h00; uB = 8'h7F; #1;
  uA = 8'h00; uB = 8'h80; #1;
  uA = 8'h00; uB = 8'hFF; #1;
  uA = 8'h7F; uB = 8'h00; #1;
  uA = 8'h7F; uB = 8'h7F; #1;
  uA = 8'h7F; uB = 8'h80; #1;
  uA = 8'h7F; uB = 8'hFF; #1;
  uA = 8'h80; uB = 8'h00; #1;
  uA = 8'h80; uB = 8'h7F; #1;
  uA = 8'h80; uB = 8'h80; #1;
  uA = 8'h80; uB = 8'hFF; #1;
  uA = 8'hFF; uB = 8'h00; #1;
  uA = 8'hFF; uB = 8'h7F; #1;
  uA = 8'hFF; uB = 8'h80; #1;
  uA = 8'hFF; uB = 8'hFF; #1;
end
//------------------------------------------------------------------------------

wire signed [7:0]sA = uA;
wire signed [7:0]sB = uB;
//------------------------------------------------------------------------------

wire        [15:0]uA_uB =                uA   *                uB  ;
wire signed [15:0]sA_uB =                sA   * $signed({1'b0, uB});
wire signed [15:0]uA_sB = $signed({1'b0, uA}) *                sB  ;
wire signed [15:0]sA_sB =                sA   *                sB  ;
//------------------------------------------------------------------------------

wire        [15:0]uA_uB_u8 = uA_uB >>> 4'h8;
wire signed [15:0]sA_sB_u8 = sA_sB >>> 4'h8;
wire signed [15:0]uA_uB_s8 = uA_uB >>> $signed(4'h8);
wire signed [15:0]sA_sB_s8 = sA_sB >>> $signed(4'h8);
//------------------------------------------------------------------------------

wire        [15:0]uA_uExt = uA;
wire        [15:0]sA_uExt = sA;
wire signed [15:0]uA_sExt = uA;
wire signed [15:0]sA_sExt = sA;
//------------------------------------------------------------------------------

wire        [15:0]uA_plus_uB =                uA   +                uB  ;
wire signed [15:0]sA_plus_uB =                sA   + $signed({1'b0, uB});
wire signed [15:0]uA_plus_sB = $signed({1'b0, uA}) +                sB  ;
wire signed [15:0]sA_plus_sB =                sA   +                sB  ;
//------------------------------------------------------------------------------

endmodule
//------------------------------------------------------------------------------

