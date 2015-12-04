module main(
 output [8:1]USER_LED,
 input  [4:1]SW
);

assign USER_LED = {SW, SW};

endmodule

