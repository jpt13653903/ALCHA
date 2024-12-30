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
- [Statements](Statements.md#statements)
  - [Control Flow Structures](#control-flow-structures)
    - [if](#if)
    - [switch](#switch)
    - [while](#while)
    - [for](#for)
    - [loop](#loop)
    - [return](#return)
    - [break and continue](#break-and-continue)
  - [Alias](#alias)
- [Arrays](Arrays.md)
- [Functions](Functions.md)
- [Synchronous Circuits](SynchronousCircuits.md)
- [Classes](Classes.md)
- [Operator Overloading](OperatorOverloading.md)
- [Scripting Features](Scripting.md)
- [Advanced Attributes](AdvancedAttributes.md)
- [High-level Structures](HighLevelStructures.md)
- [Simulation and Verification](Simulation.md)

--------------------------------------------------------------------------------

# Statements

## Control Flow Structures

### if

If statement syntax is presented below:

```alcha
  if(A){
    // if A is non-zero, do this
  }else{
    // otherwise do this
  }
```

If the condition is purely scripting (i.e. it does not contain any
synthesisable variables), the if-statement is equivalent to conditional
compilation (the `#if` ... `#else` ... `#endif` construct of the C preprocessor).

### switch

Switch-statement syntax is presented below:

```alcha
  switch(A){
    case(B, C, D){
      // if A is equal to B, C or D, do this
    }
    case(E){
      // if A is equal to E, do this
    }
    default{
      // otherwise do this
    }
  }
```

The expressions in the `case` statements (`B`, `C`, `D` and `E` in the above
example) must evaluate to unique values.  During the scripting stage of
compilation, these can be dynamic (because the values are known during
compile-time).  During the synthesis stage, the expressions must evaluate to
constants.

ALCHA does not infer latches.  Modern FPGAs do not have latches, so it makes
little sense to implement a language structure that does.  Within
combinational circuits, ALCHA assigns "don't care" to any net not assigned a
value in all possible evaluations of `if`- or `switch`-statements.  Within
clocked structures, the net keeps it's previous value unless explicitly assigned.

Case statements can be scripted, giving the code an auto-generated feel.
The example below auto-generates the large case statements often encountered
with register decoding:

```alcha
  switch(Address){
    for(Register in 'RdRegisters){
      case(Register'Address) ReadData := Register;
    }
    for(Register in 'WrRegisters){
      case(Register'Address) ReadData := Register;
    }
    for(Register in 'LiveRegisters){
      case(Register'Read'Address) ReadData := Register'Read;
    }
  }

  if(WriteEnable){
    switch(Address){
      for(Register in 'WrRegisters){
        case(Register'Address) Register := WriteData;
      }
      for(Register in 'LiveRegisters){
        case(Register'Write'Address) Register'Write := WriteData;
      }
    }
    for(Register in 'LiveRegisters){
      Register'Strobe = ((Address == Register'Strobe'Address) & Write);
    }
  }
```

### while

The while-loop syntax is presented below.  The body of the loop is evaluated
for as long as `A` is not zero.  For combinational circuits and pure RTL, the
value of `A` must be known at compile-time.  Within a finite state machine
(FSM) structure, `A` can be a synthesisable expression.

```alcha
  while(A){
    // while A is non-zero, do this
  }
```

### for

The for-loop syntax is presented below.  `A` must evaluate to an array.  For
every element of the array, `x` becomes a reference to that element and the
loop is evaluated.

```alcha
  for(x in A){
    // for every element of array A, set x to that element and do this
  }
```

The loop is evaluated in the same order as the elements of the array, not in
parallel.  When the elements of `A` is a synthesisable type, and the loop
occurs during a combinational statement, the result could potentially be a
parallel combinational circuit.  During compilation, however, the loop is
still evaluated sequentially, following normal combinational-statement
blocking assignment rules.  This is illustrated below:

```alcha
  net(8) A, B;

  // Counts the number of bits of A that are high and assigns the answer to B
  B = 0;
  for(x in 0..7) B += A[x];
```

### loop

The loop-loop syntax is presented below:

```alcha
  loop(N){
    // Do this N times
  }
  loop{
    // Do this forever
  }
```

The evaluation of loops in FSM structures depend on the use of commas or
semicolons.  If the statement within the loop body ends in a comma, the loop
is effectively unrolled into the state in which it is declared.  If, on the
other hand, the statement within the loop body ends in a semicolon (thereby
telling the compiler to go to the next state), the loop is evaluated one
clock-cycle at a time.

A loop loop can therefore be used to implement a "wait" state, as follows:

```alcha
  net LED = 0;

  fsm(Clk, Reset){
    loop{ // Main loop (do this forever)
      LED = ~LED,   // Toggle the LED and
      loop(20_000); // Go to the next state 20 000 times
    }
  }
```

This is equivalent to the following Verilog:

```verilog
  reg       LED;
  reg [14:0]Count;

  always @(posedge Clk) begin
    if(Reset) begin
      LED   <= 0;
      Count <= 15'd_20_000;

    end else if(Count == 15'd_20_000) begin
      LED   <= ~LED;
      Count <= 15'd1;

    end else begin
      Count <= Count + 1'b1;
    end
  end
```

### return

The `return` statement returns from a function.  If the function was called
combinationally, the return statement simply ends evaluation of the rest of
the function body (thereby not synthesising the rest of the circuit).  If the
return statement occurs within a clocked structure, the clocked structure goes
into an idle state, either waiting for the next call (if it was called from a
state machine) or halting until the next system reset (if it was called
combinationally).

```alcha
  num Func(A, B){
    return A+B; // Equivalent to "Func = A+B; return;"
  }
  void Proc(A, B){
    // Do some stuff
    if(whatever) return; // return early
    // do some more stuff
  }
```

### break and continue

The `break` and `continue` keywords are used to jump out of, or within,
loops.  Both of these take an optional integer expression argument to indicate
the number of loop levels.  For instance:

```alcha
  loop{
    for(j in 1..100){
      while(Busy){
        if(Timeout) break 3; // Continues at "Error code"
        continue;            // skips the rest of this loop body
                             // and does the next iteration of this loop

        continue 2; // skips the rest of the loop body of this and the "for"
                    // loop and does the next iteration of the "for" loop

        break;      // Breaks out of the "while" loop and does the rest of
                    // the "for" loop body;
      }
    }
  }
  // Error code
```

## Alias

It is often the case where the same expression is used many times, yet cannot
be simply assigned to a variable.  In this case, it is convenient to define an
alias for the expression, as follows:

```alcha
  alias C   = Namespace.ClassInstance;
  alias M   = SomeClass.Member;
  alias Clk = SYS_GLOBAL_CLK;

  C.Member = 3;
  M = 5;
  fsm(Clk){
    // Some statements...
  }
```

It is important to note, however, that the alias is evaluated first, before
used in the target expression.  The following two lines are equivalent:

```alcha
  alias sum = A + B; X = Y * sum;
  X = Y * (A + B);
```

--------------------------------------------------------------------------------

