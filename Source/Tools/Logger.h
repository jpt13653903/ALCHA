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

class Logger{
    private:
        FILE* file;

    public:
        Logger(const char* filename);
       ~Logger();

        void indent(int level);

        void print(const char*        message);
        void print(const std::string& message);
        void print(const char* format, int i);
        void print(const char* format, double d);
        void print(const char* format, unsigned u);
        void print(const char* format, const char* s);
        void print(const char* format, const std::string& s);
};
//------------------------------------------------------------------------------

extern Logger debug;
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
