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

#include "SDC.h"
#include "Netlist/Synthesisable/Pin.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace NETLIST;
//------------------------------------------------------------------------------

SDC::SDC(){
}
//------------------------------------------------------------------------------

SDC::~SDC(){
}
//------------------------------------------------------------------------------

void SDC::BuildClocks(){
  int ClockCount = 0;

  foreach(SymbolIterator, Global.Symbols){
    if(SymbolIterator->second->Type == BASE::TYPE::Pin){
      auto Pin  = (PIN*)(SymbolIterator->second);
      auto Freq = Pin->GetAttribValue("frequency");
      if(Freq){
        if(Freq->Type != AST::BASE::TYPE::Literal){
          error("frequency attribute not a literal");
          return;
        }
        if(!((AST::LITERAL*)Freq)->Value.IsReal()){
          error("frequency attribute not real");
          return;
        }
        double Period = 1e9/((AST::LITERAL*)Freq)->Value.GetReal(); // ns
        Constraints +=
          "create_clock -name {" + Pin->HDL_Name()     + "}"   +
          " -period "            + to_string(Period)   + "ns"  +
          " -waveform {0ns "     + to_string(Period/2) + "ns}" +
          " [get_ports {"        + Pin->HDL_Name()     + "}]\n";
        ClockCount++;
      }
    }
    // No recursion required -- all pins are top-level at this point
  }

  // Create clock groups
  if(ClockCount){
    Constraints += "\nset_clock_groups -asynchronous";

    foreach(SymbolIterator, Global.Symbols){
      if(SymbolIterator->second->Type == BASE::TYPE::Pin){
        auto Pin  = (PIN*)(SymbolIterator->second);
        auto Freq = Pin->GetAttribValue("frequency");
        // Already checked for validity above, so no need to check again
        if(Freq) Constraints += " \\\n  -group {"+ Pin->HDL_Name() +"}";
      }
    }
    Constraints += "\n";
  }
}
//------------------------------------------------------------------------------

void SDC::BuildPorts(NETLIST::NAMESPACE* Namespace){
  // TODO: Handle the input and output delays properly
  warning("input_delay and output_delay not yet implemented");

  foreach(SymbolIterator, Namespace->Symbols){
    switch(SymbolIterator->second->Type){
      case BASE::TYPE::Pin:{
        auto Pin = (PIN*)(SymbolIterator->second);
        if(!Pin->GetAttribValue("frequency")){ // Not a clock
          if(Pin->Direction != AST::DEFINITION::DIRECTION::Output){ // Input or bidirectional
            Constraints += "set_false_path -to   * -from ";
            Constraints += "[get_ports {" + Pin->HDL_Name();
            if(Pin->Width() > 1) Constraints += "[*]";
            Constraints += "} ]\n";
          }
          if(Pin->Direction != AST::DEFINITION::DIRECTION::Input){ // Output or bidirectional
            Constraints += "set_false_path -from * -to   ";
            Constraints += "[get_ports {" + Pin->HDL_Name();
            if(Pin->Width() > 1) Constraints += "[*]";
            Constraints += "} ]\n";
          }
        }
        break;
      }
      case BASE::TYPE::Group:{
        BuildPorts((NAMESPACE*)SymbolIterator->second);
        break;
      }
      default:
        break;
    }
    // No recursion required -- all pins are top-level at this point
  }
}
//------------------------------------------------------------------------------

string& SDC::Build(){
  Constraints.clear();

  Constraints = "# Define Clocks\n";
  BuildClocks();
  Constraints +=
    "\n"
    "derive_pll_clocks -create_base_clocks -use_net_name\n"
    "#-------------------------------------------------------------------------------\n"
    "\n"
    "# Calculate Clock Uncertainties\n"
    "derive_clock_uncertainty\n"
    "#-------------------------------------------------------------------------------\n"
    "\n";
  BuildPorts(&Global);
  Constraints +=
    "#-------------------------------------------------------------------------------\n";

  return Constraints;
}
//------------------------------------------------------------------------------

