[[include repo=code path=Wiki/MarkDown/Header.md]]

[TOC]

# Modules
## Target Descriptor
Every ALCHA design must have a target descriptor.  There are two types: simulation and project.  A simulation target indicates a waveform database of all the pins in the design.  A project target indicates a vendor-specific project, including the project file, design constraints and HDL source.

To specify a target, the `set_target` keyword is used, as follows:

    :::C++
    set_target<
     type = "Simulation",
     time = "100 ms"
    >

Or, in the case of a project target:

    :::C++
    set_target<
     type     = "Project",
     vendor   = "Altera",
     series   = "Cyclone V GX",
     device   = "5CGXFC5C6F27C7N",
     board    = "Cyclone V GX Starter Kit"
     language = "Verilog"
    >

After definition, the target attributes can be accessed by means of the `target` object and used in conditional compilation, as follows:

    :::C++
    if(target.type == "Simulation"){
     // Some simulation-specific code
    }else if(target.vendor == "Altera"){
     // Some Altera-specific code
    }else if(target.vendor == "Xilinx"){
     // Some Xilinx-specific code
    }

In order to promote portability, the attribute values should be standardised within a developer community.

## Import and Name-space
Conceptually, ALCHA processes the source as a whole. It is convenient, however, to break the source into a hierarchical structure of files, as presented in the figure below.  In the context of ALCHA, a "module" refers to a source file, rather than an HDL module. An ALCHA module might or might not compile to a corresponding HDL module, depending on the source contents.

<center markdown>![Programming Model](https://sourceforge.net/p/alcha/code/ci/master/tree/Wiki/Figures/ProgrammingModel.svg?format=raw)</center>

The import statement is used to import a child module into a parent module. There are two forms, as shown below.  The first form imports the contents of "UART.alc" into the same name-space as the parent module, as if the file contents appeared in place of the `import` statement.  The second form imports the contents of "Ethernet.alc" into the "Eth" name-space.  It is illegal to import two modules into the same name within the same module.  In both forms, the imported modules have direct access to objects in the parent module name-space.

    :::Python
    import "UART"
    import "Ethernet" as Eth

This name-space model allows the developer to define an environment in a parent module. This environment is visible to all child modules, without the need of ports or parameter passing. The target and pin definitions are examples of where this is useful.  As another example, the developer might, for instance, define a `Round` function, which is used by child modules whenever signals must be rounded. The rounding behaviour of the design can then be changed by simply modifying the behaviour of this `Round` function.

Specifying the class instance, or name-space, can become tedious.  Instead, the developer can temporarily push a name-space onto the scope stack by means of a special syntax, as shown below. The `A.{` construct pushes the `A` name-space onto the stack, whereas the `B.{` construct pushes the `B` name-space. The closing curly braces pop the name-spaces from the stack.

    :::C++
    class S1{sig'8 r, g, b, a;}
    class S2{sig'8 x, y, z, w;}
    S1 A;
    S2 B;

    // Instead of saying:
    A.r:A.g:A.b:A.a = B.x:B.y:B.z:B.w + 1234;

    // Say:
    A.{
     B.{
      r:g:b:a := x:y:z:w + 1234;
     }
    }

[[include repo=code path=Wiki/MarkDown/Footer.md]]

