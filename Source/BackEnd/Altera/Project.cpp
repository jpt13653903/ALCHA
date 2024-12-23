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
#include "Netlist/Synthesisable/pin.h"
//------------------------------------------------------------------------------

using std::string;
using std::to_string;
using namespace Altera;
using namespace Netlist;
//------------------------------------------------------------------------------

Project::Project(){}
//------------------------------------------------------------------------------

Project::~Project(){}
//------------------------------------------------------------------------------

void Project::printError(const char* message)
{
    ::printError(0, "", message);
}
//------------------------------------------------------------------------------

void Project::printWarning(const char* message)
{
    ::printWarning(0, "", message);
}
//------------------------------------------------------------------------------

bool Project::writeFile(string& filename, const char* ext, string& body)
{
    FileWrapper files;
    string fullname = path + "/" + filename + "." + ext;
    return files.writeAll(fullname.c_str(), (const byte*)body.c_str());
}
//------------------------------------------------------------------------------

void Project::buildFileList(string& body, Module* module, string path)
{
    bool isGlobal = (module == &global);

    for(auto symbolIterator: module->symbols){
        auto symbol = symbolIterator.second;
        if(symbol->type == Base::Type::Module){
            auto child = (Module*)symbol;
            if(isGlobal) buildFileList(body, child, "source");
            else         buildFileList(body, child, path + "/" + module->hdlName());
        }
    }
    if(isGlobal){
        body += "set_global_assignment -name VERILOG_FILE \""+ filename +".v\"\n";
    }else{
        body += "set_global_assignment -name VERILOG_FILE \"" +
                        path + "/" + module->name +".v\"\n";
    }
}
//------------------------------------------------------------------------------

void Project::assignPin(string& body, const string& location, const string& name)
{
    string P, N;
    bool diff = false;

    for(size_t n = 0; n < location.length(); n++){
        switch(location[n]){
            case ' ':
            case '\t':
                break;
            case '-':
                diff = true;
                break;
            default:
                if(diff) N += location[n];
                else     P += location[n];
                break;
        }
    }
    body += "set_location_assignment PIN_"+ P + " -to "+ name +"\n";
    if(diff){
        body += "set_location_assignment PIN_"+ N + " -to "+ name +"(n)\n";
    }
}
//------------------------------------------------------------------------------

bool Project::buildPins(string& body, NameSpace* nameSpace)
{
    for(auto symbolIterator: nameSpace->symbols){
        switch(symbolIterator.second->type){
            case Base::Type::Pin:{
                auto pin = (Pin*)(symbolIterator.second);
                auto standard = pin->getAttribValue("standard");
                if(standard){
                    if(standard->type != AST::Base::Type::String){
                        standard->printError("standard attribute not a string");
                        return false;
                    }
                    body += "set_instance_assignment -name "
                                    "IO_STANDARD \""+ ((AST::String*)standard)->value +"\" -to "+ pin->hdlName();
                    if(pin->width() > 1) body += "[*]";
                    body += "\n";
                }
                auto location = pin->getAttribValue("location");
                if(location){
                    if(pin->width() == 1){
                        if(location->type != AST::Base::Type::String){
                            location->printError("Scalar pin location not a string");
                            return false;
                        }
                        assignPin(body, ((AST::String*)location)->value, pin->hdlName());
                    }else{
                        if(location->type != AST::Base::Type::Array){
                            location->printError("Vector pin location not an array");
                            return false;
                        }
                        auto locationArray = (AST::Array*)location;
                        if(locationArray->elements.size() != (size_t)pin->width()){
                            locationArray->printError("Vector pin location array of wrong size");
                            return false;
                        }
                        for(int n = 0; n < pin->width(); n++){
                            AST::Base* temp = locationArray->elements[n];
                            assert(temp && temp->isExpression(), return false);
                            AST::Expression* element = (AST::Expression*)temp;
                            if(element->type != AST::Base::Type::String){
                                element->printError("pin location not a string");
                                return false;
                            }
                            assignPin(body, ((AST::String*)element)->value,
                                                pin->hdlName() +"["+ to_string(pin->width()-1-n) +"]");
                        }
                    }
                }else{
                    pin->printWarning();
                    printf("Creating virtual pin %s\n", pin->hdlName().c_str());
                    body += "set_instance_assignment "
                                    "-name VIRTUAL_PIN ON -to "+ pin->hdlName();
                    if(pin->width() > 1) body += "[*]";
                    body += "\n";
                }
                auto current = pin->getAttribValue("current");
                if(current){
                    body += "set_instance_assignment "
                                    "-name CURRENT_STRENGTH_NEW ";
                    switch(current->type){
                        case AST::Base::Type::Literal:{
                            if(!((AST::Literal*)current)->value.isReal()){
                                current->printError("current attribute not real");
                                return false;
                            }
                            Number mA = ((AST::Literal*)current)->value;
                            mA.mul(1e3);
                            body += to_string((int)mA.getReal()) + "MA";
                            break;
                        }
                        case AST::Base::Type::String:
                            body += '"' + ((AST::String*)current)->value + '"';
                            break;
                        default:
                            // TODO Need to also handle arrays (for vector types) correctly
                            current->printError("Unexpected current strength attribute type");
                            break;
                    }
                    body += " -to "+ pin->hdlName();
                    if(pin->width() > 1) body += "[*]";
                    body += "\n";
                }
                auto weakPullup = pin->getAttribValue("pullup");
                if(weakPullup){
                    body += "set_instance_assignment "
                                    "-name WEAK_PULL_UP_RESISTOR ";
                    switch(weakPullup->type){
                        case AST::Base::Type::Literal:{
                            if(((AST::Literal*)weakPullup)->value == true) body += "ON";
                            else                                           body += "OFF";
                            break;
                        }
                        case AST::Base::Type::String:
                            body += '"' + ((AST::String*)weakPullup)->value + '"';
                            break;
                        default:
                            // TODO Need to also handle arrays (for vector types) correctly
                            weakPullup->printError("Unexpected current strength attribute type");
                            break;
                    }
                    body += " -to "+ pin->hdlName();
                    if(pin->width() > 1) body += "[*]";
                    body += "\n";
                }
                break;
            }
            case Base::Type::Group:{
                buildPins(body, (NameSpace*)symbolIterator.second);
                break;
            }
            default:
                break;
        }
    }
    return true;
}
//------------------------------------------------------------------------------

