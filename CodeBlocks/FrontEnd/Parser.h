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

#ifndef Parser_h
#define Parser_h
//------------------------------------------------------------------------------

#include "Scanner.h"
#include "Dictionary.h"

#include "AST_Definition.h"
#include "AST_TargetDefinition.h"
//------------------------------------------------------------------------------

class PARSER{
 private:
  bool error;
  void Error(const char* Message);

  TOKEN    Token;    // The current token
  SCANNER* Scanner;  // The scanner (linked-list based stack)

  void GetToken();

  bool AttributeAssignment(DICTIONARY* Attributes);
  bool AttributeList      (DICTIONARY* Attributes);

  AST_Expression* Literal      ();
  AST_Expression* Identifier   ();
  AST_Expression* DotExpression();
  AST_Expression* Primary      ();
  AST_Expression* Expression   ();

  AST_Definition::ARRAY* ArrayDefinition();
  AST_Definition*        IdentifierList (AST_Definition* Node);

  AST_TargetDefinition* TargetDefinition();
  AST_Definition      * Definition();

 public:
  PARSER();
 ~PARSER();

  AST_Base* Run(const byte* Filename);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
