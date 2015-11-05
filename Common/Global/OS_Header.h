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

#ifndef OS_Header_h
#define OS_Header_h
//------------------------------------------------------------------------------

#if defined(__WIN32)
 #define  NTDDI_VERSION 0x05010000 // Windows XP
 #define  WINVER        0x0501
 #define _WIN32_WINNT   WINVER
 #define _WIN32_IE      WINVER
 #define  UNICODE

 #include <windows.h>
//------------------------------------------------------------------------------

#elif defined(__linux__)
 #error Linux not yet supported at this point.
//------------------------------------------------------------------------------

#elif defined(__APPLE__)
 #error MAC OS not yet supported at this point.
//------------------------------------------------------------------------------

#else
 #error Your operating system is not supported at this point.
#endif
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
