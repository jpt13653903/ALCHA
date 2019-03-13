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
This is a binary search tree to optimise the scanner.  It facilitates the
greedy match operation.  The tree is only balanced once, so something like
a red-black tree is not required.

It consists of a balanced BST of the first character, each containing a
balanced sub-tree of the next character, etc.                                 */
//------------------------------------------------------------------------------

#ifndef Altera_Project_h
#define Altera_Project_h
//------------------------------------------------------------------------------

#include <time.h>
#include <string>
#include <stack>
//------------------------------------------------------------------------------

#include "Engine.h"
#include "FileWrapper.h"
#include "SDC.h"
//------------------------------------------------------------------------------

namespace ALTERA{
  class PROJECT{
    private:
      std::string Path;
      std::string Filename;

      std::string Time;
      std::string Device;
      std::string Series;

      bool WriteFile(std::string& Filename, const char* Ext, std::string& Body);

      void BuildFileList(std::string& Body, OBJECTS::NAMESPACE* Namespace, std::string Path);
      bool BuildPins    (std::string& Body);

      bool BuildProject          ();
      bool BuildSettings         ();
      bool BuildDesignConstraints();
      bool BuildConfigChain      ();

    public:
      PROJECT();
     ~PROJECT();

      bool Build(const char* Path, const char* Filename);
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

