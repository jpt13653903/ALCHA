//==============================================================================
// Copyright (C) John-Philip Taylor
// jpt13653903@gmail.com
//
// This file is part of ALCHA
//
// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
//==============================================================================

#include "Altera.h"
//------------------------------------------------------------------------------

bool ALTERA::WriteProject(){
 FILE_SYSTEM fs;

 time_t rawtime; time(&rawtime);
 tm* time = localtime(&rawtime);

 const char* Month[12] = {
  "January",
  "February",
  "March",
  "April",
  "May",
  "June",
  "July",
  "August",
  "September",
  "October",
  "November",
  "December",
 };

 STRING QPF;
 QPF << "\
QUARTUS_VERSION = \"15.0\"\n\
DATE = \"" <<
time->tm_hour << ":" <<
time->tm_min  << ":" <<
time->tm_sec  << "  " <<
Month[time->tm_mon] << " " <<
time->tm_mday << ", " <<
time->tm_year + 1900 <<
"\"\n\
\n\
# Revisions\n\
\n\
PROJECT_REVISION = \"" << hdl.Toplevel->Name << "\"\n\
 ";

 STRING QSF;
 QSF << "\
set_global_assignment -name FAMILY \"" << Target.Series << "\"\n\
set_global_assignment -name DEVICE " << Target.Device << "\n\
set_global_assignment -name TOP_LEVEL_ENTITY " << hdl.Toplevel->Name << "\n\
set_global_assignment -name ORIGINAL_QUARTUS_VERSION 15.0.0\n\
set_global_assignment -name PROJECT_CREATION_TIME_DATE \"" <<
time->tm_hour << ":" <<
time->tm_min  << ":" <<
time->tm_sec  << "  " <<
Month[time->tm_mon] << " " <<
time->tm_mday << ", " <<
time->tm_year + 1900 <<
"\"\n\
set_global_assignment -name LAST_QUARTUS_VERSION 15.0.0\n\
set_global_assignment -name PROJECT_OUTPUT_DIRECTORY output_files\n\
set_global_assignment -name MIN_CORE_JUNCTION_TEMP 0\n\
set_global_assignment -name MAX_CORE_JUNCTION_TEMP 85\n\
set_global_assignment -name ERROR_CHECK_FREQUENCY_DIVISOR 256\n\
set_global_assignment -name PARTITION_NETLIST_TYPE SOURCE -section_id Top\n\
set_global_assignment -name PARTITION_FITTER_PRESERVATION_LEVEL PLACEMENT_AND_ROUTING -section_id Top\n\
set_global_assignment -name PARTITION_COLOR 16764057 -section_id Top\n\
set_instance_assignment -name PARTITION_HIERARCHY root_partition -to | -section_id Top\n\
set_global_assignment -name STRATIX_DEVICE_IO_STANDARD \"3.3-V LVCMOS\"\n\
set_global_assignment -name RESERVE_ALL_UNUSED_PINS_WEAK_PULLUP \"AS INPUT TRI-STATED WITH WEAK PULL-UP\"\n\
set_global_assignment -name POWER_PRESET_COOLING_SOLUTION \"23 MM HEAT SINK WITH 200 LFPM AIRFLOW\"\n\
set_global_assignment -name POWER_BOARD_THERMAL_MODEL \"NONE (CONSERVATIVE)\"\n\
set_global_assignment -name ENABLE_OCT_DONE OFF\n\
set_global_assignment -name EXTERNAL_FLASH_FALLBACK_ADDRESS 00000000\n\
set_global_assignment -name USE_CONFIGURATION_DEVICE OFF\n\
set_global_assignment -name CRC_ERROR_OPEN_DRAIN OFF\n\
set_global_assignment -name OUTPUT_IO_TIMING_NEAR_END_VMEAS \"HALF VCCIO\" -rise\n\
set_global_assignment -name OUTPUT_IO_TIMING_NEAR_END_VMEAS \"HALF VCCIO\" -fall\n\
set_global_assignment -name OUTPUT_IO_TIMING_FAR_END_VMEAS \"HALF SIGNAL SWING\" -rise\n\
set_global_assignment -name OUTPUT_IO_TIMING_FAR_END_VMEAS \"HALF SIGNAL SWING\" -fall\n\
set_global_assignment -name VERILOG_INPUT_VERSION SYSTEMVERILOG_2005\n\
set_global_assignment -name VERILOG_SHOW_LMF_MAPPING_MESSAGES OFF\n\
set_global_assignment -name INTERNAL_FLASH_UPDATE_MODE \"SINGLE COMP IMAGE\"\n\
#-------------------------------------------------------------------------------\n\
\n\
set_location_assignment PIN_AB5 -to Button[4]\n\
set_location_assignment PIN_V5 -to Button[3]\n\
set_location_assignment PIN_R1 -to Button[2]\n\
set_location_assignment PIN_M1 -to Button[1]\n\
set_location_assignment PIN_N14 -to Clk\n\
set_location_assignment PIN_AA5 -to LED[8]\n\
set_location_assignment PIN_AB4 -to LED[7]\n\
set_location_assignment PIN_T6 -to LED[6]\n\
set_location_assignment PIN_V4 -to LED[5]\n\
set_location_assignment PIN_T1 -to LED[4]\n\
set_location_assignment PIN_R2 -to LED[3]\n\
set_location_assignment PIN_N1 -to LED[2]\n\
set_location_assignment PIN_M2 -to LED[1]\n\
set_instance_assignment -name IO_STANDARD \"3.3-V LVTTL\" -to LED[1]\n\
set_instance_assignment -name IO_STANDARD \"3.3-V LVTTL\" -to LED[8]\n\
set_instance_assignment -name IO_STANDARD \"3.3-V LVTTL\" -to LED[7]\n\
set_instance_assignment -name IO_STANDARD \"3.3-V LVTTL\" -to LED[6]\n\
set_instance_assignment -name IO_STANDARD \"3.3-V LVTTL\" -to LED[5]\n\
set_instance_assignment -name IO_STANDARD \"3.3-V LVTTL\" -to LED[4]\n\
set_instance_assignment -name IO_STANDARD \"3.3-V LVTTL\" -to LED[3]\n\
set_instance_assignment -name IO_STANDARD \"3.3-V LVTTL\" -to LED[2]\n\
set_instance_assignment -name CURRENT_STRENGTH_NEW \"8mA\" -to LED[8]\n\
set_instance_assignment -name CURRENT_STRENGTH_NEW \"8mA\" -to LED[7]\n\
set_instance_assignment -name CURRENT_STRENGTH_NEW \"8mA\" -to LED[6]\n\
set_instance_assignment -name CURRENT_STRENGTH_NEW \"8mA\" -to LED[5]\n\
set_instance_assignment -name CURRENT_STRENGTH_NEW \"8mA\" -to LED[4]\n\
set_instance_assignment -name CURRENT_STRENGTH_NEW \"8mA\" -to LED[3]\n\
set_instance_assignment -name CURRENT_STRENGTH_NEW \"8mA\" -to LED[2]\n\
set_instance_assignment -name CURRENT_STRENGTH_NEW \"8mA\" -to LED[1]\n\
\n\
set_instance_assignment -name IO_STANDARD LVDS -to LVDS_TX_P[0]\n\
set_location_assignment PIN_V17 -to LVDS_TX_P[0]\n\
set_location_assignment PIN_W17 -to LVDS_TX_P[0](n)\n\
#-------------------------------------------------------------------------------\n\
\n\
set_global_assignment -name SDC_FILE " << hdl.Toplevel->Name << ".sdc\n\
set_global_assignment -name CDF_FILE " << hdl.Toplevel->Name << ".cdf\n\
set_global_assignment -name VERILOG_FILE " << hdl.Toplevel->Name << ".v\n\
#-------------------------------------------------------------------------------\n\
 ";

 STRING SDC;
 SDC << "\
# Define Clocks\n\
create_clock -name     \"Clk_50_Meg\" \\\n\
             -period   20ns         \\\n\
             -waveform {0ns 10ns}   \\\n\
             [get_ports Clk]\n\
\n\
derive_pll_clocks -create_base_clocks -use_net_name\n\
#-------------------------------------------------------------------------------\n\
\n\
# Calculate Clock Uncertainties\n\
derive_clock_uncertainty\n\
#-------------------------------------------------------------------------------\n\
\n\
# Ignore Timing Path\n\
set_false_path -from [get_ports     *] \\\n\
               -to   [get_registers *]\n\
\n\
set_false_path -from [get_registers *] \\\n\
               -to   [get_ports     *]\n\
\n\
set_false_path -from [get_ports *] \\\n\
               -to   [get_ports *]\n\
#-------------------------------------------------------------------------------\n\
\n\
 ";

 STRING CDF;
 CDF << "\
/* Quartus II 64-Bit Version 15.0.0 Build 145 04/22/2015 SJ Web Edition */\n\
JedecChain;\n\
	FileRevision(JESD32A);\n\
	DefaultMfr(6E);\n\
\n\
	P ActionCode(Cfg)\n\
		Device PartName(" << Target.Device << ") Path(\"output_files/\") File(\"" << hdl.Toplevel->Name << ".sof\") MfrSpec(OpMask(1));\n\
\n\
ChainEnd;\n\
\n\
AlteraBegin;\n\
	ChainType(JTAG);\n\
AlteraEnd\n\
 ";

 STRING Filename;
 Filename << hdl.Toplevel->Name << ".qpf";
 if(!fs.Write(Filename.String(), QPF.String(), QPF.Length())) return false;

 Filename.Clear();
 Filename << hdl.Toplevel->Name << ".qsf";
 if(!fs.Write(Filename.String(), QSF.String(), QSF.Length())) return false;

 Filename.Clear();
 Filename << hdl.Toplevel->Name << ".sdc";
 if(!fs.Write(Filename.String(), SDC.String(), SDC.Length())) return false;

 if(!hdl.Toplevel->Write()) return false;

 Filename.Clear();
 Filename << hdl.Toplevel->Name << ".cdf";
 if(!fs.Write(Filename.String(), CDF.String(), CDF.Length())) return false;

 return true;
}
//------------------------------------------------------------------------------
