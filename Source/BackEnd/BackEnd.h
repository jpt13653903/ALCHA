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

#ifndef BackEnd_h
#define BackEnd_h
//------------------------------------------------------------------------------

#include "Utilities.h"
#include "AST/Definition.h"
#include "AST/Expression.h"
#include "Netlist/Synthesisable/PinComponent.h"
#include "Netlist/NameSpace/Module.h"
#include "Altera/Project.h"
//------------------------------------------------------------------------------

class BackEnd{
    private:
        std::string path;
        std::string filename;

        void printError  (AST::Expression* expression, const char* message = 0);
        void printWarning(AST::Expression* expression, const char* message = 0);

        bool writeFile(std::string& filename, const char* ext, std::string& body);

        void removeTempNet(Netlist::Net* target);

        void removeTempNets     (Netlist::NameSpace* nameSpace);
        void populateUsed       (Netlist::NameSpace* nameSpace);
        bool deleteUnused       (Netlist::NameSpace* nameSpace);
        bool assignPinDirections(Netlist::NameSpace* nameSpace);
        bool routePorts         (Netlist::NameSpace* nameSpace);

        void buildSizeDef    (std::string& body, int width, bool isSigned);
        bool buildAssignments(std::string& body, Netlist::NameSpace* nameSpace);
        void buildPorts      (std::string& body, Netlist::NameSpace* nameSpace, bool& isFirst);
        void buildNets       (std::string& body, Netlist::NameSpace* nameSpace);

        bool buildHDL(Netlist::Module* module, std::string path);

    public:
        BackEnd();
       ~BackEnd();

        bool buildAltera(const char* path, const char* filename);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

