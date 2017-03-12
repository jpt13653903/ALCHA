[[include repo=code path=Wiki/MarkDown/Header.md]]

[TOC]

# Expressions

Combinational statements in ALCHA are evaluated in order.  In terms of HDL, the statements are considered blocking.

Assignments come in two flavours.  Normal assignments use the `=` operator and take the fixed-point format into account, whereas raw assignments use the `:=` operator and assigns the raw bits only.

During normal assignment, the source is type-cast to the target type before assignment.  If the target does not have sufficient bits to store the source, the source is truncated.  When most-significant bits are removed, the compiler issues a warning.  When least-significant bits are removed, no warning is issued.  It is up to the developer to ensure that correct rounding is applied, if required.

In order to prevent accidental variable creation through typos, all variables, nets, etc. must be defined before use.  It is useful, however, to be able to define a variable based on the type of the first expression assigned to it.  It that case, define it with `auto` type.  Once defined in such a manner, that variable cannot change its type, and redefining it is illegal.

## Operators
The following table summarises ALCHA operators, in order of precedence:

Group | Operator | Function
----- | -------- | --------
Group | `(...)`  | Grouping
||
Cast | `A ' N`        | Casts `A` to an `N`-bit integer
     | `A '(N, s)`    | Casts `A` to an `N`-bit fixed-point in the range [0, s)
     | `A '(N,-s)`    | Casts `A` to an (`N`+1)-bit fixed-point in the range [-s, s)
     | `A 'Class`     | Casts `A` to `Class`, provided `Class` is in A's inheritance graph
     | `A '(N.Class)` | Casts `A` to `Class`, where `Class` is in namespace `N`
||
Post-fix | `A++`  | Increment after use
         | `A--`  | Decrement after use
         | `A!`   | Factorial
         | `A.B`  | Access `B` in namespace (or class instance) `A`
         | `A(B)` | Call function `A` with parameter(s) `B`
         | `A[B]` | Slice array `A` with elements in array `B`
||
Unary | `++A` | Increment before use
      | `--A` | Decrement before use
      |  `-A` | Negate
      |  `~A` | Bitwise NOT
      |  `:A` | Cast `A` to raw bits (unsigned integer)
||
Range | `A -> B`     | Builds an array of elements from `A` to `B`, inclusive
      | `A -> B @ C` | Builds an array of elements from `A` to `B`, in steps of `C`
||
Reduction | ` &A`        | AND-reduce
          | `~&A`        | NAND-reduce
          |    &#124;`A` | OR-reduce
          | `~`&#124;`A` | NOR-reduce
          | ` #A`        | XOR-reduce
          | `~#A`        | XNOR-reduce
          | ` !A`        | Logical compliment
||
Concatenate | `A:B:C` | Bit-wise concatenate
||
Replicate   | `A \ B` | Bit-wise replicate `A`, `B` times
||
Array | `A ~ B ~ C` | Concatenate arrays `A`, `B` and `C`
||
Exponential | `A ^ B` | Raise `A` to the power of `B`
||
Arithmetic | `A * B` | Multiply
           | `A / B` | Divide
           | `A % B` | Modulus
||
           | `A + B`  | Add
           | `A - B`  | Subtract
||
Shift      | `A << B` | Shift `A` left by `B` bits
           | `A >> B` | Shift `A` right by `B` bits
||
Relational | `A >  B` | Greater than
           | `A <  B` | Less than
           | `A >= B` | Greater than or equal to
           | `A <= B` | Less than or equal to
||
           | `A == B` | Equal to
           | `A != B` | Not equal to
||
Bit-wise | `A  & B` | Bit-wise AND
         | `A ~& B` | Bit-wise NAND
||
         | `A  # B` | Bit-wise XOR
         | `A ~# B` | Bit-wise XNOR
||
         | `A `&#124;` B`  | Bit-wise OR
         | `A `~&#124;` B` | Bit-wise OR
||
Logical | `A && B`             | Logical AND
||
        | `A `&#124;&#124;` B` | Logical OR
||
Conditional | `A ? B : C` | If `A`, then `B`, else `C`
||
Assignment | `A   = B`       | Normal assign (automatically casts to the target fixed-point format)
           | `A  := B`       | Raw assign (assigns bit-for-bit, ignoring the fixed-point format)
           | `A  ~= B`       | Append array `B` to `A` (equivalent to `A = A~B` and only applicable to dynamic arrays, which are available during scripting)
           | `A  += B`       | Add and assign (normal assign)
           | `A  -= B`       | Subtract and assign (normal assign)
           | `A  *= B`       | Multiply and assign (normal assign)
           | `A  /= B`       | Divide and assign (normal assign)
           | `A  %= B`       | Modulus and assign (normal assign)
           | `A  ^= B`       | Exponential and assign (normal assign)
           | `A  &= B`       | Bit-wise AND and assign (raw assign)
           | `A  `&#124;=` B`| Bit-wise OR and assign (raw assign)
           | `A  #= B`       | Bit-wise XOR and assign (raw assign)
           | `A <<= B`       | Shift left and assign (normal assign)
           | `A >>= B`       | Shift right and assign (normal assign)

