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

#ifndef Netlist_Module_h
#define Netlist_Module_h
//------------------------------------------------------------------------------

#include <map>
#include <list>
//------------------------------------------------------------------------------

#include "../Namespace.h"
//------------------------------------------------------------------------------

namespace Netlist{
    struct Module: public NameSpace{
                 Module(int line, const std::string& filename, const char* name = "");
        virtual ~Module(); // Also cleans up the children

        void display(int indent = 0) override;

        void validate() override;
    };
    //----------------------------------------------------------------------------

    extern std::list<NameSpace*> nameSpaceStack; // Used for namespace searches
    extern Module                global;         // The root of the module tree
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

