# ALCHA

<img src="https://openclipart.org/download/3850/dchandlr-dchandlr-work.svg" height="70" alt="Work in Progress"/>
The ALCHA project, including the language grammar and, by extension, this
wiki, is under development.  This wiki serves as a documentation of the
project goals and aspirations, which are inherently unstable and subject to
change without notice.

--------------------------------------------------------------------------------

## Table of Contents

- [Introduction](Introduction.md)
- [Lexical](Lexical.md)
- [Grammar](Grammar.md)
- [Modules](Modules.md)
- [Declarations](Declarations.md)
- [Expressions](Expressions.md)
- [Statements](Statements.md)
- [Arrays](Arrays.md)
- [Functions](#functions)
  - [Combinational Functions](#combinational-functions)
  - [Clocked Functions](#clocked-functions)
  - [RTL Functions](#rtl-functions)
  - [Example](#example)
  - [Calling Clocked Functions from Combinational Circuits](#calling-clocked-functions-from-combinational-circuits)
  - [Operator Overloading](#operator-overloading)
- [Synchronous Circuits](SynchronousCircuits.md)
- [Classes](Classes.md)
- [Scripting Features](Scripting.md)
- [Advanced Attributes](AdvancedAttributes.md)
- [High-level Structures](HighLevelStructures.md)
- [Simulation and Verification](Simulation.md)

--------------------------------------------------------------------------------

# Functions

ALCHA provides a procedural programming model for combinational circuits,
state machines and scripting.  There are various categories of functions,
resulting in different calling conventions.

The `result` built-in variable is used as a reference to the target net (or
object) for the return value.  This has implications for functions that
contain clocked structures, as the state machine (or RTL) has control over the
value of the target net, even though the function has not returned yet.

The parameter types do not need to be defined in the function, but can be
inferred from the input parameters when the function is called (this is also
known as duck-typing).  When the types are specified, the parameter types form
part of the function name, so that many functions of the same name, but
different parameter types, can be defined.

When the function returns `auto`, the `result` object is also duck-typed.

## Combinational Functions

Functions in ALCHA are defined in similar fashion as C, as illustrated in the
example below.

```alcha
    net(8) add(net(8) A, net(8) B){
        result = A + B;
    }
    net(8) x, y, z;
    z = add(x, y);
```

Functions also support default parameters and named parameter assignment, as
illustrated below:

```alcha
    auto myFunction(A, B, C = 7, D = 2){
        // Function body
    }

    net(8) X = myFunction(B = 2, A = 8, D = 4); // C defaults to 7
```

Combinational functions can be called from anywhere, including the bodies of
clocked structures.  Functions can also be called in vectorised form, which is
especially useful when combined with array slicing.  The adder above can be
called as follows:

```alcha
    net(8) x[4], y[4], z[4];
    z = add(x, y);

    // This is equavalent to:
    z[0] = add(x[0], y[0]);
    z[1] = add(x[1], y[1]);
    z[2] = add(x[2], y[2]);
    z[3] = add(x[3], y[3]);
```

Arrays and scalars can be mixed in the call, as follows:

```alcha
    net(8) x, y[4], z[4];
    z = add(x, y);

    // This is equavalent to:
    z[0] = add(x, y[0]);
    z[1] = add(x, y[1]);
    z[2] = add(x, y[2]);
    z[3] = add(x, y[3]);
```

If the function takes an array as a parameter, the same rules apply.
The function can be called with a higher-dimension array, as follows:

```alcha
    net(18) dot(A[], B[]){
        result = 0;
        for(n in 0..(A'length-1)) result += A[n] * B[n];
    }
    net(18) Y[16];
    net( 8) A[4][16], B[4][16];
    Y = dot(A, B);
```

To return an array, the function name should be defined as an array, as follows:

```alcha
    net(16, -8) Mult[4][4] (A[][], B[][]){
        int i, j, k;
        for(i in 0..3){
            for(j in 0..3){
                result[i][j] = 0;
                for(k in 0..3){
                    result[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }
```

The loops above are discussed in greater detail in the [Scripting](Scripting.md)
section.  Such combinational loops should be used with care, as very little
code can result in very large circuits.

## Clocked Functions

Clocked functions contain optional combinational code, as well as an embedded
state machine or RTL.  The embedded clocked structure's clock and reset can,
potentially, be different to that of the calling clocked structure.  These
could either be global signals, or passed through a function parameter.

The function body is implemented as few times as possible, typically in a
sub-module.  If many different states call the same function, the function
body is implemented only once.  It is possible to call the same function many
times from the same state, in which case the function is implemented multiple
times.

The call is controlled through hand-shaking signals.  The resulting function
module has implicit `go` (input) and `busy` (output) signals.  These are
handled by the compiler and hidden from the developer.  When multiple
different functions are called from the same state, the state machine must
wait for all the functions to finish (i.e. make their `busy` lines low) before
continuing to the next state.

The combinational code of the function body, as well as the function reset
code, is evaluated during the calling state.  The `go` line is also pulled
high during that state.

The handshaking signals are handled differently when the function and calling
structure reside within the same clock-domain, or when there is a clock-domain
crossing required.  When there is a clock-domain crossing, the calling state
machine waits for the `busy` to go high before releasing the `go` signal.
The function waits for the `go` to go low before releasing the `busy` signal.
These hand-shaking signals are properly clock-domain crossed when required.

When there are no clock-domain crossing required, the calling state machine
pulses the `go` signal for a single cycle instead of following the full
hand-shaking algorithm described above.  This is a timing optimisation.

The `busy` signal further acts as a clock-enable signal of the function body.
The output signals of the function keep their state until the next call.

## RTL Functions

RTL structures do not have the concept of an instruction sequence.  In order
to return from a function that contains an RTL structure, a `return` statement
must exist somewhere in the RTL body, which releases the `busy` signal and
halts execution of the RTL structure.

Furthermore, RTL structures do not support the handshaking algorithm described
above.  The developer must implement them manually.

When calling a function from an RTL structure, the same rules apply as for
calling them from combinational logic.

## Example

In order to illustrate the use of functions within state machines, here
follows an example of a simple SPI interface in ALCHA, as well as its
equivalent Verilog:

```alcha
    pin<frequency = 50e6> clk;

    pin reset;

    group SPI{
        pin clk, data, latch;
    }
    //--------------------------------------------------------------------------

    void spiSend(net(32) data){
        net( 5) bit;
        net(32) shift;

        SPI.clk   = 0;
        SPI.latch = 0;
        SPI.data  = shift[31];

        fsm(clk, reset){
            SPI.latch = 0,
            shift     = data;

            for(bit in 31..0){
                SPI.clk = 1;

                SPI.clk = 0,
                shift <<= 1,
                if(!bit) SPI.latch = 1,
                ;
            }
            SPI.latch = 0;
        }
    }
    //--------------------------------------------------------------------------

    // Initialise some SPI-based peripheral
    fsm(clk, reset){
        spiSend(0x12345678);
        spiSend(0xFEDCBA98);
        spiSend(18271662);
        spiSend(0b11010011:0x123456);
    }
    //--------------------------------------------------------------------------
```

Which is equivalent to:

```verilog
    module spiSend(
        input       ipClk,
        input       ipReset,

        input       ipGo,
        output reg  opBusy,

        input [31:0]ipData,

        output reg  opSPI_Clk,
        output      opSPI_Data,
        output reg  opSPI_Latch
    );
    //--------------------------------------------------------------------------

    reg [ 4:0]bits;
    reg [31:0]shift;
    reg       reset;

    enum { GetData, Sending, Done } state;
    //--------------------------------------------------------------------------

    assign opSPI_Data = shift[31];
    //--------------------------------------------------------------------------

    always @(posedge clk) begin
        reset <= ipReset;

        if(reset) begin
            opSPI_Clk   <= 0;
            opSPI_Latch <= 0;
            busy        <= 0;
            state       <= GetData;
        //----------------------------------------------------------------------

        end else if(busy) begin
            case(state)
                GetData: begin
                    opSPI_Latch <= 0;
                    shift       <= data;
                    bits        <= 5'd31;
                    state       <= Sending;
                end
                //--------------------------------------------------------------

                Sending: begin
                    if(~opSPI_Clk) begin
                        opSPI_Clk <= 1'b1;

                    end else begin // opSPI_Clk is high
                        opSPI_Clk <= 0;
                        shift     <= {shift[30:0], 1'b0};
                        if(~|bits) begin
                            opSPI_Latch <= 1'b1;
                            state       <= Done;
                        end
                        bits <= bits - 1'b1;
                    end
                end
                //--------------------------------------------------------------

                Done: begin
                    opSPI_Latch <= 0;
                    busy        <= 0;
                end
                //--------------------------------------------------------------

                default:;
            endcase
        //----------------------------------------------------------------------

        end else if(go) begin
            opSPI_Clk   <= 0;
            opSPI_Latch <= 0;
            busy      <= 1'b1;
            state     <= GetData;
        end
    end

    endmodule
    //##########################################################################

    module TopLevel(
        input  ipClk,
        input  ipReset,

        output opSPI_Clk,
        output opSPI_Data,
        output opSPI_Latch
    );
    //--------------------------------------------------------------------------

    reg       spiSendGo;
    wire      spiSendBusy;
    reg [31:0]data;

    SPI_Send spiSend(
        .ipClk      (ipClk),
        .ipReset    (ipReset),

        .ipGo       (spiSendGo),
        .opBusy     (spiSendBusy),

        .ipData     (data),

        .opSPI_Clk  (opSPI_Clk),
        .opSPI_Data (opSPI_Data),
        .opSPI_Latch(opSPI_Latch)
    );
    //--------------------------------------------------------------------------

    reg reset;
    enum { Send1, Send2, Send3, Send4, Done } state;
    //--------------------------------------------------------------------------

    always @(posedge ipClk) begin
        reset <= ipReset;

        if(reset) begin
            spiSendGo <= 0;
            state     <= Send1;
        //----------------------------------------------------------------------

        end else begin
            case(state)
                Send1: begin
                    data      <= 32'h12345678;
                    spiSendGo <= 1'b1;
                    state     <= Send2;
                end
                //--------------------------------------------------------------

                Send2: begin
                         if( spiSendGo  ) spiSendGo <= 0;
                    else if(!spiSendBusy) begin
                        data      <= 32'hFEDCBA98;
                        spiSendGo <= 1'b1;
                        state     <= Send3;
                    end
                end
                //--------------------------------------------------------------

                Send3: begin
                         if( spiSendGo  ) spiSendGo <= 0;
                    else if(!spiSendBusy) begin
                        data      <= 32'd18271662;
                        spiSendGo <= 1'b1;
                        state     <= Send4;
                    end
                end
                //--------------------------------------------------------------

                Send4: begin
                         if( spiSendGo  ) spiSendGo <= 0;
                    else if(!spiSendBusy) begin
                        data      <= {8'b11010011, 24'h123456};
                        spiSendGo <= 1'b1;
                        state     <= Done;
                    end
                end
                //--------------------------------------------------------------

                default:
                    spiSendGo <= 0;
            endcase
        end
    end

    endmodule
    //--------------------------------------------------------------------------
```

The hand-shaking has been simplified because the two clocked structures are in
the same clock domain.  This Verilog code has been hand-written in order to
show the details of the ALCHA calling convention.  Compiler-generated code
might look very different, but is equivalent in functionality.

## Calling Clocked Functions from Combinational Circuits

It is possible to call a clocked function from a combinational circuit.  In
that case, the function body is evaluated as if it occurred in place of the
function call.

## Operator Overloading

Operator overloading is done similar to C++, i.e. the `operator` keyword.
Some examples include:

```alcha
    // binary operator
    MyClass operator+ (MyClass a, num b){
        !! The addition algorithm
    }
    // prefix unary operator
    num operator& (MyClass a){
        !! The and-reduce algorithm
    }
    // postfix unary operator
    num operator! (MyClass a, _){
        !! The factorial algorithm
    }
```

The `_` parameter is a special built-in marker used for this purpose,
but it can also be used as the "discard variable" (similar to C#).

--------------------------------------------------------------------------------

