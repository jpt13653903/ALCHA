[[include repo=code path=Wiki/MarkDown/Header.md]]

[TOC]

# Scripting Features

ALCHA provides a powerful scripting platform to control the generation of firmware.  Any scripting variable (or scripting expression) can be used where synthesisable code expects a constant, thereby providing powerful parametrisation features.  Scripting can further be used for conditional compilation based on various parameters.

### Algorithmic Code Generation

Like [Migen](https://github.com/m-labs/migen), ALCHA supports algorithmic code generation.  During compilation, the abstract syntax tree is evaluated in two passes.  The first pass evaluates the scripting statements, while ignoring any synthesisable code.  This reduces all scripting statements to constants.  The second pass synthesises the intended circuit, based of the reduced abstract syntax tree.

The user can, for example, use a scripting `for` loop to generate a `switch`-statement based look-up table, as illustrated in the code below.  The scripting interpreter will duplicate the part of the AST containing the `case` and assignment, updating the constants during each iteration.  The same feature can be used to implement conditional compilation.

```C++
  net( 5) x;
  net(32) y;
  num     n;
  
  switch(x){
    for(n in 0..31){
      case(n) y = (2**32-1) >> (31-n);
    }
  }
```

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

Fast Fourier transforms (FFTs) are implemented by means of the [FFTW library](http://www.fftw.org/).  The implication is that all FFTs are double-precision, not the arbitrary precision available in the `num` type in general.

## I/O Functions

ALCHA supports two types of file access: binary and text-based.  Binary files make use of the `byte` base type, whereas text files make use of the `char` base type.  A `byte` is an 8-bit one-to-one mapping to the physical file bytes.  A `char` is a UTF-32 representation of a UTF-8 encoded file.  The code below shows an example of how file I/O is used in ALCHA.

```C++
  byte Data[] = read("MyDataFile.dat");
  char Log [] = "";  // An array of UTF-32 characters
  
  byte data;
  for(data in Data){
    !! Do some stuff, possibly appending to the Log array
  }
  textwrite("MyLogFile.log", Log); // Writes the log to file, in UTF-8 format.
```

The `read` function acts as if it reads the entire file into the array provided.  The actual implementation, however, might involve caching strategies implemented by the scripting interpreter.  The `write` function has no such luxury, because the user must define the entire array contents before writing it to the file.

In order to support large files for writing, the user can make use of the `append` family of functions instead.  It is up to the scripting interpreter to decide whether or not to keep the file open between function calls.  The ALCHA I/O related functions are listed below:

Function     | Description
--------     | -----------
`read`       | Reads the given file into a `byte` array buffer
`write`      | Writes the given `byte` array to a file
`append`     | Appends the given `byte` array to a file
`textread`   | Reads the given UTF-8 file into a UTF-32 `char` array buffer
`textwrite`  | Writes the given UTF-32 `char` array to a UTF-8 file
`textappend` | Appends the given UTF-32 `char` array to a UTF-8 a file
`print`      | Prints a string to the command-line

The core ALCHA language does not have functions to read specific file formats, such as CSV, XML or JSON, as these can be provided by means of libraries.  A similar library function can be developed to, for example, read in a PCB net-list and dynamically build the pin definitions.  It is impractical to support all such file formats as part of the core language, but essential to provide sufficient functionality that the user can, with relative ease, develop a parser for any arbitrary file format.

## Conversion Functions

While reading and writing text-based files, it is often required to convert between numerical variables (of type `num`) and strings.  The functions presented below can be used for this purpose:

Function  | Description
--------  | -----------
`$x`      | Converts variable `x` to a string.
`$(x, S)` | Converts expression `x` to a string.  The base and other parameters are controlled by means of the optional format string `S`, which takes a C `printf`-style format string.
`eval(S)` | Converts a string `S` to a number.  The scripting interpreter makes use of the same expression engine used in [Engineering Calculator](https://sourceforge.net/p/alwaysontopcalc/wiki).

It is similarly useful to be able to convert `byte` arrays to `num` types, and vice versa.  A `byte` array can be assigned directly to a `num` type, and vice versa, in which case the array is evaluated in big Endian order (so that the assignments are compatible with packed structures).  The code below shows some examples.

```C++
  byte b[4] = [1, 2, 3, 4]; // b[0] = 1; b[1] = 2; ...
  num  n;
  
  n = b;      // assigns 0x01020304 to n
  n = b[0..3] // assigns 0x01020304 to n
  n = b[3..0] // assigns 0x04030201 to n
  n = b[3, 1] // assigns 0x00000402 to n
  
  n       = 0x05060708;
  b       = n; // assigns b[0] = 5; b[1] = 6; ...
  b[3..0] = n; // assigns b[3] = 5; b[2] = 6; ...
```

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

ALCHA scripting is not sufficient for all scenarios of circuit parametrisation.  When the design contains a processor, for instance, the user might want the ALCHA compiler to generate a C include-file with all the register addresses, call the C compiler, and then import the resulting executable file into a ROM block on the FPGA.

External commands can be called by means of the `shell` function.  It returns the error code of the function called.  The `shell` function is a blocking call, so that the external tool can finish before the ALCHA script interpreter continues.  The code below shows an example.

```C++
  !! Dynamically build the register addresses here
  
  textwrite("RegDefs.h", RegDefs);
  if(shell("gcc main.c -o bin/MyCPU")) error("Cannot compile MyCPU");
  byte MyCPU[] = read("bin/MyCPU");
  
  !! Initialise the CPU ROM here
```

Another use for this mechanism is to call an external scripting tool to plot data.  This could be useful to visualise the data used to initialise an instance of ROM.

[[include repo=code path=Wiki/MarkDown/Footer.md]]

