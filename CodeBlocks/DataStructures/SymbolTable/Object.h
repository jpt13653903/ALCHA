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

#ifndef Object_h
#define Object_h
//------------------------------------------------------------------------------

#include "AST.h"
//------------------------------------------------------------------------------

struct OBJECT{ // Base class for the symbol table
 enum TYPE{
  Target,                   // The target object
  Pin, Sig, Clk,            // Physical signals
  Int, Rat, Float, Complex, // Scripting variables
  Array,                    // Array of...
  Class,                    // Class type
  ClassInstance,            // Class instance
  Function,                 // Actual function, with an AST as a body
  FuncPtr,                  // Function pointer
  Namespace                 // Namespace of an imported file
 } Type;

 // The object name (or reference) is stored in the symbol table, not as
 // part of the object.
 enum{Output, Input, Bidirectional} Direction;

 DICTIONARY Attributes; // Makes use of AST.h AttributeXXX functions
 void       Attribute_Add(const byte* Name, STRING* Value);

          OBJECT(TYPE Type);
 virtual ~OBJECT();

 virtual void Display() = 0;
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

