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

#include "BackEnd.h"
#include "AST/Expression/Object.h"
#include "Netlist/Synthesisable/Pin.h"
#include "Netlist/Synthesisable/Net.h"
//------------------------------------------------------------------------------

using std::string;
using std::to_string;
using namespace Netlist;
//------------------------------------------------------------------------------

BackEnd::BackEnd(){}
//------------------------------------------------------------------------------

BackEnd::~BackEnd(){}
//------------------------------------------------------------------------------

void BackEnd::printError(AST::Expression* expression, const char* message)
{
    ::printError(expression->source.line, expression->source.filename, message);
}
//------------------------------------------------------------------------------

void BackEnd::printWarning(AST::Expression* expression, const char* message)
{
    ::printWarning(expression->source.line, expression->source.filename.c_str(), message);
}
//------------------------------------------------------------------------------

void BackEnd::removeTempNet(Netlist::Net* target)
{
    if(target->value){
        target->value = target->value->removeTempNet(target->width(), target->isSigned());
    }
}
//------------------------------------------------------------------------------

void BackEnd::removeTempNets(NameSpace* nameSpace)
{
    if(!nameSpace) return;

    logger.print("Delete temporary nets...\n");

    for(auto symbolIterator: nameSpace->symbols){
        switch(symbolIterator.second->type){
            case Base::Type::Pin:
                removeTempNet(((Pin*)symbolIterator.second)->driver );
                removeTempNet(((Pin*)symbolIterator.second)->enabled);
                break;

            case Base::Type::Net:
                removeTempNet((Net*)symbolIterator.second);
                break;

            case Base::Type::Module:
            case Base::Type::Group:{
                removeTempNets((NameSpace*)(symbolIterator.second));
                break;
            }

            default:
                break;
        }
    }
    return;
}
//------------------------------------------------------------------------------

void BackEnd::populateUsed(NameSpace* nameSpace)
{
    logger.print("Populate used...\n");

    for(auto symbolIterator: nameSpace->symbols){
        switch(symbolIterator.second->type){
            case Base::Type::Pin: // If it's used, it must end up at a pin
                symbolIterator.second->populateUsed(false);
                break;

            case Base::Type::Module:
            case Base::Type::Group:
                populateUsed((NameSpace*)(symbolIterator.second));
                break;

            default:
                break;
        }
    }
}
//------------------------------------------------------------------------------

bool BackEnd::deleteUnused(NameSpace* nameSpace)
{
    logger.print("Delete unused...\n");

    auto symbolIterator = nameSpace->symbols.begin();

    while(symbolIterator != nameSpace->symbols.end()){
        switch(symbolIterator->second->type){
            case Base::Type::Byte:
            case Base::Type::Character:
            case Base::Type::Number:
            case Base::Type::Alias:{
                auto object = (Synthesisable*)(symbolIterator->second);
                symbolIterator++;
                nameSpace->symbols.erase(object->name);
                delete object;
                break;
            }

            case Base::Type::Pin:
            case Base::Type::Net:{
                auto object = (Synthesisable*)(symbolIterator->second);
                symbolIterator++;
                if(!object->used){
                    if(!object->isTemporary()){
                        object->printWarning();
                        printf("Deleting unused object %s\n", object->hdlName().c_str());
                    }
                    nameSpace->symbols.erase(object->name);
                    delete object;
                }
                break;
            }

            case Base::Type::Module:
            case Base::Type::Group:{
                auto object = (NameSpace*)(symbolIterator->second);
                deleteUnused(object);
                symbolIterator++;
                if(object->symbols.empty()){
                    object->printWarning();
                    printf("Deleting unused object %s\n", object->hdlName().c_str());
                    nameSpace->symbols.erase(object->name);
                    delete object;
                }
                break;
            }

            default:
                error("Type %d not handled", (int)symbolIterator->second->type);
                symbolIterator++;
                break;
        }
    }
    return true;
}
//------------------------------------------------------------------------------

