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

#ifndef Altera_Project_h
#define Altera_Project_h
//------------------------------------------------------------------------------

#include <time.h>
#include <string>
#include <stack>
//------------------------------------------------------------------------------

#include "Engine.h"
#include "Utilities.h"
#include "FileWrapper.h"
#include "SDC.h"

#include "AST/Expression.h"

#include "Netlist/Namespace/Module.h"
#include "Netlist/Namespace.h"
//------------------------------------------------------------------------------

namespace Altera{
    class Project{
        private:
            std::string path;
            std::string filename;

            std::string time;
            std::string device;
            std::string series;

            void printError  (const char* message = 0);
            void printWarning(const char* message = 0);

            bool writeFile(std::string& filename, const char* ext, std::string& body);

            void buildFileList(std::string& body, Netlist::Module   * module, std::string path);
            void assignPin    (std::string& body, const std::string& location, const std::string& name);
            bool buildPins    (std::string& body, Netlist::NameSpace* nameSpace);

            bool buildProject          ();
            bool buildSettings         ();
            bool buildPins             ();
            bool buildDesignConstraints();
            bool buildConfigChain      ();

        public:
            Project();
           ~Project();

            bool build(const char* path, const char* filename);
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

