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

#ifndef Utilities_h
#define Utilities_h
//------------------------------------------------------------------------------

#include <string>
//------------------------------------------------------------------------------

#include "General.h"
//------------------------------------------------------------------------------

void printError  (int line, const std::string& filename, const char* message = 0);
void printWarning(int line, const std::string& filename, const char* message = 0);

// Replaces "path/../" patterns with "/".
// In Windows, '\' is also an accepted path specifier.
void simplifyFilename(std::string& filename);

// Adds spaces so that the "cursor" is at the specified column
void align(std::string& body, int column);
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
