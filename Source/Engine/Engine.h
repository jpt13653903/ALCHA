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

/* Description

1. Processes the AST and produces the object tree (OBJECTS::Global).
2. Processes the scripting statements so that the scripting variables become
   constants, thereby making the resulting object tree synthesisable.
------------------------------------------------------------------------------*/

#ifndef Engine_h
#define Engine_h
//------------------------------------------------------------------------------

#include "Parser.h"
#include "Objects/Alias.h"
#include "Objects/Namespace.h"
#include "Objects/Pin.h"
//------------------------------------------------------------------------------

class ENGINE{
  private: // Massages
    AST::BASE* Ast;

    bool error;
    void Error  (const char* Message = 0);
    void Warning(const char* Message = 0);

  private: // Expression Evaluation
    OBJECTS::EXPRESSION* Evaluate(AST::EXPRESSION* Node);

  private: // AST processing
    bool ApplyAttributes(
      OBJECTS::BASE*       Object,
      std::string&         Name,
      OBJECTS::EXPRESSION* Value
    );
    bool ApplyAttributes(OBJECTS::BASE* Object, AST::ASSIGNMENT* AttributeList);
    bool ApplyParameters(OBJECTS::SYNTHESISABLE* Object, AST::BASE* Parameter);

    bool Import    (AST::IMPORT    * Ast);
    bool Alias     (AST::ALIAS     * Ast);
    bool Definition(AST::DEFINITION* Ast);
    bool Assignment(AST::ASSIGNMENT* Ast);

  public:
    ENGINE();
   ~ENGINE();

    bool Run();
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

