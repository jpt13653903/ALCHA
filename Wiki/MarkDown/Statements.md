[[include repo=code path=Wiki/MarkDown/Header.md]]

[TOC]

# Statements
## Control Flow Structures
### if

    :::C++
    if(A){
     // if A is non-zero, do this
    }else{
     // otherwise do this
    }

### switch

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

### while

    :::C++
    while(A){
     // while A is non-zero, do this
    }

### for

    :::C++
    for(x in A){
     // for every element of array A, set x to that element and do this
    }

One can use array literals to make this easier:

    :::C++
    for(j in 0->7) A += S[j]; // Sums the first 8 elements of S

The above is equivalent to:

    :::C++
    for(s in S[0->7]) A += s;

### loop

    :::C++
    loop(N){
     // Do this N times
    }
    loop{
     // Do this forever
    }

### return

The `return` statement returns from a function, optionally with a value:

    :::C++
    int Func(A, B){
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
     for(j = 1->100){
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
    alias M   = C.Member;
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

