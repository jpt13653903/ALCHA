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
- [Functions](Functions.md)
- [Synchronous Circuits](#synchronous-circuits)
  - [Clock Networks](#clock-networks)
    - [Definition](#definition)
    - [Global Clock Networks](#global-clock-networks)
    - [Built-in Properties](#built-in-properties)
  - [Register Transfer Level](#register-transfer-level)
  - [Finite State Machines](#finite-state-machines)
- [Classes](Classes.md)
- [Scripting Features](Scripting.md)
- [Advanced Attributes](AdvancedAttributes.md)
- [High-level Structures](HighLevelStructures.md)
- [Simulation and Verification](Simulation.md)

--------------------------------------------------------------------------------

# Synchronous Circuits

## Clock Networks

### Definition

A clock in ALCHA is any synthesisable node with a `frequency` attribute.  In
order to generate a clock by means of a PLL, use the `hdl` construct to
instantiate the appropriate mega-function.  Some examples of clock definitions
are presented below:

```alcha
    // An input pin
    pin<frequency = 100e6> ipClkPin;

    // An RTL circuit
    net<frequency = 50e6> clk_50;
    rtl(ipClkPin) clk_50 = ~clk_50;

    // A combinational circuit
    net<frequency = 100e6> clkLowPower = ipClkPin & (~sleep);

    // A phase-locked loop megafunction
    hdl() altera_pll(
        reference_clock_frequency = "100 MHz";

        number_of_clocks          = 1;
        operation_mode            = "direct";

        output_clock_frequency0   = "250 MHz";
        duty_cycle0               = 50;
        phase_shift0              = "0 ps";

        pll_type                  = "General";
        pll_subtype               = "General";
        fractional_vco_multiplier = "false";
    ){
        net refclk = ipClkPin;
        net rst    = ipGlobalReset;
        net locked;
        net fbclk  = 0;
        net fboutclk;

        net<frequency = 250e6> outclk;
    }
    altera_pll() pll_250;
    alias clk_250 = pll_250.outclk;
```

ALCHA further supports clock-enable type clocks, which are generally used for
lower frequency circuits that must be synchronised to a higher frequency clock
domain.  The clock enable net must also carry the `frequency` attribute, so
that the compiler knows how to set up the associated multi-cycle timing
requirements.

```alcha
    net(14) clkCounter;

    net<frequency = 100e6> clk;
    net<frequency = 9600 > clkEnable = ~|clkCounter;

    rtl(clk){
        if(clkCounter) clkCounter--;
        else           clkCounter = (clk'frequency / clkEnable'frequency) - 1;
    }
    rtl(clk, reset, clkEnable){
        // The RS-232 controller code goes here.
    }
```

### Global Clock Networks

Most modern FPGAs contain dedicated high-fanout, low-skew clock tree
networks.  ALCHA automatically assigns clocks to the available clock
networks.  If there are more clocks defined than clock networks available, the
higher-frequency clocks take priority.  This automated global clock network
assignment can be bypassed by explicitly specifying the `global = true` attribute.

### Built-in Properties

All clocks (including clock pins) have a `frequency` member (of type `num`)
that can be used by scripts to calculate clock-frequency dependent parameters.
A button denouncer might, for instance, be defined as follows:

```alcha
    net debounce(net clk, net button){
        num timeout = clk'frequency * 20e-3 - 1; // 20 ms dead-time
        num N       = ceil(log2(count));

        net(N) counter;

        rtl(clk){
            if(counter == timeout){
                if(result != button){
                    result  = button;
                    counter = 0;
                }
            }else{
                counter++;
            }
        }
    }
```

## Register Transfer Level

Most HDL designers are familiar with RTL design.  In ALCHA, the `rtl`
construct can be used to describe RTL logic.  It takes the clock signal as
the first parameter, which must be a synthesisable type with a frequency
attribute.   It may also take two more optional parameters: the reset and
clock-enable signals. All statements within an `rtl` construct are
non-blocking. Below is an example:

```alcha
    pin<frequency = 50e6> ipClk;
    pin                   ipReset;

    net A, B;

    net(27) count = 0; // Initialiser => reset value

    rtl(ipClk, ipReset){
        count++;

        if(&count){
            A = B;
            B = A;
        }
    }
```

The equivalent Verlog of the above ALCHA is:

```verilog
    reg       reset;
    reg       A, B;
    reg [26:0]count;

    always @(posedge ipClk) begin
        // Resets are automatically localised
        reset <= ipReset;

        if(reset) count <= 0;
        else      count <= count + 1'b1;

        // No initialiser on A or B, so not part of the reset circuit
        if(&count) begin
            A <= B;
            B <= A;
        end
    end
```

All reset signals are localised to the RTL block.  All signals that enter
the `rtl` construct with a value (that must evaluate to a run-time constant)
are reset to that value.

## Finite State Machines

ALCHA provides a concise FSM description syntax by means of the `fsm`
construct.  It takes the clock signal as the first parameter, which must be a synthesisable type with a frequency attribute.  It may also take two more optional parameters: the reset and clock-enable signals.

If a reset signal is not specified, an auto-generated power-on reset signal
is used.

Each statement within an `fsm` construct that ends in a comma (`,`), is
considered to be in the same state, or cycle, as the next statement.  A
semicolon (`;`) ends a cycle (or state).  All statements are considered
non-blocking.  A simple example is provided below:

```alcha
    pin<frequency = 50e6> ipClk;
    pin                   ipReset;

    net(8) A, B, C;

    B = 123;

    fsm(ipClk, ipReset){
        loop{
            A = B + C,
            B = C - A;

            if(A < B) C++,
            else      C--,
            ;
        }
    }
```

This state machine has two states and is equivalent to the following Verilog:

```verilog
    reg[7:0] A, B, C;

    reg reset;
    reg state;

    always @(posedge Clk) begin
        reset <= ipReset;

        if(reset) begin
            B     <= 8'd123;
            state <= 0;

        end else begin
            case(state)
                1'b0: begin
                    A     <= B + C;
                    B     <= C - A;
                    state <= 1'b1;
                end

                1'b1: begin
                    if(A < B) C <= C + 1'b1;
                    else      C <= C - 1'b1;
                    state <= 1'b0;
                end

                default:;
            endcase
        end
    end
```

All reset signals are localised to the state machine.  All signals that enter
the `fsm` construct with a value (that must evaluate to a run-time constant)
are reset to that value.

The ALCHA `if` and `while` statements follow the same syntax as in C.  The
state (or cycle) boundaries are controlled by means of semicolons (`;`).

A `for` loop with a semicolon is unrolled, for example:

```alcha
    net( 8) x;
    net(10) A;

    fsm(ipClk, ipReset){
        for(x in 0..200){
            A = 3 * x + 7;
        }
    }
```

This is equivalent to the following Verilog:

```verilog
    reg [7:0]x;
    reg [9:0]A;
    reg      state;

    always @(posedge Clk) begin
        reset <= ipReset;

        if(reset) begin
            x     <= 0;
            state <= 0;

        end else begin
            case(state)
                1'b0: begin
                    A <= 2'd3 * x + 3'd7;
                    x <= x + 1'b1;
                    if(x == 8'd200) state <= 1'b1;
                end

                default:;
            endcase
        end
    end
```

--------------------------------------------------------------------------------

