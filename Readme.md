# ALCHA

<img src="https://openclipart.org/download/3850/dchandlr-dchandlr-work.svg" height="70" alt="Work in Progress"/>
A New Programming Language for FPGA Projects &ndash; **currently in pre-alpha**.

ALCHA aims to reduce FPGA project develop time by means of automation and
abstraction, but without loosing the low-level control that HDLs,
such as VHDL or Verilog, provide.

## Features

- Compile to a complete vendor-specific project from largely vendor-neutral source
- Significantly reduced routing and wiring
- Procedural state-machine programming model
- Object oriented programming model
- Abstract data and signal types
- Compile-time scripting
- Automated timing-requirement generation
- Bit- and timing-accurate circuit description

## Syntax Highlighting

The following syntax highlighting options are available:

- Legacy (and unmaintained) [vim syntax script][vim_syntax]
- [Tree-sitter grammar][tree-sitter] (intended for Neovim)

## How to Build

When cloning the repository, remember to also clone the sub-modules.

### Windows

1. Install [TDM GCC MinGW 64-bit][MinGW] version 10.3.0 or newer.
1. Open a POSIX-like terminal (such as [git bash][gitBash]).
1. Change directory to the project folder.
1. Run `mingw32-make`

### Linux

1. Install a C++20 capable version of GCC.
1. Open a bash (or sh) terminal.
1. Install the development versions of `libgmp`, `libmpfr` and `libfftw3`.
   On Ubuntu, this can be achieved with:
    - `sudo apt-get install libgmp-dev`
    - `sudo apt-get install libmpfr-dev`
    - `sudo apt-get install libfftw3-dev`
1. Change directory to the project folder.
1. Run `make`

## Running Tests

To run the automated tests, run `make test` from the root folder.

You can also run specific tests from within the [`Tests`](Tests) folder:

- `make test`: Run all the tests
- `make testScanner`: Run and test the scanner
- `make testParser`: Run and test the parser
- `make testInterpreter`: Run and test the interpreter

## More Information

Alcha was the subject of [a PhD thesis][PhD].

See the [Wiki](Wiki) for more information.

--------------------------------------------------------------------------------

[MinGW]:       https://jmeubank.github.io/tdm-gcc
[gitBash]:     https://git-scm.com
[PhD]:         http://hdl.handle.net/11427/37117
[vim_syntax]:  https://github.com/jpt13653903/ALCHA/blob/6c6c2cd/Syntax%20Highlighting/Vim/alcha.vim
[tree-sitter]: https://github.com/jpt13653903/tree-sitter-alcha

