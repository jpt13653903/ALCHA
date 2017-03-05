[[include repo=code path=Wiki/MarkDown/Header.md]]

[TOC]

# Statements
## Control Flow Structures
### if

If statement syntax is presented below:

    :::C++
    if(A){
     // if A is non-zero, do this
    }else{
     // otherwise do this
    }

If the condition is purely scripting (i.e. it does not contain any synthesisable variables), the if-statement is equivalent to conditional compilation (the `#if` ... `#else` ... `#endif` construct of the C preprocessor).

### switch

Switch-statement syntax is presented below:

    :::C++
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

The expressions in the `case` statements (`B`, `C`, `D` and `E` in the above example) must evaluate to unique values.  During the scripting stage of compilation, these can be dynamic (because the values are known during compile-time).  During the synthesis stage, the expressions must evaluate to constants.

ALCHA does not infer latches.  Modern FPGAs do not have latches, so it makes little sense to implement a language structure does.  Within combinational circuits, ALCHA assigns "don't care" to any net not assigned a value in all possible evaluations of `if`- or `switch`-statements.  Within clocked structures, the net keeps it's previous value unless explicitly assigned.

### while

The while-loop syntax is presented below.  The body of the loop is evaluated for as long as `A` is not zero.  For combinational circuits and pure RTL, the value of `A` must be known at compile-time.  Within a finite state machine (FSM) structure, `A` can be a synthesisable expression.

    :::C++
    while(A){
     // while A is non-zero, do this
    }

### for

The for-loop syntax is presented below.  `A` must evaluate to an array.  For every element of the array, `x` becomes a reference to that element and the loop is evaluated.  

    :::C++
    for(x in A){
     // for every element of array A, set x to that element and do this
    }

The loop is evaluated in the same order as the elements of the array, not in parallel.  When the elements of `A` is a synthesisable type, and the loop occurs during a combinational statement, the result could potentially be a parallel combinational circuit.  During compilation, however, the loop is still evaluated sequentially, following normal combinational-statement blocking assignment rules.

    :::C++
    net'8 A, B;

    // Counts the number of bits of A that are high and assigns the answer to B
    B = 0;
    for(x in 0->7) B += A[x];

### loop

The loop-loop syntax is presented below:

    :::C++
    loop(N){
     // Do this N times
    }
    loop{
     // Do this forever
    }

The evaluation of loops in FSM structures depend on the use of commas or semicolons.  If the statement within the loop body ends in a comma, the loop is effectively unrolled into the state in which it is declared.  If, on the other hand, the statement within the loop body ends in a semicolon (thereby telling the compiler to go to the next state), the loop is evaluated one clock-cycle at a time.

A loop loop can therefore be used to implement a "wait" state, as follows:

    :::C++
    net LED = 0;

    fsm(Clk, Reset){
     loop{ // Main loop (do this forever)
      LED = ~LED,   // Toggle the LED and
      loop(20_000); // Go to the next state 20 000 times
     }
    }

This is equivalent to the following Verilog:

    :::Verilog
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

### return

The `return` statement returns from a function, optionally with a value:

    :::C++
    num Func(A, B){
     return A+B;
    }
    void Proc(A, B){
     // Do some stuff
     if(whatever) return; // return early
     // do some more stuff
    }

### break and continue

The `break` and `continue` keywords are used to jump out of, or within, loops.  Both of these take an optional integer expression argument to indicate the number of loop levels.  For instance:

    :::C++
    loop{
     for(j in 1->100){
      while(Busy){
       if(Timeout) break 3; // Continues at "Error code"
       continue;   // skips the rest of this loop body
                   // and does the next iteration of this loop

       continue 2; // skips the rest of the loop body of this and the "for"
                   // loop and does the next iteration of the "for" loop

       break;      // Breaks out of the "while" loop and does the rest of
                   // the "for" loop body;
      }
     }
    }
    // Error code

## Alias
It is often the case where the same expression is used many times, yet cannot be simply assigned to a variable.  In this case, it is convenient to define an alias for the expression, as follows:

    :::C++
    alias C   = Namespace.ClassInstance;
    alias M   = SomeClass.Member;
    alias Clk = SYS_GLOBAL_CLK;

    C.Member = 3;
    M = 5;
    fsm(Clk){
     // Some statements...
    }

It is important to note, however, that the alias is evaluated first, before used in the target expression.  The following two lines are equivalent:

    :::C++
    alias sum = A + B; X = Y * sum;
    X = Y * (A + B);

[[include repo=code path=Wiki/MarkDown/Footer.md]]

