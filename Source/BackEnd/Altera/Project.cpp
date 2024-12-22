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
#include "Netlist/Synthesisable/Pin.h"
//------------------------------------------------------------------------------

using std::string;
using std::to_string;
using namespace ALTERA;
using namespace NETLIST;
//------------------------------------------------------------------------------

PROJECT::PROJECT()
{
}
//------------------------------------------------------------------------------

PROJECT::~PROJECT()
{
}
//------------------------------------------------------------------------------

void PROJECT::Error(const char* Message)
{
    ::Error(0, "", Message);
}
//------------------------------------------------------------------------------

void PROJECT::Warning(const char* Message)
{
    ::Warning(0, "", Message);
}
//------------------------------------------------------------------------------

bool PROJECT::WriteFile(string& Filename, const char* Ext, string& Body)
{
    FileWrapper Files;
    string Fullname = Path + "/" + Filename + "." + Ext;
    return Files.writeAll(Fullname.c_str(), (const byte*)Body.c_str());
}
//------------------------------------------------------------------------------

void PROJECT::BuildFileList(string& Body, MODULE* Module, string Path)
{
    bool isGlobal = (Module == &Global);

    foreach(SymbolIterator, Module->Symbols){
        auto Symbol = SymbolIterator->second;
        if(Symbol->Type == BASE::TYPE::Module){
            auto Child = (MODULE*)Symbol;
            if(isGlobal) BuildFileList(Body, Child, "source");
            else         BuildFileList(Body, Child, Path + "/" + Module->HDL_Name());
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

void PROJECT::AssignPin(string& Body, const string& Location, const string& Name)
{
    string P, N;
    bool Diff = false;

    for(size_t n = 0; n < Location.length(); n++){
        switch(Location[n]){
            case ' ':
            case '\t':
                break;
            case '-':
                Diff = true;
                break;
            default:
                if(Diff) N += Location[n];
                else     P += Location[n];
                break;
        }
    }
    Body += "set_location_assignment PIN_"+ P + " -to "+ Name +"\n";
    if(Diff){
        Body += "set_location_assignment PIN_"+ N + " -to "+ Name +"(n)\n";
    }
}
//------------------------------------------------------------------------------

bool PROJECT::BuildPins(string& Body, NAMESPACE* Namespace)
{
    foreach(SymbolIterator, Namespace->Symbols){
        switch(SymbolIterator->second->Type){
            case BASE::TYPE::Pin:{
                auto Pin = (PIN*)(SymbolIterator->second);
                auto Standard = Pin->GetAttribValue("standard");
                if(Standard){
                    if(Standard->Type != AST::BASE::TYPE::String){
                        Standard->Error("Standard attribute not a string");
                        return false;
                    }
                    Body += "set_instance_assignment -name "
                                    "IO_STANDARD \""+ ((AST::STRING*)Standard)->Value +"\" -to "+ Pin->HDL_Name();
                    if(Pin->Width() > 1) Body += "[*]";
                    Body += "\n";
                }
                auto Location = Pin->GetAttribValue("location");
                if(Location){
                    if(Pin->Width() == 1){
                        if(Location->Type != AST::BASE::TYPE::String){
                            Location->Error("Scalar pin location not a string");
                            return false;
                        }
                        AssignPin(Body, ((AST::STRING*)Location)->Value, Pin->HDL_Name());
                    }else{
                        if(Location->Type != AST::BASE::TYPE::Array){
                            Location->Error("Vector pin location not an array");
                            return false;
                        }
                        auto LocationArray = (AST::ARRAY*)Location;
                        if(LocationArray->Elements.size() != (size_t)Pin->Width()){
                            LocationArray->Error("Vector pin location array of wrong size");
                            return false;
                        }
                        for(int n = 0; n < Pin->Width(); n++){
                            AST::BASE* Temp = LocationArray->Elements[n];
                            assert(Temp && Temp->IsExpression(), return false);
                            AST::EXPRESSION* Element = (AST::EXPRESSION*)Temp;
                            if(Element->Type != AST::BASE::TYPE::String){
                                Element->Error("Pin location not a string");
                                return false;
                            }
                            AssignPin(Body, ((AST::STRING*)Element)->Value,
                                                Pin->HDL_Name() +"["+ to_string(Pin->Width()-1-n) +"]");
                        }
                    }
                }else{
                    Pin->Warning();
                    printf("Creating virtual pin %s\n", Pin->HDL_Name().c_str());
                    Body += "set_instance_assignment "
                                    "-name VIRTUAL_PIN ON -to "+ Pin->HDL_Name();
                    if(Pin->Width() > 1) Body += "[*]";
                    Body += "\n";
                }
                auto Current = Pin->GetAttribValue("current");
                if(Current){
                    Body += "set_instance_assignment "
                                    "-name CURRENT_STRENGTH_NEW ";
                    switch(Current->Type){
                        case AST::BASE::TYPE::Literal:{
                            if(!((AST::LITERAL*)Current)->Value.IsReal()){
                                Current->Error("Current attribute not real");
                                return false;
                            }
                            NUMBER mA = ((AST::LITERAL*)Current)->Value;
                            mA.Mul(1e3);
                            Body += to_string((int)mA.GetReal()) + "MA";
                            break;
                        }
                        case AST::BASE::TYPE::String:
                            Body += '"' + ((AST::STRING*)Current)->Value + '"';
                            break;
                        default:
                            // TODO Need to also handle arrays (for vector types) correctly
                            Current->Error("Unexpected current strength attribute type");
                            break;
                    }
                    Body += " -to "+ Pin->HDL_Name();
                    if(Pin->Width() > 1) Body += "[*]";
                    Body += "\n";
                }
                auto WeakPullup = Pin->GetAttribValue("pullup");
                if(WeakPullup){
                    Body += "set_instance_assignment "
                                    "-name WEAK_PULL_UP_RESISTOR ";
                    switch(WeakPullup->Type){
                        case AST::BASE::TYPE::Literal:{
                            if(((AST::LITERAL*)WeakPullup)->Value == true) Body += "ON";
                            else                                           Body += "OFF";
                            break;
                        }
                        case AST::BASE::TYPE::String:
                            Body += '"' + ((AST::STRING*)WeakPullup)->Value + '"';
                            break;
                        default:
                            // TODO Need to also handle arrays (for vector types) correctly
                            WeakPullup->Error("Unexpected current strength attribute type");
                            break;
                    }
                    Body += " -to "+ Pin->HDL_Name();
                    if(Pin->Width() > 1) Body += "[*]";
                    Body += "\n";
                }
                break;
            }
            case BASE::TYPE::Group:{
                BuildPins(Body, (NAMESPACE*)SymbolIterator->second);
                break;
            }
            default:
                break;
        }
    }
    return true;
}
//------------------------------------------------------------------------------

bool PROJECT::BuildProject()
{
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
        "# Version 19.1\n"
        "# Date created = "+ Time +"\n"
        "#-------------------------------------------------------------------------------\n"
        "\n"
        "QUARTUS_VERSION  = \"19.1\"\n"
        "DATE             = \""+ Time +"\"\n"
        "PROJECT_REVISION = \""+ Filename +"\"\n";

    return WriteFile(Filename, "qpf", Body);
}
//------------------------------------------------------------------------------

bool PROJECT::BuildSettings()
{
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
        "# Version 19.1\n"
        "# Date created = "+ Time +"\n"
        "#-------------------------------------------------------------------------------\n"
        "\n";

    Body += // General settings
        "set_global_assignment -name FAMILY \""+ Series +"\"\n"
        "set_global_assignment -name DEVICE "+ Device +"\n"
        "set_global_assignment -name TOP_LEVEL_ENTITY "+ Filename +"\n"
        "set_global_assignment -name PROJECT_CREATION_TIME_DATE \""+ Time +"\"\n"
        "set_global_assignment -name PROJECT_OUTPUT_DIRECTORY output_files\n"
        "set_global_assignment -name PARTITION_NETLIST_TYPE SOURCE -section_id Top\n"
        "set_global_assignment -name PARTITION_FITTER_PRESERVATION_LEVEL PLACEMENT_AND_ROUTING -section_id Top\n"
        "set_global_assignment -name PARTITION_COLOR 16764057 -section_id Top\n"
        "set_global_assignment -name VERILOG_INPUT_VERSION SYSTEMVERILOG_2005\n"
        "#-------------------------------------------------------------------------------\n"
        "\n";

    auto Standard = Global.GetAttribValue("standard");
    Body += "set_global_assignment -name STRATIX_DEVICE_IO_STANDARD ";
    if(Standard){
        if(Standard->Type != AST::BASE::TYPE::String){
            Standard->Error("Standard attribute not a string");
            return false;
        }
        Body += "\""+ ((AST::STRING*)Standard)->Value +"\"\n\n";
    }else{
        Body += "\"3.3-V LVCMOS\"\n\n";
    }

    Body += "source \""+ Filename +".pin\"\n";
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
        "set_global_assignment -name SDC_FILE \""+ Filename +".sdc\"\n"
        "set_global_assignment -name CDF_FILE \""+ Filename +".cdf\"\n"
        "#-------------------------------------------------------------------------------\n"
        "\n"
        "set_instance_assignment -name PARTITION_HIERARCHY root_partition -to | -section_id Top\n";

    return WriteFile(Filename, "qsf", Body);
}
//------------------------------------------------------------------------------

bool PROJECT::BuildPins()
{
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
        "# Version 19.1\n"
        "# Date created = "+ Time +"\n"
        "#-------------------------------------------------------------------------------\n"
        "\n";

    if(!BuildPins(Body, &Global)) return false;
    Body +=
        "#-------------------------------------------------------------------------------\n"
        "\n";

    return WriteFile(Filename, "pin", Body);
}
//------------------------------------------------------------------------------

bool PROJECT::BuildDesignConstraints()
{
    SDC Constraints;

    return WriteFile(Filename, "sdc", Constraints.Build());
}
//------------------------------------------------------------------------------

bool PROJECT::BuildConfigChain()
{
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

bool PROJECT::Build(const char* Path, const char* Filename)
{
    this->Path     = Path;
    this->Filename = Filename;

    char   Time[0x100];
    time_t RawTime;
    time(&RawTime);
    strftime(Time, 0x100, "%H:%M:%S  %B %d, %Y", localtime(&RawTime));
    this->Time = Time;

    auto Device = Global.GetAttribValue("target_device");
    if(!Device){
        Error("Global attribute \"target_device\" not defined");
        return false;
    }
    if(Device->Type != AST::BASE::TYPE::String){
        Device->Error("Global attribute \"target_device\" not a string");
        return false;
    }
    this->Device = ((AST::STRING*)Device)->Value;

    auto Series = Global.GetAttribValue("target_series");
    if(!Series){
        Error("Global attribute \"target_series\" not defined");
        return false;
    }
    if(Series->Type != AST::BASE::TYPE::String){
        Series->Error("Global attribute \"target_series\" not a string");
        return false;
    }
    this->Series = ((AST::STRING*)Series)->Value;

    if(!BuildProject          ()) return false;
    if(!BuildSettings         ()) return false;
    if(!BuildPins             ()) return false;
    if(!BuildDesignConstraints()) return false;
    if(!BuildConfigChain      ()) return false;

    return true;
}
//------------------------------------------------------------------------------

