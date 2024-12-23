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

#include "Token.h"
//------------------------------------------------------------------------------

Token::Token()
{
    line = 0;
    type = Type::Unknown;
}
//------------------------------------------------------------------------------

void Token::display()
{
    debug.print(ANSI_FG_CYAN "Token: " ANSI_RESET);
    debug.print(ANSI_FG_BRIGHT_BLACK "Type = "  ANSI_RESET "%3d "  , (int)type);
    debug.print(ANSI_FG_BRIGHT_BLACK "Line = "  ANSI_RESET "%5d "  , line);
    debug.print(ANSI_FG_BRIGHT_BLACK "Data = "  ANSI_RESET "%-40s ", data.c_str());
    debug.print(ANSI_FG_BRIGHT_BLACK "Value = " ANSI_RESET "%s\n"  , value.display());
}
//------------------------------------------------------------------------------

