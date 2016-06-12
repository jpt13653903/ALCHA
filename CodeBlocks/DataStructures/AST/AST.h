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

#ifndef AST_h
#define AST_h
//------------------------------------------------------------------------------

#include "MyString.h"
#include "Dictionary.h"
#include "IdentifierTree.h"
//------------------------------------------------------------------------------

struct AST_Base{ // The base type for AST nodes
 enum TYPE{
  Fence, // Empty statement, but also "next-cycle" specifier in FSMs
  Import,
  Group,
  TargetDefinition,
  ClassDefinition,
  EnumDefinition,
  Definition, // pin, net, clk, byte, char, int, rat, float, complex and 
              // class instance
  Parameter,
  Expression,
  Assignment,
  NamespacePush,
  IfStatement,
  ForLoop,
  LoopLoop,
  WhileLoop,
  Switch,
  Jump,
  RTL,
  FSM,
  HDL
 } Type;

 int       Line;
 byte*     Filename; // Memory handled by IdentifierTree
 AST_Base* Next;     // Next sibling

          AST_Base(int Line, const byte* Filename);
 virtual ~AST_Base(); // Also deletes the rest of the linked list

 virtual void Display() = 0;
};
//------------------------------------------------------------------------------

void* AttributesOnDuplicate(const byte* Name, void* Old, void* New);
void  AttributesDisplay    (const byte* Name, void* Data);
void  AttributesDeleteData (const byte* Name, void* Data);
//------------------------------------------------------------------------------

extern AST_Base* AST; // The global AST root
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

