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
- [Expressions](Expressions.md#expressions)
  - [Operators](#operators)
  - [Global Attribute Definition and Access](#global-attribute-definition-and-access)
  - [Net use Before Assignment](#net-use-before-assignment)
- [Statements](Statements.md)
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

# Expressions

Combinational statements in ALCHA are evaluated in order.  In terms of HDL,
the statements are considered blocking.

Assignments come in two flavours.  Normal assignments use the `=` operator and
take the fixed-point format into account, whereas raw assignments use the
`:=` operator and assigns the raw bits only.

During normal assignment, the source is type-cast to the target type before
assignment.  If the target does not have sufficient bits to store the source,
the source is truncated.  When most-significant bits are removed, the compiler
issues a warning.  When least-significant bits are removed, no warning is
issued.  It is up to the developer to ensure that correct rounding is applied,
if required.

In order to prevent accidental variable creation through typos, all variables,
nets, etc. must be defined before use.  It is useful, however, to be able to
define a variable based on the type of the first expression assigned to it.
It that case, define it with `auto` type.  Once defined in such a manner, that
variable cannot change its type, and redefining it is illegal.

## Operators

The following table summarises ALCHA operators, in order of precedence:

Group | Operator | Function
----- | -------- | --------
Group | `(...)`  | Grouping
||
Array |  `[A, B, C]` | Build array from elements `A`, `B` and `C`
||
Concatenate | `:(A, B, C)` | Bit-wise concatenate the vectors `A`, `B` and `C`
&nbsp;      | `:[A, B, C]` | Concatenate arrays `A`, `B` and `C`
||
Cast   | `A @ N`        | Casts `A` to an `N`-bit integer
&nbsp; | `A @(N, s)`    | Casts `A` to an `N`-bit fixed-point in the range [0, s)
&nbsp; | `A @(N,-s)`    | Casts `A` to an (`N`+1)-bit fixed-point in the range [-s, s)
&nbsp; | `A @Class`     | Casts `A` to `Class`, provided `Class` is in A's inheritance graph
&nbsp; | `A @(N.Class)` | Casts `A` to `Class`, where `Class` is in namespace `N`
||
Post-fix | `A++`  | Increment after use
&nbsp;   | `A--`  | Decrement after use
&nbsp;   | `A!`   | Factorial
&nbsp;   | `A.B`  | Access `B` in namespace (or class instance) `A`
&nbsp;   | `A'B`  | Access the attribute `B` of `A`
&nbsp;   | `A(B)` | Call function `A` with parameter(s) `B`
&nbsp;   | `A[B]` | Slice array `A` with elements in array `B`
||
Unary | `++A` | Increment before use
&nbsp;| `--A` | Decrement before use
&nbsp;|  `-A` | Negate
&nbsp;|  `~A` | Bitwise NOT
&nbsp;|  `:A` | Cast `A` to raw bits (unsigned integer)
&nbsp;|  `'A` | Access the global attribute `A`
||
Range | `A .. B`     | Builds an array of elements from `A` to `B`, inclusive
&nbsp;| `A .. B : C` | Builds an array of elements from `A` to `B`, in steps of `C`
||
Reduction | ` &A`        | AND-reduce
&nbsp;    | `~&A`        | NAND-reduce
&nbsp;    |    &#124;`A` | OR-reduce
&nbsp;    | `~`&#124;`A` | NOR-reduce
&nbsp;    | ` ^A`        | XOR-reduce
&nbsp;    | `~^A`        | XNOR-reduce
&nbsp;    | ` !A`        | Logical compliment
||
Replicate   | `A`&#0096;`B` | Bit-wise replicate `A`, `B` times
||
Exponential | `A ** B` | Raise `A` to the power of `B`
||
Arithmetic | `A * B` | Multiply
&nbsp;     | `A / B` | Divide
&nbsp;     | `A % B` | Modulus
||
&nbsp;     | `A + B`  | Add
&nbsp;     | `A - B`  | Subtract
||
Shift      | `A << B` | Shift `A` left by `B` bits
&nbsp;     | `A >> B` | Shift `A` right by `B` bits
||
Relational | `A >  B` | Greater than
&nbsp;     | `A <  B` | Less than
&nbsp;     | `A >= B` | Greater than or equal to
&nbsp;     | `A <= B` | Less than or equal to
||
&nbsp;     | `A == B` | Equal to
&nbsp;     | `A != B` | Not equal to
||
Bit-wise | `A  & B` | Bit-wise AND
&nbsp;   | `A ~& B` | Bit-wise NAND
||
&nbsp;   | `A  ^ B` | Bit-wise XOR
&nbsp;   | `A ~^ B` | Bit-wise XNOR
||
&nbsp;   | `A `&#124;` B`  | Bit-wise OR
&nbsp;   | `A `~&#124;` B` | Bit-wise OR
||
Conditional | `A ? B : C` | If `A`, then `B`, else `C`
&nbsp;      | `A ?: C`    | If `A`, then `A`, else `C`
||
Assignment | `A   = B`       | Normal assign (automatically casts to the target fixed-point format)
&nbsp;     | `A  := B`       | Raw assign (assigns bit-for-bit, ignoring the fixed-point format)
&nbsp;     | `A  ~= B`       | Append array `B` to `A` (equivalent to `A = :[A, B]` and only applicable to dynamic arrays, which are available during scripting)
&nbsp;     | `A  += B`       | Add and assign (normal assign)
&nbsp;     | `A  -= B`       | Subtract and assign (normal assign)
&nbsp;     | `A  *= B`       | Multiply and assign (normal assign)
&nbsp;     | `A  /= B`       | Divide and assign (normal assign)
&nbsp;     | `A  %= B`       | Modulus and assign (normal assign)
&nbsp;     | `A  ^= B`       | Exclusive OR and assign (raw assign)
&nbsp;     | `A  &= B`       | Bit-wise AND and assign (raw assign)
&nbsp;     | `A  `&#124;=` B`| Bit-wise OR and assign (raw assign)
&nbsp;     | `A  ^= B`       | Bit-wise XOR and assign (raw assign)
&nbsp;     | `A <<= B`       | Shift left and assign (normal assign)
&nbsp;     | `A >>= B`       | Shift right and assign (normal assign)

The unary, arithmetic, shift and relational operators take the fixed-point
format of the operands into account, whereas the other operators do not.
Arithmetic operators result in a fixed-point number which is of such a format
that no bits are lost in the calculation.  Other operators result in an
unsigned integer, regardless of input format.

The target of an assignment can be an array, array slice or concatenation.

The usual flow-control structures (`if`, `for`, `while`, etc.) are supported.
More detail is provided later.

## Global Attribute Definition and Access

Defining an attribute puts it in the current namespace.  Accessing a namespace
runs the hierarchy from the current namespace toward the root of the namespace
tree, which is the global attribute.  The current namespace attributes
therefore shadow the global attributes.  In addition, global attributes are
read-only from within a namespace.

## Net use Before Assignment

When describing FPGA firmware it is often necessary to use a net before a
value is assigned.  This could occur when using parametrised classes, or
clocked structures with external combinational feedback.  The code below
provides two examples:

```alcha
    class Adder(N){ // Parametrisation is presented in the scripting section
        net(N) A, B;
        net(N) Y;
        Y = A + B;
    }
    Adder(8) MyAdder; // Creates an instance of an 8-bit adder and assigns the
                      // object to the MyAdder reference.  At this point, "Y" is
                      // a reference to the expression "Y = A + B", but "A" and "B"
                      // are null references.
    net(8) x, y, z;
    MyAdder.{ // Pushes the name-space "MyAdder" onto the name-space stack
        A = x; // Assign the expression "x" to the reference "A"
        B = y; // Assign the expression "y" to the reference "B"
        z = Y; // Assign the expression "Y" to the reference "z"
    }
```

The Verilog equivalent of the above is:

```verilog
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
```

And here is another example:

```alcha
    net(8) Counter;
    UserClk = (Counter == 125); // "Counter" has no value yet
    rtl(CPUClock){
        if(UserClk) Counter = 0;
        else        Counter++;
    }
```

ALCHA is intended for describing hardware, so the defined `net` types
represent physical hardware nets.  When using that net in an assignment, the
ALCHA user is making connections, making a copy from one RAM location to
another.  This paradigm allows for use-before-assign type code.

ALCHA does, however, allow circular assignments.  These are performed by using
the following algorithm:

1. Evaluate the right-hand-side (RHS) expression to produce an expression tree
1. Inspect the left-hand-side (LHS) and the expression tree to determine
   whether or not the statement is circular
1. If it is circular, update the expression tree to use the current expression
   assigned to the LHS net, instead of the net itself
1. Assign the expression tree to the LHS net
1. Perform garbage collection on any resulting dangling circuits

--------------------------------------------------------------------------------

