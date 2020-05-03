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

main main1();

endmodule
//------------------------------------------------------------------------------

module main(
  output logic signed [  7:0]sA,
  output logic signed [ 15:0]sA_sB,
  output logic signed [ 15:0]sA_uB,
  output logic signed [  7:0]sB,
  output logic signed [  7:0]sC,
  output logic signed [  7:0]sD,
  output logic        [  7:0]uA,
  output logic signed [ 15:0]uA_sB,
  output logic        [ 15:0]uA_uB,
  output logic        [  7:0]uB,
  output logic        [  7:0]uC,
  output logic        [  7:0]uD,
  output logic        [  7:0]uE,
  output logic        [  7:0]uF
);
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

assign sA          = 1'h0;                                           // ../TestCases/Combinational/main.alc +72
assign sA_sB       = (sA) * (sB);                                    // ../TestCases/Combinational/main.alc +63
assign sA_uB       = (sA) * $signed({1'b0, (uB)});                   // ../TestCases/Combinational/main.alc +61
assign sB          = 7'h7F;                                          // ../TestCases/Combinational/main.alc +73
assign sC          = 8'sh81;                                         // ../TestCases/Combinational/main.alc +74
assign sD          = 8'sh80;                                         // ../TestCases/Combinational/main.alc +75
assign uA          = 1'h0;                                           // ../TestCases/Combinational/main.alc +65
assign uA_sB       = $signed({1'b0, (uA)}) * (sB);                   // ../TestCases/Combinational/main.alc +62
assign uA_uB       = (uA) * (uB);                                    // ../TestCases/Combinational/main.alc +60
assign uB          = 7'h7F;                                          // ../TestCases/Combinational/main.alc +66
assign uC          = 8'h80;                                          // ../TestCases/Combinational/main.alc +67
assign uD          = 8'hFF;                                          // ../TestCases/Combinational/main.alc +68
assign uE          = 8'sh81;                                         // ../TestCases/Combinational/main.alc +69
assign uF          = 8'sh80;                                         // ../TestCases/Combinational/main.alc +70
//------------------------------------------------------------------------------

endmodule
//------------------------------------------------------------------------------