bool BackEnd::assignPinDirections(NameSpace* nameSpace)
{
    logger.print("Assign pin directions...\n");

    for(auto symbolIterator: nameSpace->symbols){
        switch(symbolIterator.second->type){
            case Base::Type::Pin:{
                auto pin = (Pin*)(symbolIterator.second);
                if(pin->direction == AST::Definition::Direction::Inferred){
                    if(pin->enabled->value){ // Possible bidirectional
                        if(pin->enabled->value->type == AST::Base::Type::Literal){
                            if(((AST::Literal*)pin->enabled->value)->value == 0){
                                pin->direction = AST::Definition::Direction::Input;
                            }else{
                                pin->direction = AST::Definition::Direction::Output;
                            }
                        }else{
                            pin->direction = AST::Definition::Direction::Bidirectional;
                        }
                    }else{ // Enabled is undefined
                        if(pin->driver->value){
                            pin->direction = AST::Definition::Direction::Output;
                        }else{
                            pin->direction = AST::Definition::Direction::Input;
                        }
                    }
                }
                break;
            }
            case Base::Type::Module:
            case Base::Type::Group:
                assignPinDirections((NameSpace*)(symbolIterator.second));
                break;

            default:
                break;
        }
    }
    return true;
}
//------------------------------------------------------------------------------

bool BackEnd::routePorts(NameSpace* nameSpace)
{
    logger.print("Route ports...\n");

    // At this point, the expressions use pointers, not names.  Any inter-module
    // usage needs to be broken into temporary signals throughout the hierarchy
    // and assigned to either module ports or internal signals.  Pins need to be
    // routed to be moved to the top-level entity, and the original replaced
    // with HDL module ports.

    // Do the children first
    for(auto symbolIterator: nameSpace->symbols){
        if(symbolIterator.second->isNameSpace()){
            routePorts((NameSpace*)(symbolIterator.second));
        }
    }

    // If this is the global module, don't go further
    if(!nameSpace->nameSpace) return true;

    // Route inter-module connections to the parent
    error("Not yet implemented");
    return true;
}
//------------------------------------------------------------------------------

bool BackEnd::writeFile(string& filename, const char* ext, string& body)
{
    FileWrapper files;
    string fullname = path + "/" + filename + "." + ext;
    return files.writeAll(fullname.c_str(), (const byte*)body.c_str());
}
//------------------------------------------------------------------------------

bool BackEnd::buildAssignments(string& body, NameSpace* nameSpace)
{
    for(auto symbolIterator: nameSpace->symbols){
        auto object = symbolIterator.second;
        switch(object->type){
            case Base::Type::Pin:{
                auto pin = (Pin*)object;
                if(pin->driver->value){
                    string driver;
                    if(!pin->driver->value->getVerilog(driver)) return false;
                    if(pin->enabled->value){
                        string enabled;
                        if(!pin->enabled->value->getVerilog(enabled)) return false;
                        body += "assign "+ pin->escapedName();
                        align(body, 25);
                        body += "= |("+ enabled + ") ? ("+ driver + ")"
                                " : " + to_string(pin->width()) + "'bZ;";
                        align(body, 70);
                        body += "// " + pin->driver ->value->source.filename
                             + " +" + to_string(pin->driver ->value->source.line)
                             + " (driver); "
                             + pin->enabled->value->source.filename
                             + " +" + to_string(pin->enabled->value->source.line)
                             + " (enabled)\n";
                    }else{
                        body += "assign "+ pin->escapedName();
                        align(body, 25);
                        body += "= "+ driver + ";";
                        align(body, 70);
                        body += "// " + pin->driver->value->source.filename
                             + " +" + to_string(pin->driver->value->source.line)
                             + "\n";
                    }
                }
                break;
            }
            case Base::Type::Net:{
                auto net = (Net*)object;
                if(net->value){
                    string value;
                    if(!net->value->getVerilog(value)) return false;
                    body += "assign "+ net->escapedName();
                    align(body, 25);
                    body += "= "+ value +";";
                    align(body, 70);
                    body += "// " + net->value->source.filename
                         + " +" + to_string(net->value->source.line) + "\n";
                }
                break;
            }
            case Base::Type::Group:{
                if(!buildAssignments(body, (NameSpace*)object)) return false;
                break;
            }
            default:
                break;
        }
    }
    return true;
}
//------------------------------------------------------------------------------

void BackEnd::buildSizeDef(string& body, int width, bool isSigned)
{
    int top;
    if(isSigned) top = width;
    else       top = width-1;

    if(top > 0){
        body += "[";
        if(top < 100) body += ' ';
        if(top <  10) body += ' ';
        body += to_string(top) +":0]";
    }else{
        body += "       ";
    }
}
//------------------------------------------------------------------------------