bool Project::buildProject()
{
    string body;

    body +=
        "#-------------------------------------------------------------------------------\n"
        "#\n"
        "# Generated by ALCHA version " +
          to_string(MAJOR_VERSION) +"."+ to_string(MINOR_VERSION) +
        ", build \"" __DATE__ ", " __TIME__ "\"\n"
        "#-------------------------------------------------------------------------------\n"
        "#\n"
        "# Quartus II 64-Bit\n"
        "# Version 19.1\n"
        "# Date created = "+ time +"\n"
        "#-------------------------------------------------------------------------------\n"
        "\n"
        "QUARTUS_VERSION  = \"19.1\"\n"
        "DATE             = \""+ time +"\"\n"
        "PROJECT_REVISION = \""+ filename +"\"\n";

    return writeFile(filename, "qpf", body);
}
//------------------------------------------------------------------------------

bool Project::buildSettings()
{
    string body;

    body += // Header
        "#-------------------------------------------------------------------------------\n"
        "#\n"
        "# Generated by ALCHA version " +
          to_string(MAJOR_VERSION) +"."+ to_string(MINOR_VERSION) +
        ", build \"" __DATE__ ", " __TIME__ "\"\n"
        "#-------------------------------------------------------------------------------\n"
        "#\n"
        "# Quartus II 64-Bit\n"
        "# Version 19.1\n"
        "# Date created = "+ time +"\n"
        "#-------------------------------------------------------------------------------\n"
        "\n";

    body += // General settings
        "set_global_assignment -name FAMILY \""+ series +"\"\n"
        "set_global_assignment -name DEVICE "+ device +"\n"
        "set_global_assignment -name TOP_LEVEL_ENTITY "+ filename +"\n"
        "set_global_assignment -name PROJECT_CREATION_TIME_DATE \""+ time +"\"\n"
        "set_global_assignment -name PROJECT_OUTPUT_DIRECTORY output_files\n"
        "set_global_assignment -name PARTITION_NETLIST_TYPE SOURCE -section_id Top\n"
        "set_global_assignment -name PARTITION_FITTER_PRESERVATION_LEVEL PLACEMENT_AND_ROUTING -section_id Top\n"
        "set_global_assignment -name PARTITION_COLOR 16764057 -section_id Top\n"
        "set_global_assignment -name VERILOG_INPUT_VERSION SYSTEMVERILOG_2005\n"
        "#-------------------------------------------------------------------------------\n"
        "\n";

    auto standard = global.getAttribValue("standard");
    body += "set_global_assignment -name STRATIX_DEVICE_IO_STANDARD ";
    if(standard){
        if(standard->type != AST::Base::Type::String){
            standard->printError("standard attribute not a string");
            return false;
        }
        body += "\""+ ((AST::String*)standard)->value +"\"\n\n";
    }else{
        body += "\"3.3-V LVCMOS\"\n\n";
    }

    body += "source \""+ filename +".pin\"\n";
    body +=
        "#-------------------------------------------------------------------------------\n"
        "\n";

    // body += // TODO - location attributes
        // "set_location_assignment PLL_1 -to \"altpll:PLL\"\n"

    // body += // TODO - HDL construct dependency list
        //"set_global_assignment -name QIP_FILE Qsys/ADC/synthesis/ADC.qip\n"

    // File list
    buildFileList(body, &global, "");

    body += // Tail end
        "\n"
        "set_global_assignment -name SDC_FILE \""+ filename +".sdc\"\n"
        "set_global_assignment -name CDF_FILE \""+ filename +".cdf\"\n"
        "#-------------------------------------------------------------------------------\n"
        "\n"
        "set_instance_assignment -name PARTITION_HIERARCHY root_partition -to | -section_id Top\n";

    return writeFile(filename, "qsf", body);
}
//------------------------------------------------------------------------------

