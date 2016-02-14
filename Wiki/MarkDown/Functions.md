[[include repo=doccode path=MarkDown/Header.md]]

[TOC]

# Functions
ALCHA provides a procedural programming model for combinational circuits, as well as state machines.  Any function that does not contain an embedded state machine is considered combinational and can be called from anywhere.  A function that contains an embedded state machine can only be called from within another state machine.

## Combinational Functions
Functions in ALCHA are defined in similar fashion as C, as illustrated in the example below.  The function name is used as a temporary signal (or variable) for the return value.  The parameter types do not need to be defined in the function, but can be inferred from the input parameters when the function is called.  When the types are specified, the parameter types form part of the function name, so that many functions of the same name, but different parameter types, can be defined.

    :::C++
    sig'8 Add(sig A, sig B){
     return A + B; // This is short-hand for "Add = A + B; return;"
    }
    sig'8 x, y, z;
    z = Add(x, y);

Combinational functions can be called from anywhere, including the bodies of clocked structures.  Functions can also be called in vectorised form, which is especially useful when combined with array slicing.  The adder above can be called as follows:

    :::C++
    sig'8 x[4], y[4], z[4];
    z = Add(x, y);

    // This is equavalent to:
    z[0] = Add(x[0], y[0]);
    z[1] = Add(x[1], y[1]);
    z[2] = Add(x[2], y[2]);
    z[3] = Add(x[3], y[3]);

Arrays and scalars can be mixed in the call, as follows:

    :::C++
    sig'8 x, y[4], z[4];
    z = Add(x, y);

    // This is equavalent to:
    z[0] = Add(x, y[0]);
    z[1] = Add(x, y[1]);
    z[2] = Add(x, y[2]);
    z[3] = Add(x, y[3]);

If the function takes an array as a parameter, the same rules apply.  The function can be called with a higher-dimension array, as follows:

    :::C++
    sig'18 Dot(A[], B[]){
     Dot  = A[1] * B[1];
     Dot += A[2] * B[2];
     Dot += A[3] * B[3];
     Dot += A[4] * B[4];
     return;
    }
    sig'18 Y[16];
    sig'8  A[4][16], B[4][16];
    Y = Dot(A, B);

To return an array, the function name should be defined as an array, as follows:

    :::C++
    signed sig'(16,8) Mult[4][4](A[][], B[][]){
     int i, j, k;
     for(i in 0->3){
      for(j in 0->3){
       Mult[i][j] = 0;
       for(k in 0->3){
        Mult[i][j] += A[i][k] * B[k][j];
       }
      }
     }
    }

The loops above are discussed in greater detail in the Scripting section.  Such combinational loops should be used with care, as very little code can result in very large circuits.

## Clocked Functions
Clocked functions contain optional combinational code, as well as an embedded state machine.  The embedded state machine does not take parameters, as the clock is inherited from the calling state machine.  The calling state also implements the reset.

There has to be a cycle-boundary after a clocked function call (i.e. the call must end with a semicolon, not a comma).

The body of the embedded state machine is only implemented once, so that many states in the calling state machine can make use of the function without further resource usage.

The combinational code is evaluated during the calling state, after which the function parameters, as well as the return state, are saved to temporary registers.  The next cycle executes the first state of the embedded state machine.

A return statement in the embedded state machine will cause the target signal to be assigned, and the next state to be loaded from the temporarily-stored state register.  To illustrate this process, consider the following ALCHA (and its corresponding Verilog) code:

    :::C++
    sig'4 BitCount(A){
     sig'3 x;
     BitCount = 0;

     fsm{
      for(x in 0->7){
       BitCount += A[x]; // The ";" forces every addition into a new cycle
      }
      return;
     }
    }

    sig'8 A, B;

    fsm(Clk, Reset){
     A = 123;
     B = 93;
     A = BitCount(A);
     B = BitCount(B);
    }

Which translates to:

    :::Verilog
    reg [2:0]BitCount_RetState;
    reg [3:0]BitCount;
    reg [7:0]BitCount_A;
    reg [2:0]BitCount_x;

    reg [7:0]A;
    reg [7:0]B;

    reg [2:0]State;
    reg      tReset;

    always @(posedge Clk) begin
     tReset <= Reset;

     if(tReset) begin
      State <= 0;

     end else begin
      case(State)
       3'd0: begin
        A     <= 8'd123;
        State <= 3'd1;
       end
       
       3'd1: begin
        B     <= 8'd93;
        State <= 3'd2;
       end
       
       3'd2: begin
        BitCount_RetState <= 3'd3;
        BitCount_A        <= A;
        BitCount_x        <= 0;
        BitCount          <= 0;
        State             <= 3'd7;
       end
       
       3'd3: begin
        A                 <= BitCount;
        BitCount_RetState <= 3'd4;
        BitCount_A        <= B;
        BitCount_x        <= 0;
        BitCount          <= 0;
        State             <= 3'd7;
       end

       3'd4: begin
        A     <= BitCount;
        State <= 3'd5;
       end
       
       3'd7: begin // The BitCount function
        BitCount   <= BitCount + BitCount_A[BitCount_x];
        BitCount_x <= BitCount_x + 1'b1;
        if(BitCount_x == 3'd7) State <= BitCount_RetState;
       end

       default:;
      endcase
     end
    end

[[include repo=doccode path=MarkDown/Footer.md]]

