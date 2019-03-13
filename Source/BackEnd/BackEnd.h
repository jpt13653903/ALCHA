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

#include "AST/Definition.h"
#include "Altera/Project.h"
//------------------------------------------------------------------------------

class BACK_END{
  private:
    std::string Path;
    std::string Filename;

    bool WriteFile(std::string& Filename, const char* Ext, std::string& Body);

    bool AssignPinDirections(OBJECTS::MODULE* Module);
    bool RoutePorts         (OBJECTS::MODULE* Module);

    bool BuildExpression(std::string& Body, OBJECTS::EXPRESSION* Expression);
    bool BuildHDL(OBJECTS::MODULE* Module, std::string Path);

  public:
    BACK_END();
   ~BACK_END();
   
    bool BuildAltera(const char* Path, const char* Filename);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

