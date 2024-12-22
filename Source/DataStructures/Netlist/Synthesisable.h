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

/* This class is used as a base for PIN and NET
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

namespace NETLIST{
    class SYNTHESISABLE: public BASE{
        private:
            struct FORMAT{
                NUM* Width;
                NUM* FullScale;
                NUM* Signed;
            } Format;

        protected:
            void DisplayParameters(int Indent);

        public:
            bool    Used; // Actually used in an expression somewhere
            int     Width    () override;
            NUMBER& FullScale() override;
            bool    Signed   () override;

            // If "Signed" is not specified, the signedness is taken from the sign
            // of Width and / or FullScale, otherwise both Width and FullScale are
            // expected to be positive.
            void SetFixedPoint(int Width, const NUMBER& FullScale);
            void SetFixedPoint(int Width, const NUMBER& FullScale, bool Signed);

            AST::DEFINITION::DIRECTION Direction;

                              SYNTHESISABLE(int Line, const std::string& Filename, const char* Name, TYPE Type);
            virtual ~SYNTHESISABLE();

            bool IsSynthesisable() override;

            bool ApplyParameters(std::list<AST::BASE*>& Parameters);

            BASE*            GetAttribute            (const std::string& Name) override;
            AST::EXPRESSION* GetBuiltInAttributeValue(const std::string& Name) override;

            void Validate() override;
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

