echo off

C:\altera\15.0\quartus\bin64\qmegawiz -silent ^
 module=altpll ^
 INTENDED_DEVICE_FAMILY="MAX 10" ^
 OPERATION_MODE=NO_COMPENSATION ^
 INCLK0_INPUT_FREQUENCY=20000 ^
 CLK0_DIVIDE_BY=5 ^
 CLK1_DIVIDE_BY=7 ^
 OPTIONAL_FILES=NONE ^
 areset=unused ^
 locked=used ^
 clk0=used ^
 clk1=used ^
 MAX10PLL.v

echo Trying next one...
c:\altera\15.0\quartus\sopc_builder\bin\ip-generate ^
 --output-directory="." ^
 --file-set="QUARTUS_SYNTH" ^
 --component-name=altera_pll ^
 --output-name=CycloneVPLL ^
 --system-info=DEVICE_FAMILY="Cyclone V" ^
 --component-param=gui_use_locked=on ^
 --component-param=gui_number_of_clocks=1 ^
 --component-param=gui_reference_clock_frequency="50 MHz" ^
 --component-param=gui_output_clock_frequency0="10 MHz"

pause

