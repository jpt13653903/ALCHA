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

## How to build

When cloning the repository, remember to also clone the sub-modules.

### Windows

1. Install [TDM GCC MinGW](http://tdm-gcc.tdragon.net/).  Currently, only the 32-bit version links to the libraries provided.
1. Open a POSIX-like terminal (such as [git bash](https://git-scm.com/)).
1. Change directory to the project folder.
1. Run `mingw32-make`

### Linux

1. Install a C++11 capable version of GCC.
1. Open a bash (or sh) terminal.
1. Install the development versions of `libfftw3`, `libgmp` and `libmpfr`.  On Ubuntu, this can be achieved with:
    - `sudo apt-get install libfftw3-dev`
    - `sudo apt-get install libgmp-dev`
    - `sudo apt-get install libmpfr-dev`
1. Change directory to the project folder.
1. Run `make`

## More information

See the [Wiki](https://sourceforge.net/p/alcha/wiki) for more information.

