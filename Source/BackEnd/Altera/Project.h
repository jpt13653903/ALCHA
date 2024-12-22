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

namespace ALTERA{
    class PROJECT{
        private:
            std::string Path;
            std::string Filename;

            std::string Time;
            std::string Device;
            std::string Series;

            void Error  (const char* Message = 0);
            void Warning(const char* Message = 0);

            bool WriteFile(std::string& Filename, const char* Ext, std::string& Body);

            void BuildFileList(std::string& Body, NETLIST::MODULE   * Module, std::string Path);
            void AssignPin    (std::string& Body, const std::string& Location, const std::string& Name);
            bool BuildPins    (std::string& Body, NETLIST::NAMESPACE* Namespace);

            bool BuildProject          ();
            bool BuildSettings         ();
            bool BuildPins             ();
            bool BuildDesignConstraints();
            bool BuildConfigChain      ();

        public:
            PROJECT();
          ~PROJECT();

            bool Build(const char* Path, const char* Filename);
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

