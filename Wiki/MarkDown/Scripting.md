[[include repo=code path=Wiki/MarkDown/Header.md]]

[TOC]

# Scripting Features
ALCHA provides a powerful scripting platform to control the generation of firmware.  Any scripting variable (or scripting expression) can be used where synthesisable code expects a constant, thereby providing powerful parametrisation features.  Scripting can further be used for conditional compilation based on various global parameters, including the `target` attributes.

## Mathematical Functions
ALCHA scripting has the following built-in mathematical functions:

Function | Description
-------- | -----------
`sin`    | Sine (radians)
`cos`    | Cosine (radians)
`tan`    | Tangent (radians)
`asin`   | Arc-sine
`acos`   | Arc-cosine
`atan`   | Arc-tangent
`sinh`   | Hyperbolic sine
`cosh`   | Hyperbolic cosine
`tanh`   | Hyperbolic tangent
`asinh`  | Hyperbolic arc-tangent
`acosh`  | Hyperbolic arc-cosine
`atanh`  | Hyperbolic arc-tangent
`log`    | Natural logarithm
`log2`   | Logarithm to base 2
`log10`  | Logarithm to base 10
`exp`    | Natural exponent
`pow`    | Exponent (takes 2 parameters)
`round`  | Round to nearest integer
`fix`    | Drops the part after the decimal point
`floor`  | Round to negative infinity
`ceil`   | Round to positive infinity
`rand`   | Random number between 0 and 1, inclusive
`real`   | The real part of a `complex` type
`imag`   | The imaginary part of a `complex` type
`abs`    | The magnitude of a `complex` type
`angle`  | The phase of a `complex` type, in radians
`fft`    | Returns the fast Fourier transform of an array of `float` or `complex`

Fast Fourier transforms (FFTs) are implemented by means of the [FFTW library](http://www.fftw.org/).  The implication is that all FFTs are double-precision.

## File I/O
File I/O is accomplished by means of four functions, described below:

Function    | Description
--------    | -----------
`read`      | Reads the given file into a `byte` array buffer
`write`     | Writes the given `byte` array to file
`textread`  | Reads the given UTF-8 file into an UTF-32 `char` array buffer
`textwrite` | Writes the given UTF-32 `char` array to an UTF-8 file

The file contents are kept in arrays, which are buffered in RAM.  This limits the number, and size, of files that can be handled at the same time.  The memory is freed when the array life-time expires.  Developers are therefore encouraged to keep arrays meant for large files local to functions, or make use of the same array for multiple files.

## Dynamic Arrays
Scriping arrays can be declared as dynamic by specifying empty square brackets.  The array size is adjusted depending on the assignment.  While building the contents of a text file, for instance, the code might look as follows:

    :::C++
    char File[] = "";
    // Some code...
    File ~= "Header\n";
    // Some more code...
    File ~= "Some Body...\n";
    // Some more code...
    File ~= "Some Body...\n";
    // Some more code...
    File ~= "Some Body...\n";
    textwrite("My Log File.log", File);

## Shell Commands
ALCHA has a built-in function `shell` to execute shell commands.  It takes a single parameter (a string) that is executed as if called on the command-line.  This can be used, for instance, to call an external compiler for CPU code.  The resulting CPU binary can be imported by means of file I/O functions and used to initialise the CPU ROM.

[[include repo=code path=Wiki/MarkDown/Footer.md]]

