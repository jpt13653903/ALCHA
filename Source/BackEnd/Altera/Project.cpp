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

#include "Project.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace ALTERA;
using namespace OBJECTS;
//------------------------------------------------------------------------------

PROJECT::PROJECT(){
}
//------------------------------------------------------------------------------

PROJECT::~PROJECT(){
}
//------------------------------------------------------------------------------

bool PROJECT::WriteFile(string& Filename, const char* Ext, string& Body){
  FILE_WRAPPER Files;
  string Fullname = Path + "/" + Filename + "." + Ext;
  return Files.WriteAll(Fullname.c_str(), (const byte*)Body.c_str());
}
//------------------------------------------------------------------------------

void PROJECT::BuildFileList(string& Body, MODULE* Module, string Path){
  bool isGlobal = (Module == &Global);

  for(auto SymbolIterator  = Module->Symbols.begin();
           SymbolIterator != Module->Symbols.end  ();
           SymbolIterator++){
    auto Symbol = SymbolIterator->second;
    if(Symbol->Type == BASE::TYPE::Module){
      auto Child = (MODULE*)Symbol;
      if(isGlobal) BuildFileList(Body, Child, "source");
      else         BuildFileList(Body, Child, Path + "/" + Module->Name);
    }
  }
  if(isGlobal){
    Body += "set_global_assignment -name VERILOG_FILE \""+ Filename +".v\"\n";
  }else{
    Body += "set_global_assignment -name VERILOG_FILE \"" +
            Path + "/" + Module->Name +".v\"\n";
  }
}
//------------------------------------------------------------------------------

bool PROJECT::BuildPins(string& Body){
  for(auto SymbolIterator  = Global.Symbols.begin();
           SymbolIterator != Global.Symbols.end  ();
           SymbolIterator++){
    if(SymbolIterator->second->Type == BASE::TYPE::Pin){
      auto Pin = (PIN*)(SymbolIterator->second);
      auto Location = Pin->GetAttrib("location");
      if(!Location){
        warning("Pin without location attribute");
        continue;
      }
      if(Pin->Width == 1){
        if(Location->ExpressionType != EXPRESSION::String){
          error("Scalar pin location not a string");
          return false;
        }
        Body += "set_location_assignment PIN_"+ Location->StrValue +
                " -to "+ Pin->Name +"\n";
      }else{
        if(Location->ExpressionType != EXPRESSION::Array){
          error("Vector pin location not an array");
          return false;
        }
        if(Location->Elements.size() != (size_t)Pin->Width){
          error("Vector pin location array of wrong size");
          return false;
        }
        for(int n = 0; n < Pin->Width; n++){
          if(Location->Elements[n]->ExpressionType != EXPRESSION::String){
            error("Pin location not a string");
            return false;
          }
          Body += "set_location_assignment PIN_"+ Location->Elements[n]->StrValue +
                  " -to "+ Pin->Name +"["+ to_string(Pin->Width-1-n) +"]\n";
        }
      }
      auto Standard = Pin->GetAttrib("standard");
      if(Standard){
        if(Standard->ExpressionType != EXPRESSION::String){
          error("Standard attribute not a string");
          return false;
        }
        Body += "set_instance_assignment -name "
                "IO_STANDARD \""+ Standard->StrValue +"\" -to "+ Pin->Name;
        if(Pin->Width > 1) Body += "*";
        Body += "\n";
      }
      auto Current = Pin->GetAttrib("current");
      if(Current){
        Body += "set_instance_assignment "
                "-name CURRENT_STRENGTH_NEW ";
        switch(Current->ExpressionType){
          case EXPRESSION::Literal:{
            if(!Current->Value.IsReal()){
              error("Current attribute not real");
              return false;
            }
            NUMBER mA = Current->Value;
            mA.Mul(1e3);
            Body += to_string((int)mA.GetReal()) + "MA";
            break;
          }
          case EXPRESSION::String:
            Body += '"' + Current->StrValue + '"';
            break;
          default:
            error("Unexpected current strength attribute type");
            break;
        }
        Body += " -to "+ Pin->Name;
        if(Pin->Width > 1) Body += "*";
        Body += "\n";
      }
    }
  }
  return true;
}
//------------------------------------------------------------------------------

