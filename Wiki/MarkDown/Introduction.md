[[include repo=code path=Wiki/MarkDown/Header.md]]

[TOC]

# Introduction
ALCHA (architectural level computational hardware abstraction) is a command-line tool to generate vendor-specific FPGA projects from largely vendor-neutral text-based source code.  The image below provides a rough overview of the intended tool-chain.

<center markdown>![Concept Block Diagram](https://sourceforge.net/p/alcha/code/ci/master/tree/Wiki/Figures/ConceptBlock.svg?format=raw)</center>

ALCHA is aimed at unifying the various aspects of FPGA firmware design (RTL design, finite state machines, timing and design constraints, as well as scripting) into a single language.

ALCHA is designed to be as portable, concise and expressive as possible, without the loss of low-level control. It provides a development platform that promises reduced development time and maintenance effort.

## Automated Optimisation
The ALCHA compiler applies various optimisations before project synthesis, most of which are related to finite state machine resource usage. The details of this is outside the scope of this wiki. One notable optimisation, however, is automated multi-cycle analysis.

Unless specified otherwise, the circuit synthesis process assumes that all register-to-register paths have to have a delay of less than one clock period. This is not true for all paths in a typical state-machine. It is likely that a particular signal path have multiple clock-cycles in which to pass through the combinational cloud, thereby requiring less stringent timing constraints than the synthesis tool initially assumes.

Manually specifying multi-cycle paths is a labour-intensive and tedious process. ALCHA performs this task automatically whenever the design is compiled, thereby generating code that is easier to synthesise, resulting in reduced compilation times.

## Implementation Time-frame

Target Date   | Feature
-----------   | -------
December 2016 | Case study of the language grammar, using [FUI Audio DAC](https://sourceforge.net/projects/fui-audio-dac/)
April 2017    | Prototype compiler with elementary simulation
June 2017     | Compilation to Altera projects
December 2017 | Compilation to Xilinx projects

## References
### Similar Projects

- [Synflow Cx](http://cx-lang.org/)
- [Migen](https://github.com/m-labs/migen)
- [MyHDL](http://www.myhdl.org/)

### Acknowledgements

Many of the ideas of ALCHA are borrowed from other existing languages.  These include, among others:

- [C++](https://isocpp.org/)
- [D](http://dlang.org/)
- [[[img src=https://sourceforge.net/p/alcha/code/ci/master/tree/Wiki/Figures/LaTeX.svg?format=raw]]](https://www.latex-project.org/)
- [MATLAB](http://www.mathworks.com/products/matlab/)
- [Python](https://www.python.org/)

### Publications

- J Taylor and S Winberg
  [ALCHA: New Object Oriented Approach to FPGA Project Development](http://www.icit2016.org/)
  Proceedings of International Conference on Industrial Technology, IEEE, March 2016, Pages&nbsp;707&nbsp;&ndash;&nbsp;712

### Bibliography
- K Chapman
  [Get your Priorities Right -- Make your Design Up to 50% Smaller](http://www.xilinx.com/support/documentation/white_papers/wp275.pdf)
  Xilinx white paper, 2007

- K Chapman
  [Get Smart About Reset: Think Local, Not Global](http://www.xilinx.com/support/documentation/white_papers/wp272.pdf)
  Xilinx white paper, 2008

- P Coussy and A Morawiec
  [High-Level Synthesis: from Algorithm to Digital Circuit](http://www.amazon.com/High-Level-Synthesis-Algorithm-Digital-Circuit/dp/9048179238)
  Springer, 2008, ISBN 978-1-4020-8587-1, e-ISBN 978-1-4020-8588-8

- J DeLaere and S Zammattio
  [Top 7 Reasons to Replace Your Microcontroller with a MAX 10 FPGA](https://www.altera.com/content/dam/altera-www/global/en_US/pdfs/literature/wp/wp-01255-top-7-reasons-to-replace-your-microcontroller-with-a-max-10-fpga.pdf)
  Altera, June 2015

- A Feller, R Noto and A M Smith
  [Standard Cell Approach for Generating Custom CMOS/SOS Devices Using a Fully Automatic Layout Program](http://dx.doi.org/10.1109/MCAS.1981.6323756)
  IEEE Circuits and Systems Magazine, September 1981, Volume 3, Number 3, Pages&nbsp;9&nbsp;&ndash;&nbsp;13

- L Fousse, G Hanrot, V Lef&#232;vre, P P&#233;lissier and P Zimmermann
  [MPFR: A Multiple-precision Binary Floating-point Library with Correct Rounding](http://doi.acm.org/10.1145/1236463.1236468)
  ACM Transactions on Mathematical Software (TOMS), June 2007, Volume 33, Number 2

- M Frigo and S G Johnson
  [The Design and Implementation of FFTW3](http://dx.doi.org/10.1109/JPROC.2004.840301)
  Proceedings of the IEEE, February 2005, Volume 93, Number 2, Pages&nbsp;216&nbsp;&ndash;&nbsp;231

- T Granlund and the GMP development team
  [GNU MP: The GNU Multiple Precision Arithmetic Library](http://gmplib.org/)
  2015, Version 6.1.0

- P Luksch, U Maier, S Rathmayer, M Weidmann, F Unger, P Bastian, V Reichenberger and A Haas
  [Software Engineering in Parallel and Distributed Scientific Computing: a Case Study from Industrial Practice](http://dx.doi.org/10.1109/PDSE.1998.668179)
 Proceedings of International Symposium on Software Engineering for Parallel and Distributed Systems, IEEE, April 1998, Pages&nbsp;187&nbsp;&ndash;&nbsp;1997

- R Nane, V M Sima, C Pilato and J Choi
  [A Survey and Evaluation of FPGA High-Level Synthesis Tools](http://dx.doi.org/10.1109/TCAD.2015.2513673)
  IEEE Transactions on Computer-Aided Design of Integrated Circuits and Systems, December 2015

- R R Seban
  [An Overview of Object-Oriented Design and C++](http://dx.doi.org/10.1109/AERO.1994.291202)
  Proceedings of Aerospace Applications Conference, IEEE, February 1994, Pages&nbsp;65&nbsp;&ndash;&nbsp;86

- J Stephenson
  [Design Guidelines for Optimal Results in FPGAs](http://notes-application.abcelectronique.com/038/38-21414.pdf)
  Altera Corporation, 2005

- IEEE 1800-2012
  [IEEE Standard for SystemVerilog &ndash; Unified Hardware Design, Specification, and Verification Language](http://dx.doi.org/10.1109/IEEESTD.2013.6469140)
  IEEE, February 2013

- T van Court and M C Herbordt
  [Requirements for any HPC/FPGA Application Development Tool Flow](http://dx.doi.org/10.1155/ASP/2006/97950)
  Proceedings of the fourth Annual Boston Area Computer Architecture Workshop, 2006

- H Zheng, S T Gurumani, L Yang, D Chen and K Rupnow
  [High-Level Synthesis with Behavioral-Level Multicycle Path Analysis](http://dx.doi.org/10.1109/TCAD.2014.2361661)
  Transactions on Computer-Aided Design of Integrated Circuits and Systems, IEEE, December 2014, Volume 33, Number 12, Pages&nbsp;1832&nbsp;&ndash;&nbsp;1845

[[include repo=code path=Wiki/MarkDown/Footer.md]]

