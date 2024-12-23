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

/* This class is used as a base for Pin and Net
  *
  */
//------------------------------------------------------------------------------

#ifndef Netlist_Synthesisable_h
#define Netlist_Synthesisable_h
//------------------------------------------------------------------------------

#include "Base.h"
#include "Num.h"
#include "AST/Definition.h"
//------------------------------------------------------------------------------

namespace Netlist{
    class Synthesisable: public Base{
        private:
            struct Format{
                Num* width;
                Num* fullScale;
                Num* isSigned;
            } format;

        protected:
            void displayParameters(int indent);

        public:
            bool    used; // Actually used in an expression somewhere
            int     width    () override;
            Number& fullScale() override;
            bool    isSigned () override;

            // If "Signed" is not specified, the signedness is taken from the sign
            // of Width and / or FullScale, otherwise both Width and FullScale are
            // expected to be positive.
            void setFixedPoint(int width, const Number& fullScale);
            void setFixedPoint(int width, const Number& fullScale, bool isSigned);

            AST::Definition::Direction direction;

                     Synthesisable(int line, const std::string& filename, const char* name, Type type);
            virtual ~Synthesisable();

            bool isSynthesisable() override;

            bool applyParameters(std::list<AST::Base*>& parameters);

            Base*            getAttribute            (const std::string& name) override;
            AST::Expression* getBuiltInAttributeValue(const std::string& name) override;

            void validate() override;
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

