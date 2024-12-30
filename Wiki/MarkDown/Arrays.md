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
- [Arrays](Arrays.md#arrays)
  - [Defining Arrays](#defining-arrays)
  - [Slices](#slices)
  - [Array Literals and Concatenation](#array-literals-and-concatenation)
  - [Vectorised Statements](#vectorised-statements)
- [Functions](Functions.md)
- [Synchronous Circuits](SynchronousCircuits.md)
- [Classes](Classes.md)
- [Operator Overloading](OperatorOverloading.md)
- [Scripting Features](Scripting.md)
- [Advanced Attributes](AdvancedAttributes.md)
- [High-level Structures](HighLevelStructures.md)
- [Simulation and Verification](Simulation.md)

--------------------------------------------------------------------------------

# Arrays

ALCHA supports fixed-length arrays for synthesisable types, and
variable-length arrays for scripting types.  Multiple-dimensional arrays are
defined in terms of "arrays of arrays".

## Defining Arrays

Each new dimension is defined by means of a positive integer in square
brackets, representing the number of elements in that dimension.  An example
is presented below:

```alcha
  net(8) A[16];      // A 16-element array of 8-bit unsigned integers
  net(8) B[3][4][5]; // A 3-dimensional array of 8-bit unsigned integers
```

## Slices

Array elements can be addressed by means of slices.  The bits are modelled as
the deepest dimension.  The element indices are zero-based, i.e. index&nbsp;0
is the least significant bit of a bit-vector, or the first element in an
array.  An empty slice ("`[]`") can be used to indicate "all elements".

Any bit-slice removes the fixed-point format.  The result of a bit-slice is
always an unsigned integer with as many bits as in the slice.  Some examples
are presented below:

```alcha
  net(16, 8) A[16];
  A[5]            // Element 5 of A (the 6th element).
  A[][15->13]     // All the elements of A, but only the integer bits
  A[3,7,2][12..0] // Elements 3, 7 and 2 of A (in that order), and only the fraction bits
  A[0..15:3]      // Every third element of A (0, 3, 6, 9, 12 and 15)

  A[5,3,7][6,7,2] // Bits 6, 7 and 2 (in that order) of elements 5, 3 and 7 (in that order)
                  // The result is a 3-element array of 3-bit integers
```

The indices can also be specified by means of a number array, as follows:

```alcha
  net(8) A[16];
  num    B[5] = [4, 2, 7, 1, 12];
  A[B]        // Elements 4, 2, 7, 1 and 12 of A, in that order
  A[B, 0..15] // Elements 4, 2, 7, 1, 12 and 0 to 15 of A
```

## Array Literals and Concatenation

Array literals are useful for vectorised operations.  There are many forms,
which are illustrated in the example below:

```alcha
  [0, 1, 2, 3]    // 4-element array
  [0..3]          // Same as above
  [0..4:2]        // The elements 0, 2 and 4
  [3..0]          // The elements 3, 2, 1 and 0
  [0..3, 4..0:-2] // The elements 0, 1, 2, 3, 4, 2 and 0
```

## Vectorised Statements

Array slices can be used in vectorised statements.  Vectorised operations
apply the operation to each element of the array as if it was specified
separately for each element.  This works for scalar functions as well.  Below
are some examples:

```alcha
  auto Add(A, B){
    return A + B;
  }

  net(8) A[16], B[16], Y[16];
  Y = A + B;     // Perform an element-wise addition
  Y = Add(A, B); // Equivalent to the addition above

  Y[0..15] = A[15..0] + B[0..15:2, 1..15:2];
  // Equivalent to:
  Y[ 0] = A[15] + B[ 0];
  Y[ 1] = A[14] + B[ 2];
  Y[ 2] = A[13] + B[ 4];
  ...
  Y[13] = A[ 2] + B[11];
  Y[14] = A[ 1] + B[13];
  Y[15] = A[ 0] + B[15];
```

When an array is used as the condition of an `if` statement or `while` loop,
the array is AND-reduced.  This allows a concise means by which to compare all
elements in an array (or string).  Any other intention must be written
explicitly in terms of a loop structure.

When a scalar is involved in a vectorised operation, the scalar is repeated
for every instance of the operation.  This can be used, for instance, to
assign a scalar to every element of an array.

--------------------------------------------------------------------------------

