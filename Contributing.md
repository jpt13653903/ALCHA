# Contributing to ALCHA

ALCHA is in the planning / pre-alpha stage of development.  This means that
the intended functionality and code architecture can change at any moment.  It
therefore does not make much sense to contribute to the code base at this
early stage.

This said, any contribution to planning will be greatly appreciated.

## General Workflow

If you'd like to make contributions to the project, the preferred method is to
create a fork, make the changes and then [create a pull request][pull_request]
for that fork.

## Tests

When adding a feature, make sure to also add a test for that feature.
The various stages of compilation are tested separately:

- `Tests/testScanner`
- `Tests/testParser`
- `Tests/testInterpreter`
- etc.

The testing methodology should be reasonably self-explanatory.

## Build environment

The build process is dependent on:

- A C++20 capable version of GNU G++ (use TDM GCC MinGW on Windows)
- GNU Make
- Python (optional)
    - Python is used to auto-gen various parts of the code-base, but the output
      of the auto-gen process is included in the repository.
    - At the time of writing this, Python 3.6.5 was used.

To build the project, run "make" from the project root.

## Dependencies

Please keep dependencies to an absolute minimum.  The vision is that the
target user can download a single executable file and then use it.  Any
framework or library in use should either be so standard that all target
systems are guaranteed to have it (C run-time, Win32 API on Windows, POSIX
Threads on Linux, etc.) or statically linked into the project binary.

Another reason is to ease the build process for developers.  Developers should
be able to clone the repository and run "make", without installing anything
other than the C++ compiler.

## Language Syntax and Features

The intended language syntax and features are described in the [wiki].
Suggestions and corrections regarding the language syntax and features can be
done as follows:

- Modify the [wiki] source directly and [create a pull request][pull_request] or
- [Create an issue][issue].

## Diagrams and Figures

The documentation includes various diagrams, all of which are stored in the
[Wiki/Figures][wiki_figures] folder.  The preferred format is [InkScape]-based
scalable vector graphics.  Most diagram editing tools can export SVG files.

--------------------------------------------------------------------------------

[issue]:        https://github.com/jpt13653903/ALCHA/issues
[pull_request]: https://github.com/jpt13653903/ALCHA/pulls
[wiki]:         https://github.com/jpt13653903/ALCHA/tree/interpreter/Wiki
[wiki_figures]: https://github.com/jpt13653903/ALCHA/tree/master/Wiki/Figures
[InkScape]:     https://inkscape.org