The unary, arithmetic, shift and relational operators take the fixed-point format of the operands into account, whereas the other operators do not.  Arithmetic operators result in a fixed-point number which is of such a format that no bits are lost in the calculation.  Other operators result in an unsigned integer, regardless of input format.

The target of an assignment can be an array, array slice or concatenation.

The usual flow-control structures (`if`, `for`, `while`, etc.) are supported.  More detail is provided later.

## Net use Before Assignment
When describing FPGA firmware it is often necessary to use a net before a value is assigned.  This could occur when using parametrised classes, or clocked structures with external combinational feedback.  The code below provides two examples:

    :::C++
    class Adder(N){ // Parametrisation is presented in the scripting section
     net'N A, B;
     net'N Y;
     Y = A + B;
    }
    Adder MyAdder;      // Creates a reference to MyAdder, no object is created yet
    MyAdder = Adder(8); // Creates an instance of an 8-bit adder and assigns the
                        // object to the MyAdder reference.  At this point, "Y" is
                        // a reference to the expression "Y = A + B", but "A" and "B"
                        // are null references.
    net'8 x, y, z;
    MyAdder.{ // Pushes the name-space "MyAdder" onto the name-space stack
     A = x; // Assign the expression "x" to the reference "A"
     B = y; // Assign the expression "y" to the reference "B"
     z = Y; // Assign the expression "Y" to the reference "z"
    }

The Verilog equivalent of the above is:

    :::Verilog
    module Adder #(parameter N)(
     input  [N-1:0]A,
     input  [N-1:0]B,
     output [N-1:0]Y
    );
     assign Y = A + B;
    endmodule

    module TopLevel;
     wire [7:0]x;
     wire [7:0]y;

     Adder #(8) MyAdder(x, y, z);
    endmodule

And here is another example:

    :::C++
    net'8 Counter;
    UserClk = (Counter == 125); // "Counter" has no value yet
    rtl(CPUClock){
     if(UserClk) Counter = 0;
     else        Counter++;
    }

Internally, ALCHA implements the blocking nature of combinational assignments by using a copy of the source expression-tree, rather that the source net itself.  When that source has no expression-tree yet, a reference to the net is used instead.  A simplified example is presented below:

    :::C++
    net'8 A, B, C, D;
    A = C;
    B = 5 * D;
    C = B + 7;

    // At this point, the expression trees look as follows:
    A = C;
    B = 5 * D;
    C = (5 * D) + 7;
    D = null

It is possible to create circular references, which is illegal.  This is illustrated below:

    :::C++
    net'8 A, B, C;
    A = C;
    B = 5 * C;
    C = B + 7;

    // At this point, the expression trees look as follows:
    A = C;
    B = 5 * C;
    C = (5 * C) + 7; // Circular reference => illegal

This only applies to combinational circuits.  Clocked structures are evaluated as non-blocking assignments, so clocked circular references are legal.

An important consequence of this mechanism is that the developer must keep the dependencies in mind.  This is illustrated below:

    :::C++
    net'8 A, B, C, D, E;
    A  = C;
    B  = 5 * D;
    C  = B + 7;
    E  = C - 9;
    B  = 2;
    C  = 3;
    D  = 4;
    A += D;

    // At this point, the expression trees look as follows:
    A = C + 4;
    B = 2;
    C = 3;
    D = 4;
    E = ((5 * D) + 7) - 9;
    // Any further changes to C will affect A, but not E.
    // Any further changes to D will affect E, but not B.

This can become quite confusing.  Developers are therefore encouraged to avoid net use before assignment when describing combinational circuits.  Developers are also encouraged to avoid assigning more than one expression to the same net, unless the intention is clear from the code.

Another way to avoid unintended side-effects is to always build the circuit in one direction: either output to input, or input to output, depending on which is more appropriate.  This is illustrated below:

    :::C++
    pin'8 A, B, C, D, Y;
    net'8 E, F;

    // Build the circuit input to output (consistently assign before use):
    E  = A;
    F  = C;
    E *= B;
    F *= D;
    Y  = E + F;

    // Build the circuit output to input (consistently use before assign):
    Y  = E + F;
    E  = A;
    F  = C;
    E *= B;
    F *= D;

    // This results in an unintended, wrong, circuit:
    E  = A;
    F  = C;
    Y  = E + F;
    E *= B; // Changes the expression tree of E, but Y remains unchanged
    F *= D; // Changes the expression tree of F, but Y remains unchanged

It is sometimes useful to use non-blocking assignments in combinational circuits.  The last example in the code above can be "fixed" as follows:

    :::C++
          E  = A;
          F  = C;
    alias S  = E + F;
          E *= B;
          F *= D;
          Y  = S;

[[include repo=code path=Wiki/MarkDown/Footer.md]]

