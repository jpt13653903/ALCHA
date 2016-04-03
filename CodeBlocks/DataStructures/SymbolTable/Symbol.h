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

#ifndef Symbol_h
#define Symbol_h
//------------------------------------------------------------------------------

#include "AST.h"
#include "Number.h"
#include "SymbolTree.h"
//------------------------------------------------------------------------------

struct SYMBOL{ // Base class for the symbol table
 enum TYPE{
  Pin, Sig, Clk,            // Physical signals
  Int, Rat, Float, Complex, // Scripting variables
  Array,                    // Array of...
  Class,                    // Class type
  ClassInstance,            // Class instance
  Function,                 // Actual function, with an AST as a body
  FuncPtr,                  // Function pointer
  Namespace                 // Namespace of an imported file
 } Type;

 byte* Name; // ID obtained via IdentifierTree

 unsigned Length; // bit count in Pin and Sig; precision in Float and Complex;
                  // number of elements in Array.
 NUMBER   Value;

 AST_Base*   Body;    // The executable body of the class or function
 SYMBOL_TREE Members; // The member symbols of the class, function or name-space
 SYMBOL*     SubType; // The elements of the array; actual function pointer to
                      // by FuncPtr.

 SYMBOL(TYPE Type, byte* Name);
~SYMBOL();
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

