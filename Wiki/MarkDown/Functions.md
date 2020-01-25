[[include repo=code path=Wiki/MarkDown/Header.md]]

[TOC]

# Functions

ALCHA provides a procedural programming model for combinational circuits, state machines and scripting.  There are various categories of functions, resulting in different calling conventions.

The function name is used as a reference to the target net (or variable) for the return value.  This has implications for functions that contain clocked structures, as the state machine (or RTL) has control over the value of the target net, even though the function has not returned yet.

The parameter types do not need to be defined in the function, but can be inferred from the input parameters when the function is called (this is also known as duck-typing).  When the types are specified, the parameter types form part of the function name, so that many functions of the same name, but different parameter types, can be defined.

## Combinational Functions

Functions in ALCHA are defined in similar fashion as C, as illustrated in the example below.

```C++
  net(8) Add(net(8) A, net(8) B){
    Add = A + B;
  }
  net(8) x, y, z;
  z = Add(x, y);
```

Functions also support default parameters and named parameter assignment, as illustrated below:

```C++
  auto MyFunction(A, B, C = 7, D = 2){
    // Function body
  }

  net(8) X = MyFunction(B = 2, A = 8, D = 4); // C defaults to 7
```

Combinational functions can be called from anywhere, including the bodies of clocked structures.  Functions can also be called in vectorised form, which is especially useful when combined with array slicing.  The adder above can be called as follows:

```C++
  net(8) x[4], y[4], z[4];
  z = Add(x, y);

  // This is equavalent to:
  z[0] = Add(x[0], y[0]);
  z[1] = Add(x[1], y[1]);
  z[2] = Add(x[2], y[2]);
  z[3] = Add(x[3], y[3]);
```

Arrays and scalars can be mixed in the call, as follows:

```C++
  net(8) x, y[4], z[4];
  z = Add(x, y);

  // This is equavalent to:
  z[0] = Add(x, y[0]);
  z[1] = Add(x, y[1]);
  z[2] = Add(x, y[2]);
  z[3] = Add(x, y[3]);
```

If the function takes an array as a parameter, the same rules apply.  The function can be called with a higher-dimension array, as follows:

```C++
  net(18) Dot(A[], B[]){
    Dot  = A[1] * B[1];
    Dot += A[2] * B[2];
    Dot += A[3] * B[3];
    Dot += A[4] * B[4];
  }
  net(18) Y[16];
  net( 8) A[4][16], B[4][16];
  Y = Dot(A, B);
```

To return an array, the function name should be defined as an array, as follows:

```C++
  net(16, -8) Mult[4][4](A[][], B[][]){
    int i, j, k;
    for(i in 0..3){
      for(j in 0..3){
        Mult[i][j] = 0;
        for(k in 0..3){
          Mult[i][j] += A[i][k] * B[k][j];
        }
      }
    }
  }
```

The loops above are discussed in greater detail in the Scripting section.  Such combinational loops should be used with care, as very little code can result in very large circuits.

## Clocked Functions

Clocked functions contain optional combinational code, as well as an embedded state machine or RTL.  The embedded clocked structure's clock and reset can, potentially, be different to that of the calling clocked structure.  These could either be global signals, or passed through a function parameter.

The function body is implemented as few times as possible, typically in a sub-module.  If many different states call the same function, the function body is implemented only once.  It is possible to call the same function many times from the same state, in which case the function is implemented multiple times.

The call is controlled through hand-shaking signals.  The resulting function module has implicit `Go` (input) and `Busy` (output) signals.  These are handled by the compiler and hidden from the developer.  When multiple different functions are called from the same state, the state machine must wait for all the functions to finish (i.e. make their `Busy` lines low) before continuing to the next state.

The combinational code of the function body, as well as the function reset code, is evaluated during the calling state.  The `Go` line is also pulled high during that state.

The handshaking signals are handled differently when the function and calling structure reside within the same clock-domain, or when there is a clock-domain crossing required.  When there is a clock-domain crossing, the calling state machine waits for the `Busy` to go high before releasing the `Go` signal.  The function waits for the `Go` to go low before releasing the `Busy` signal.  These hand-shaking signals are properly clock-domain crossed when required.

When there are no clock-domain crossing required, the calling state machine pulses the `Go` signal for a single cycle instead of following the full hand-shaking algorithm described above.  This is a timing optimisation.

The `Busy` signal further acts as a clock-enable signal of the function body.  The output signals of the function keep their state until the next call.

## RTL Functions

RTL structures do not have the concept of an instruction sequence.  In order to return from a function that contains an RTL structure, a `return` statement must exist somewhere in the RTL body, which releases the `Busy` signal and halts execution of the RTL structure.

Furthermore, RTL structures do not support the handshaking algorithm described above.  The developer must implement them manually.

When calling a function from an RTL structure, the same rules apply as for calling them from combinational logic.

## Example

In order to illustrate the use of functions within state machines, here follows an example of a simple SPI interface in ALCHA, as well as its equivalent Verilog:

```C++
  pin<frequency = 50e6> Clk;

  pin Reset;

  group SPI{
    pin Clk, Data, Latch;
  }
  //------------------------------------------------------------------------------

  void SPI_Send(net(32) Data){
    net( 5)  bit;
    net(32) Shift;

    SPI.Clk   = 0;
    SPI.Latch = 0;
    SPI.Data  = Shift[31];

    fsm(Clk, Reset){
      SPI.Latch = 0,
      Shift     = Data;

      for(bit in 31..0){
        SPI.Clk = 1;

        SPI.Clk = 0,
        Shift <<= 1,
        if(!bit) SPI.Latch = 1,
        ;
      }
      SPI.Latch = 0;
    }
  }
  //------------------------------------------------------------------------------

  // Initialise some SPI-based peripheral
  fsm(Clk, Reset){
    SPI_Send(0x12345678);
    SPI_Send(0xFEDCBA98);
    SPI_Send(18271662);
    SPI_Send(0b11010011:0x123456);
  }
  //------------------------------------------------------------------------------
```