bool Project::buildPins()
{
    string body;

    body += // Header
        "#-------------------------------------------------------------------------------\n"
        "#\n"
        "# Generated by ALCHA version " +
          to_string(MAJOR_VERSION) +"."+ to_string(MINOR_VERSION) +
        ", build \"" __DATE__ ", " __TIME__ "\"\n"
        "#-------------------------------------------------------------------------------\n"
        "#\n"
        "# Quartus II 64-Bit\n"
        "# Version 19.1\n"
        "# Date created = "+ time +"\n"
        "#-------------------------------------------------------------------------------\n"
        "\n";

    if(!buildPins(body, &global)) return false;
    body +=
        "#-------------------------------------------------------------------------------\n"
        "\n";

    return writeFile(filename, "pin", body);
}
//------------------------------------------------------------------------------

bool Project::buildDesignConstraints()
{
    SDC constraints;

    return writeFile(filename, "sdc", constraints.build());
}
//------------------------------------------------------------------------------

bool Project::buildConfigChain()
{
    string body;

    body =
        "JedecChain;\n"
        "\tFileRevision(JESD32A);\n"
        "\tDefaultMfr(6E);\n"
        "\n"
        "\tP ActionCode(Cfg)\n"
        "\t\tDevice PartName("+ device +") "
        "path(\"output_files/\") " "File(\""+ filename +".sof\") " "MfrSpec(OpMask(1));\n"
        "\n"
        "ChainEnd;\n"
        "\n"
        "AlteraBegin;\n"
        "\tChainType(JTAG);\n"
        "AlteraEnd\n";

    return writeFile(filename, "cdf", body);
}
//------------------------------------------------------------------------------

bool Project::build(const char* path, const char* filename)
{
    this->path     = path;
    this->filename = filename;

    char   _time[0x100];
    time_t rawTime;
    ::time(&rawTime);
    strftime(_time, 0x100, "%H:%M:%S  %B %d, %Y", localtime(&rawTime));
    this->time = _time;

    auto device = global.getAttribValue("target_device");
    if(!device){
        printError("global attribute \"target_device\" not defined");
        return false;
    }
    if(device->type != AST::Base::Type::String){
        device->printError("global attribute \"target_device\" not a string");
        return false;
    }
    this->device = ((AST::String*)device)->value;

    auto series = global.getAttribValue("target_series");
    if(!series){
        printError("global attribute \"target_series\" not defined");
        return false;
    }
    if(series->type != AST::Base::Type::String){
        series->printError("global attribute \"target_series\" not a string");
        return false;
    }
    this->series = ((AST::String*)series)->value;

    if(!buildProject          ()) return false;
    if(!buildSettings         ()) return false;
    if(!buildPins             ()) return false;
    if(!buildDesignConstraints()) return false;
    if(!buildConfigChain      ()) return false;

    return true;
}
//------------------------------------------------------------------------------

