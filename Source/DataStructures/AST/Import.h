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

#ifndef AST_Import_h
#define AST_Import_h
//------------------------------------------------------------------------------

#include "Base.h"
//------------------------------------------------------------------------------

namespace AST{
    class Import: public Base{
        private:
            Base* ast; // Used to store in-line ASTs

        public:
            std::string file;
            std::string nameSpace;

            Import(int line, std::string& filename);
            Import(int line, const char*  filename);
           ~Import();

            // Returns a copy of this instance
            Base* copy() override;

            bool runAST() override;
            bool getVerilog(std::string& body) override;

            void display() override;

            void validateMembers() override;
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