Which is equivalent to:

```Verilog
  module SPI_Send(
    input       Clk,
    input       Reset,

    input       Go,
    output reg  Busy,

    input [31:0]Data,

    output reg  SPI_Clk,
    output      SPI_Data,
    output reg  SPI_Latch
  );
  //----------------------------------------------------------------------------

  reg [ 4:0]bit;
  reg [31:0]Shift;

  reg       tReset;
  reg   [1:0]State;
  localparam GetData = 2'd0;
  localparam Sending = 2'd1;
  localparam Done    = 2'd2;
  //----------------------------------------------------------------------------

  assign SPI_Data = Shift[31];
  //----------------------------------------------------------------------------

  always @(posedge Clk) begin
    tReset <= Reset;

    if(tReset) begin
      SPI_Clk   <= 0;
      SPI_Latch <= 0;
      Busy      <= 0;
      State     <= GetData;
    //--------------------------------------------------------------------------

    end else if(Busy) begin
      case(State)
        GetData: begin
          SPI_Latch <= 0;
          Shift     <= Data;
          bit       <= 5'd31;
          State     <= Sending
        end
        //----------------------------------------------------------------------

        Sending: begin
          if(~SPI_Clk) begin
            SPI_Clk <= 1'b1;

          end else begin // SPI_Clk is high
            SPI_Clk <= 0;
            Shift   <= {Shift[30:0], 1'b0};
            if(~|bit) begin
              SPI_Latch <= 1'b1;
              State     <= Done;
            end
            bit <= bit - 1'b1;
          end
        end
        //----------------------------------------------------------------------

        Done: begin
          SPI_Latch <= 0;
          Busy      <= 0;
        end
        //----------------------------------------------------------------------

        default:;
      endcase
    State <= State + 1'b1;
    //--------------------------------------------------------------------------

    end else if(Go) begin
      SPI_Clk   <= 0;
      SPI_Latch <= 0;
      Busy      <= 1'b1;
      State     <= GetData;
    end
  end

  endmodule
  //############################################################################

  module TopLevel(
    input Clk,
    input Reset,

    output SPI_Clk,
    output SPI_Data,
    output SPI_Latch
  );
  //----------------------------------------------------------------------------

  reg       SPI_Send_Go;
  wire      SPI_Send_Busy;
  reg [31:0]Data;

  SPI_Send SPI_Send_Intance(
    .Clk      (Clk),
    .Reset    (Reset),

    .Go       (SPI_Send_Go),
    .Busy     (SPI_Send_Busy),

    .Data     (Data),

    .SPI_Clk  (SPI_Clk),
    .SPI_Data (SPI_Data),
    .SPI_Latch(SPI_Latch)
  );
  //----------------------------------------------------------------------------

  reg       tReset;
  reg   [2:0]State;
  localparam Send1 = 3'd0;
  localparam Send2 = 3'd1;
  localparam Send3 = 3'd2;
  localparam Send4 = 3'd3;
  localparam Done  = 3'd4;
  //----------------------------------------------------------------------------

  always @(posedge Clk) begin
    tReset <= Reset;

    if(tReset) begin
      SPI_Send_Go <= 0;
      State       <= Send1;
    //--------------------------------------------------------------------------

    end else begin
      case(State)
        Send1: begin
          Data        <= 32'h12345678;
          SPI_Send_Go <= 1'b1;
          State       <= Send2;
        end
        //----------------------------------------------------------------------

        Send2: begin
               if( SPI_Send_Go  ) SPI_Send_Go <= 0;
          else if(!SPI_Send_Busy) begin
            Data        <= 32'hFEDCBA98;
            SPI_Send_Go <= 1'b1;
            State       <= Send3;
          end
        end
        //----------------------------------------------------------------------

        Send3: begin
               if( SPI_Send_Go  ) SPI_Send_Go <= 0;
          else if(!SPI_Send_Busy) begin
            Data        <= 32'd18271662;
            SPI_Send_Go <= 1'b1;
            State       <= Send4;
          end
        end
        //----------------------------------------------------------------------

        Send4: begin
               if( SPI_Send_Go  ) SPI_Send_Go <= 0;
          else if(!SPI_Send_Busy) begin
            Data        <= {8'b11010011, 24'h123456};
            SPI_Send_Go <= 1'b1;
            State       <= Done;
          end
        end
        //----------------------------------------------------------------------

        default: SPI_Send_Go <= 0;
      endcase
    end
  end

  endmodule
  //----------------------------------------------------------------------------
```

The hand-shaking has been simplified because the two clocked structures are in the same clock domain.  This Verilog code has been hand-written in order to show the details of the ALCHA calling convention.  Compiler-generated code might look very different, but is equivalent in functionality.

## Calling Clocked Functions from Combinational Circuits

It is possible to call a clocked function from a combinational circuit.  It that case, the function body is evaluated as if it occurred in place of the function call, with the signal being assigned to appearing in place of the function name.

[[include repo=code path=Wiki/MarkDown/Footer.md]]

