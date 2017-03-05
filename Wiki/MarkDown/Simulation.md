[[include repo=code path=Wiki/MarkDown/Header.md]]

[TOC]

# Simulation and Verification

ALCHA supports both simulation and hardware testing.

## Simulation

When compiling to a simulation target, the developer must specify a test-bench. ALCHA automatically generates waveforms for clock pins and derived clock nets.  The developer can generate other waveforms by using the `fsm` construct, as illustrated below:

~~~C++
// Generating a test waveform of a bouncing button
pin<frequency = "1 kHz"> ButtonClk;
pin Button = 1;

fsm(ButtonClk){
 loop(100); // Wait 100 ms
 Button = 0,
 loop(floor(rand()*5+3)){ // 3 to 8 bounces
  loop(floor(rand()*5+1)); // 1 to 5 ms delay
  Button = 1,
  loop(floor(rand()*5+1)); // 1 to 5 ms delay
  Button = 0;
 } // End of waveform
}
~~~

During simulation, the ALCHA compiler writes all pins to a wave-form database that can be viewed with a GUI or analysed by other means. The simulation only supports cycle-accurate behavioural simulation. Timing-accurate simulation can be achieved by simulating the output of a project target with existing vendor tools.

## Finite State Machines

When simulating ALCHA-based state machines, the code is not converted to an RTL intermediate format.  The sequential nature of the state machine is maintained, which can potentially simulate much faster than the RTL equivalent.

## Data Injection and Logging

During simulation, it is often useful to inject data from a file, and then store the result in a log file.

ALCHA scripting can be used to populate an instance of ROM, which is injected into the circuit by means of a simple state machine.  The standard simulation output log file can be post-processed in order to extract the required information.

If more sophisticated functionality is required, the design can be compiled to Verilog and analysed by means of more powerful tools.

## Hardware Testing

The waveform generator above is hardware synthesisable. The same test-bench can therefore be used to test the system on real hardware, optionally using a firmware-based logic analyser.

[[include repo=code path=Wiki/MarkDown/Footer.md]]

