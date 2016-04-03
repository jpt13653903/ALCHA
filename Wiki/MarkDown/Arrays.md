[[include repo=doccode path=MarkDown/Header.md]]

[TOC]

# Arrays
ALCHA only supports fixed-length arrays.  Arrays can be defined with any number of dimensions.

## Defining Arrays
Each new dimension is defined by means of a positive integer in square brackets, representing the number of elements in that dimension.  An example is presented below:

    :::C++
    sig'8 A[16]; // A 16-element array of 8-bit unsigned integers
    sig'8 B[3][4][5]; // A 3-dimensional array of 8-bit unsigned integers

## Slices
Array elements can be addressed by means of slices.  The bits are modelled as the deepest dimension, with indices equal to the power-of-two representation of that bit.  The element indices are zero-based.  An empty slice ("`[]`") can be used to indicate "all elements".  Any bit-slice removes the fixed-point format.  The result of a bit-slice is always an unsigned integer with as many bits as in the slice.  Some examples are presented below:

    :::C++
    sig'(16, 8) A[16];
    A[5]            // Element 5 of A (the 6th element).
    A[][15->13]     // All the elements of A, but only the integer bits
    A[3~7~2][12->0] // Elements 3, 7 and 2 of A (in that order), and only the fraction bits
    A[0->15@3]      // Every third element of A (0, 3, 6, 9, 12 and 15)

    A[5~3~7][6~7~2] // Bits -6, 7 and 2 (in that order) of elements 5, 3 and 7 (in that order)
                    // The result is a 3-element array of 3-bit integers

The indices can also be specified by means of an integer array, as follows:

    :::C++
    sig'8 A[16];
    int   B[5] = 4~2~7~1~12;
    A[B] // Elements 4, 2, 7, 1 and 12 of A, in that order
    A[B ~ 0->15] // Elements 4, 2, 7, 1, 12 and 0 to 15 of A

## Array Literals and Concatenation
Array literals are useful for vectorised operations.  There are many forms, which are illustrated in the example below:

    :::C++
    0~1~2~3        // 4-element array
    0->3           // Same as above
    0->4@2         // The elements 0, 2 and 4
    3->0           // The elements 3, 2, 1 and 0
    0->3 ~ 4->0@-2 // The elements 0, 1, 2, 3, 4, 2 and 0

## Vectorised Statements
Array slices can be used in vectorised statements.  Vectorised operations apply the operation to each element of the array as if it was specified separately for each element.  This works for scalar functions as well.  Below are some examples:

    :::C++
    auto Add(A, B){
     return A + B;
    }

    sig'8 A[16], B[16], Y[16];
    Y = A + B;     // Perform an element-wise addition
    Y = Add(A, B); // Equivalent to the addition above

    Y[0->15] = A[15->0] + B[0->15@2 ~ 1->15@2];
    // Equivalent to:
    Y[ 0] = A[15] + B[ 0];
    Y[ 1] = A[14] + B[ 2];
    Y[ 2] = A[13] + B[ 4];
    ...
    Y[13] = A[ 2] + B[11];
    Y[14] = A[ 1] + B[13];
    Y[15] = A[ 0] + B[15];

When an array is used as the condition in an `if` statement or `while` loop, the array is AND-reduced.  This allows a concise means by which to compare all elements in an array (or string).

When a scalar is involved in a vectorised operation, the scalar is repeated for every instance of the operation.  This can be used, for instance, to assign a scalar to every element of an array.

[[include repo=doccode path=MarkDown/Footer.md]]