bool PROJECT::BuildProject(){
  string Body;

  Body +=
    "#-------------------------------------------------------------------------------\n"
    "#\n"
    "# Generated by ALCHA version " +
     to_string(MAJOR_VERSION) +"."+ to_string(MINOR_VERSION) +
    ", build \"" __DATE__ ", " __TIME__ "\"\n"
    "#-------------------------------------------------------------------------------\n"
    "#\n"
    "# Quartus II 64-Bit\n"
    "# Version 16.1\n"
    "# Date created = "+ Time +"\n"
    "#-------------------------------------------------------------------------------\n"
    "\n"
    "QUARTUS_VERSION  = \"16.1\"\n"
    "DATE             = \""+ Time +"\"\n"
    "PROJECT_REVISION = \""+ Filename +"\"\n";

  return WriteFile(Filename, "qpf", Body);
}
//------------------------------------------------------------------------------

bool PROJECT::BuildSettings(){
  string Body;

  Body += // Header
    "#-------------------------------------------------------------------------------\n"
    "#\n"
    "# Generated by ALCHA version " +
     to_string(MAJOR_VERSION) +"."+ to_string(MINOR_VERSION) +
    ", build \"" __DATE__ ", " __TIME__ "\"\n"
    "#-------------------------------------------------------------------------------\n"
    "#\n"
    "# Quartus II 64-Bit\n"
    "# Version 16.1\n"
    "# Date created = "+ Time +"\n"
    "#-------------------------------------------------------------------------------\n"
    "\n";

  Body += // General settings
    "set_global_assignment -name FAMILY \""+ Series +"\"\n"
    "set_global_assignment -name DEVICE "+ Device +"\n"
    "set_global_assignment -name TOP_LEVEL_ENTITY "+ Filename +"\n"
    "set_global_assignment -name ORIGINAL_QUARTUS_VERSION 16.1.0\n"
    "set_global_assignment -name PROJECT_CREATION_TIME_DATE \""+ Time +"\"\n"
    "set_global_assignment -name LAST_QUARTUS_VERSION 16.1.0\n"
    "set_global_assignment -name PROJECT_OUTPUT_DIRECTORY output_files\n"
    "set_global_assignment -name MIN_CORE_JUNCTION_TEMP 0\n"
    "set_global_assignment -name MAX_CORE_JUNCTION_TEMP 85\n"
    "set_global_assignment -name ERROR_CHECK_FREQUENCY_DIVISOR 256\n"
    "set_global_assignment -name PARTITION_NETLIST_TYPE SOURCE -section_id Top\n"
    "set_global_assignment -name PARTITION_FITTER_PRESERVATION_LEVEL PLACEMENT_AND_ROUTING -section_id Top\n"
    "set_global_assignment -name PARTITION_COLOR 16764057 -section_id Top\n"
    "set_global_assignment -name STRATIX_DEVICE_IO_STANDARD \"3.3-V LVCMOS\"\n"
    "set_global_assignment -name POWER_PRESET_COOLING_SOLUTION \"23 MM HEAT SINK WITH 200 LFPM AIRFLOW\"\n"
    "set_global_assignment -name POWER_BOARD_THERMAL_MODEL \"NONE (CONSERVATIVE)\"\n"
    "set_global_assignment -name ENABLE_OCT_DONE OFF\n"
    "set_global_assignment -name EXTERNAL_FLASH_FALLBACK_ADDRESS 00000000\n"
    "set_global_assignment -name USE_CONFIGURATION_DEVICE OFF\n"
    "set_global_assignment -name CRC_ERROR_OPEN_DRAIN OFF\n"
    "set_global_assignment -name OUTPUT_IO_TIMING_NEAR_END_VMEAS \"HALF VCCIO\" -rise\n"
    "set_global_assignment -name OUTPUT_IO_TIMING_NEAR_END_VMEAS \"HALF VCCIO\" -fall\n"
    "set_global_assignment -name OUTPUT_IO_TIMING_FAR_END_VMEAS \"HALF SIGNAL SWING\" -rise\n"
    "set_global_assignment -name OUTPUT_IO_TIMING_FAR_END_VMEAS \"HALF SIGNAL SWING\" -fall\n"
    "set_global_assignment -name VERILOG_INPUT_VERSION SYSTEMVERILOG_2005\n"
    "set_global_assignment -name VERILOG_SHOW_LMF_MAPPING_MESSAGES OFF\n"
    "set_global_assignment -name INTERNAL_FLASH_UPDATE_MODE \"SINGLE COMP IMAGE\"\n"
    "#-------------------------------------------------------------------------------\n"
    "\n";

  if(!BuildPins(Body)) return false;
  Body +=
    "#-------------------------------------------------------------------------------\n"
    "\n";

  // Body += // TODO - Location attributes
    // "set_location_assignment PLL_1 -to \"altpll:PLL\"\n"

  // Body += // TODO - HDL construct dependency list
    //"set_global_assignment -name QIP_FILE Qsys/ADC/synthesis/ADC.qip\n"

  // File list
  BuildFileList(Body, &Global, "");

  Body += // Tail end
    "\n"
    "set_global_assignment -name SDC_FILE "+ Filename +".sdc\n"
    "set_global_assignment -name CDF_FILE "+ Filename +".cdf\n"
    "#-------------------------------------------------------------------------------\n"
    "\n"
    "set_instance_assignment -name PARTITION_HIERARCHY root_partition -to | -section_id Top\n";

  return WriteFile(Filename, "qsf", Body);
}
//------------------------------------------------------------------------------

