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

#include "Messages.h"
#include "AST/Definition.h"
#include "Altera/Project.h"
//------------------------------------------------------------------------------

class BACK_END{
  private:
    std::string Path;
    std::string Filename;

    void Error  (NETLIST::EXPRESSION* Expression, const char* Message = 0);
    void Warning(NETLIST::EXPRESSION* Expression, const char* Message = 0);

    bool WriteFile(std::string& Filename, const char* Ext, std::string& Body);

    bool DeleteUnused       (NETLIST::NAMESPACE* Module);
    bool AssignPinDirections(NETLIST::NAMESPACE* Module);
    bool RoutePorts         (NETLIST::NAMESPACE* Module);

    const char* GetTemporaryName();

    bool BuildExpression (std::string& Body, NETLIST::EXPRESSION* Expression, std::string& Temporary);
    bool BuildAssignments(std::string& Body, NETLIST::NAMESPACE*  Namespace);
    void BuildPorts      (std::string& Body, NETLIST::NAMESPACE*  Namespace, bool& isFirst);
    void BuildNets       (std::string& Body, NETLIST::NAMESPACE*  Namespace);

    bool BuildHDL(NETLIST::MODULE* Module, std::string Path);

  public:
    BACK_END();
   ~BACK_END();

    bool BuildAltera(const char* Path, const char* Filename);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

