[[include repo=doccode path=MarkDown/Header.md]]

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

[[include repo=doccode path=MarkDown/Footer.md]]