bool PROJECT::BuildDesignConstraints(){
  SDC Constraints;

  return WriteFile(Filename, "sdc", Constraints.Build());
}
//------------------------------------------------------------------------------

bool PROJECT::BuildConfigChain(){
  string Body;

  Body =
    "JedecChain;\n"
    "\tFileRevision(JESD32A);\n"
    "\tDefaultMfr(6E);\n"
    "\n"
    "\tP ActionCode(Cfg)\n"
    "\t\tDevice PartName("+ Device +") "
    "Path(\"output_files/\") " "File(\""+ Filename +".sof\") " "MfrSpec(OpMask(1));\n"
    "\n"
    "ChainEnd;\n"
    "\n"
    "AlteraBegin;\n"
    "\tChainType(JTAG);\n"
    "AlteraEnd\n";

  return WriteFile(Filename, "cdf", Body);
}
//------------------------------------------------------------------------------

bool PROJECT::Build(const char* Path, const char* Filename){
  this->Path     = Path;
  this->Filename = Filename;

  char   Time[0x100];
  time_t RawTime;
  time(&RawTime);
  strftime(Time, 0x100, "%H:%M:%S  %B %d, %Y", localtime(&RawTime));
  this->Time = Time;

  auto Device = Global.GetAttrib("target_device");
  if(!Device){
    error("Global attribute \"target_device\" not defined");
    return false;
  }
  if(Device->ExpressionType != EXPRESSION::String){
    error("Global attribute \"target_device\" not a string");
    return false;
  }
  this->Device = Device->StrValue;

  auto Series = Global.GetAttrib("target_series");
  if(!Series){
    error("Global attribute \"target_series\" not defined");
    return false;
  }
  if(Series->ExpressionType != EXPRESSION::String){
    error("Global attribute \"target_series\" not a string");
    return false;
  }
  this->Series = Series->StrValue;

  if(!BuildProject          ()) return false;
  if(!BuildSettings         ()) return false;
  if(!BuildDesignConstraints()) return false;
  if(!BuildConfigChain      ()) return false;

  return true;
}
//------------------------------------------------------------------------------

