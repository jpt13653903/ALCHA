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

#include "Logger.h"
//------------------------------------------------------------------------------

using std::string;
//------------------------------------------------------------------------------

Logger logger("Debug.log");
//------------------------------------------------------------------------------

Logger::Logger(const char* filename)
{
    file = fopen(filename, "wb");
}
//------------------------------------------------------------------------------

Logger::~Logger()
{
    fclose(file);
}
//------------------------------------------------------------------------------

void Logger::indent(int level)
{
    for(int n = 0; n < level; n++) fprintf(file, "    ");
}
//------------------------------------------------------------------------------

void Logger::print(const char* message)
{
    fprintf(file, "%s", message);
}
//------------------------------------------------------------------------------

void Logger::print(const string& message)
{
    fprintf(file, "%s", message.c_str());
}
//------------------------------------------------------------------------------

void Logger::print(const char* format, int i)
{
    fprintf(file, format, i);
}
//------------------------------------------------------------------------------

void Logger::print(const char* format, double d)
{
    fprintf(file, format, d);
}
//------------------------------------------------------------------------------

void Logger::print(const char* format, unsigned u)
{
    fprintf(file, format, u);
}
//------------------------------------------------------------------------------

void Logger::print(const char* format, const char* s)
{
    fprintf(file, format, s);
}
//------------------------------------------------------------------------------

void Logger::print(const char* format, const string& s)
{
    fprintf(file, format, s.c_str());
}
//------------------------------------------------------------------------------

