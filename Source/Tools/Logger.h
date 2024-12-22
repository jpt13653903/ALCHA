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

/**
    Class wrapper for GNU MP rational numbers
*/
//------------------------------------------------------------------------------

#ifndef Logger_h
#define Logger_h
//------------------------------------------------------------------------------

#include <stdio.h>
#include <string>
//------------------------------------------------------------------------------

class LOGGER{
    private:
        FILE* File;

    public:
        LOGGER(const char* Filename);
      ~LOGGER();

        void Indent(int Level);

        void Print(const char*        String);
        void Print(const std::string& String);
        void Print(const char* Format, int i);
        void Print(const char* Format, double d);
        void Print(const char* Format, unsigned u);
        void Print(const char* Format, const char* s);
        void Print(const char* Format, const std::string& s);
};
//------------------------------------------------------------------------------

extern LOGGER Debug;
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