void BackEnd::buildPorts(string& body, NameSpace* nameSpace, bool& isFirst)
{
    for(auto symbolIterator: nameSpace->symbols){
        auto object = symbolIterator.second;
        switch(object->type){
            case Base::Type::Pin:{
                auto pin = (Pin*)object;
                if(!isFirst) body += ",\n";
                isFirst = false;

                switch(pin->direction){
                    case AST::Definition::Direction::Input : body += "  input  logic "; break;
                    case AST::Definition::Direction::Output: body += "  output logic "; break;
                    default                                : body += "  inout  logic "; break;
                }
                if(pin->isSigned()) body += "isSigned ";
                else              body += "       ";
                buildSizeDef(body, pin->width(), pin->isSigned());
                body += pin->escapedName();
                break;
            }
            case Base::Type::Group:{
                buildPorts(body, (NameSpace*)object, isFirst);
                break;
            }
            default:
                break;
        }
    }
}
//------------------------------------------------------------------------------

void BackEnd::buildNets(string& body, NameSpace* nameSpace)
{
    for(auto symbolIterator: nameSpace->symbols){
        switch(symbolIterator.second->type){
            case Base::Type::Net:{
                auto net = (Net*)symbolIterator.second;
                body += "logic ";
                if(net->isSigned()) body += "Signed ";
                else              body += "       ";
                buildSizeDef(body, net->width(), net->isSigned());
                body += net->escapedName() + ";\n";
                break;
            }
            case Base::Type::Group:{
                buildNets(body, (NameSpace*)symbolIterator.second);
                break;
            }
            default:
                break;
        }
    }
}
//------------------------------------------------------------------------------

bool BackEnd::buildHDL(Module* module, string path)
{
    bool isGlobal = (module == &global);

    // Recursively generate the modules (each namespace is a module)
    for(auto symbolIterator: module->symbols){
        auto symbol = symbolIterator.second;
        if(symbol->type == Base::Type::Module){
            auto child = (Module*)symbol;
            if(isGlobal) buildHDL(child, "source");
            else         buildHDL(child, path + "/" + module->hdlName());
        }
    }
    // Generate this module's name
    string name;
    if(isGlobal) name = filename;
    else         name = module->escapedName();

    // Header
    string body;
    body = "// Auto-generated by ALCHA "
           "Version "+ to_string(MAJOR_VERSION) +"."+ to_string(MINOR_VERSION) +" ("
           "Built on " __DATE__ " at " __TIME__ ")\n"
           "//--------------------------------------"
           "----------------------------------------\n\n";

    // Module Definition
    body += "module "+ name +"(\n";

    // Ports
    bool isFirst = true;
    buildPorts(body, module, isFirst);
    if(!isFirst) body += "\n";
    body += ");\n";
    body += "//--------------------------------------"
            "----------------------------------------\n\n";

    // Nets
    buildNets(body, module);
    body += "//--------------------------------------"
            "----------------------------------------\n\n";

    // Assignments
    if(!buildAssignments(body, module)) return false;

    body += "//--------------------------------------"
            "----------------------------------------\n\n";
    body += "endmodule\n";
    body += "//--------------------------------------"
            "----------------------------------------\n\n";

    if(isGlobal) name = filename;
    else         name = path + "/" + module->name;
    writeFile(name, "v", body);

    return true;
}
//------------------------------------------------------------------------------

bool BackEnd::buildAltera(const char* path, const char* filename)
{
    this->path     = path;
    this->filename = filename;

    logger.print(
        ANSI_FG_GREEN "\nStarting BackEnd -----------------------"
                      "----------------------------------------\n\n"
        ANSI_RESET
    );

    removeTempNets(&global);
    logger.print("\n");

    populateUsed(&global);
    logger.print("\n");

    if(!deleteUnused(&global)) return false;
    logger.print("\n");

    if(!assignPinDirections(&global)) return false;
    logger.print("\n");

    if(!routePorts(&global)) return false;
    logger.print("\n");

    global.display();

    logger.print(
        ANSI_FG_GREEN "\nBuilding Project -----------------------"
                      "----------------------------------------\n\n"
        ANSI_RESET
    );

    Altera::Project project;
    project.build(path, filename);

    if(!buildHDL(&global, "")) return false;

    return true;
}
//------------------------------------------------------------------------------

