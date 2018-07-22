[[include repo=code path=Wiki/MarkDown/Header.md]]

[TOC]

# Scripting Features

ALCHA provides a powerful scripting platform to control the generation of firmware.  Any scripting variable (or scripting expression) can be used where synthesisable code expects a constant, thereby providing powerful parametrisation features.  Scripting can further be used for conditional compilation based on various parameters.

During compilation, the abstract syntax tree is evaluated in two passes.  The first pass evaluates the scripting statements, while ignoring any synthesisable code.  This reduces all scripting statements to constants.  The second pass synthesises the intended circuit, based of the reduced abstract syntax tree.

All ALCHA features are available for scripting use, including, among others, functions, classes, array slicing and vectorised expressions.

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
`real`   | The real part of a complex number
`imag`   | The imaginary part of a complex number
`abs`    | The magnitude of a complex number
`angle`  | The phase of a complex number, in radians
`fft`    | Returns the fast Fourier transform of an array of `num`

Fast Fourier transforms (FFTs) are implemented by means of the [FFTW library](http://www.fftw.org/).  The implication is that all FFTs are double-precision.

## I/O Functions

Arbitrary input and output is accomplished by means of five functions, described below:

Function    | Description
--------    | -----------
`read`      | Reads the given file into a `byte` array buffer
`write`     | Writes the given `byte` array to file
`textread`  | Reads the given UTF-8 file into a UTF-32 `char` array buffer
`textwrite` | Writes the given UTF-32 `char` array to a UTF-8 file
`print`     | Prints a string to the command-line

The file contents are kept in arrays, which are buffered in RAM.  Large files are handled by means of dynamic paging.  The ALCHA user is unaware of this caching mechanism.

## Conversion Functions

While reading and writing text-based files, it is often required to convert between numerical variables (of type `num`) and strings.  The functions presented below can be used for this purpose:

Function  | Description
--------  | -----------
`$x`      | Converts variable `x` to a string.
`$(x, S)` | Converts expression `x` to a string.  The base and other parameters are controlled by means of the optional format string `S`.
`eval(S)` | Converts a string `S` to a number.  The scripting interpreter makes use of the same expression engine used in [Engineering Calculator](https://sourceforge.net/p/alwaysontopcalc/wiki).

## Dynamic Arrays

Scriping arrays can be declared as dynamic by specifying empty square brackets.  The array size is adjusted depending on the assignment.  While building the contents of a text file, for instance, the code might look as follows:

```C++
  char Buffer[] = "";
  // Some code...
  Buffer ~= "Header\n";
  // Some more code...
  Buffer ~= "Some Body...\n";
  // Some more code...
  Buffer ~= "Some Body...\n";
  // Some more code...
  Buffer ~= "Some Body...\n";
  textwrite("My Log Buffer.log", Buffer);
```

## Shell Commands

ALCHA has a built-in function `shell` to execute shell commands.  It takes a single parameter (a string) that is executed as if called on the command-line.  This can be used, for instance, to call an external compiler for CPU code.  The resulting CPU binary can be imported by means of file I/O functions and used to initialise the CPU ROM.

Another use for this mechanism is to call an external scripting tool to plot data.  This could be useful to visualise the data used to initialise an instance of ROM.

[[include repo=code path=Wiki/MarkDown/Footer.md]]

