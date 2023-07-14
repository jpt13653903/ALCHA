# ALCHA

A New Programming Language for FPGA Projects &ndash; currently in pre-alpha.

ALCHA aims to reduce FPGA project develop time by means of automation and abstraction,
but without loosing the low-level control that HDLs, such as Verilog, provide.

## Features

- Compile to complete vendor-specific project from largely vendor-neutral source
- Object oriented programming model
- Proceedural state-machine programming model
- Abstract data and signal types
- Compile-time scripting
- Automated timing-requirement generation
- Bit- and timing-accurate circuit description

## How to Build

When cloning the repository, remember to also clone the sub-modules.

### Windows

1. Install [TDM GCC MinGW](http://tdm-gcc.tdragon.net/) version 5.1.0 or newer.  Currently, only the 32-bit version links to the libraries provided.
1. Open a POSIX-like terminal (such as [git bash](https://git-scm.com/)).
1. Change directory to the project folder.
1. Run `mingw32-make`

### Linux

1. Install a C++11 capable version of GCC.
1. Open a bash (or sh) terminal.
1. Install the development versions of `libgmp`, `libmpfr` and `libfftw3`.  On Ubuntu, this can be achieved with:
    - `sudo apt-get install libgmp-dev`
    - `sudo apt-get install libmpfr-dev`
    - `sudo apt-get install libfftw3-dev`
1. Change directory to the project folder.
1. Run `make`

## Running Test Cases

To run the test cases, change directory into the `Source` folder, then run `make test????`.  Replace the question-marks with the appropriate subfolder in the `TestCases` folder.

The functional test-cases at the moment are:

- `make testButtonsToLEDs`: a minimal test-case used to test that the logic of the implemented architecture is working.
- `make testCombinational`: A more thorough test of combination circuits (and various scripting structures).

## More Information

Alcha was the subject of [a PhD thesis](http://hdl.handle.net/11427/37117).

See the [Wiki](https://sourceforge.net/p/alcha/wiki) for more information.

