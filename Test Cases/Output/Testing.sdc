# Define Clocks
create_clock -name     "Clk_50_Meg" \
             -period   20ns         \
             -waveform {0ns 10ns}   \
             [get_ports Clk]

derive_pll_clocks -create_base_clocks -use_net_name
#-------------------------------------------------------------------------------

# Calculate Clock Uncertainties
derive_clock_uncertainty
#-------------------------------------------------------------------------------

# Ignore Timing Path
set_false_path -from [get_ports     *] \
               -to   [get_registers *]

set_false_path -from [get_registers *] \
               -to   [get_ports     *]

set_false_path -from [get_ports *] \
               -to   [get_ports *]
#-------------------------------------------------------------------------------

 