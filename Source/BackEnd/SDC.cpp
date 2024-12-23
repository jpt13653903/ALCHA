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
#include "Netlist/Synthesisable/pin.h"
//------------------------------------------------------------------------------

using std::string;
using std::to_string;
using namespace Netlist;
//------------------------------------------------------------------------------

SDC::SDC(){}
//------------------------------------------------------------------------------

SDC::~SDC(){}
//------------------------------------------------------------------------------

void SDC::buildClocks()
{
    int clockCount = 0;

    for(auto symbolIterator: global.symbols){
        if(symbolIterator.second->type == Base::Type::Pin){
            auto pin  = (Pin*)(symbolIterator.second);
            auto freq = pin->getAttribValue("frequency");
            if(freq){
                if(freq->type != AST::Base::Type::Literal){
                    error("frequency attribute not a literal");
                    return;
                }
                if(!((AST::Literal*)freq)->value.isReal()){
                    error("frequency attribute not real");
                    return;
                }
                double period = 1e9/((AST::Literal*)freq)->value.getReal(); // ns
                constraints +=
                    "create_clock -name {" + pin->hdlName()     + "}"   +
                    " -period "            + to_string(period)   + "ns"  +
                    " -waveform {0ns "     + to_string(period/2) + "ns}" +
                    " [get_ports {"        + pin->hdlName()     + "}]\n";
                clockCount++;
            }
        }
        // No recursion required -- all pins are top-level at this point
    }

    // Create clock groups
    if(clockCount){
        constraints += "\nset_clock_groups -asynchronous";

        for(auto symbolIterator: global.symbols){
            if(symbolIterator.second->type == Base::Type::Pin){
                auto pin  = (Pin*)(symbolIterator.second);
                auto freq = pin->getAttribValue("frequency");
                // Already checked for validity above, so no need to check again
                if(freq) constraints += " \\\n  -group {"+ pin->hdlName() +"}";
            }
        }
        constraints += "\n";
    }
}
//------------------------------------------------------------------------------

void SDC::buildPorts(Netlist::NameSpace* nameSpace)
{
    // TODO: Handle the input and output delays properly
    warning("input_delay and output_delay not yet implemented");

    for(auto symbolIterator: nameSpace->symbols){
        switch(symbolIterator.second->type){
            case Base::Type::Pin:{
                auto pin = (Pin*)(symbolIterator.second);
                if(!pin->getAttribValue("frequency")){ // Not a clock
                    if(pin->direction != AST::Definition::Direction::Output){ // Input or bidirectional
                        constraints += "set_false_path -to   * -from ";
                        constraints += "[get_ports {" + pin->hdlName();
                        if(pin->width() > 1) constraints += "[*]";
                        constraints += "} ]\n";
                    }
                    if(pin->direction != AST::Definition::Direction::Input){ // Output or bidirectional
                        constraints += "set_false_path -from * -to   ";
                        constraints += "[get_ports {" + pin->hdlName();
                        if(pin->width() > 1) constraints += "[*]";
                        constraints += "} ]\n";
                    }
                }
                break;
            }
            case Base::Type::Group:{
                buildPorts((NameSpace*)symbolIterator.second);
                break;
            }
            default:
                break;
        }
        // No recursion required -- all pins are top-level at this point
    }
}
//------------------------------------------------------------------------------

string& SDC::build()
{
    constraints.clear();

    constraints = "# Define Clocks\n";
    buildClocks();
    constraints +=
        "\n"
        "derive_pll_clocks -create_base_clocks -use_net_name\n"
        "#-------------------------------------------------------------------------------\n"
        "\n"
        "# Calculate Clock Uncertainties\n"
        "derive_clock_uncertainty\n"
        "#-------------------------------------------------------------------------------\n"
        "\n";
    buildPorts(&global);
    constraints +=
        "#-------------------------------------------------------------------------------\n";

    return constraints;
}
//------------------------------------------------------------------------------

