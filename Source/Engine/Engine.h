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

1. Processes the AST and produces the object tree (NETLIST::Global).
2. Processes the scripting statements so that the scripting variables become
   constants, thereby making the resulting object tree synthesisable.
------------------------------------------------------------------------------*/

#ifndef Engine_h
#define Engine_h
//------------------------------------------------------------------------------

#include <list>
#include <stack>
//------------------------------------------------------------------------------

#include "Parser.h"
#include "Netlist/Alias.h"
#include "Netlist/Group.h"
#include "Netlist/Module.h"
#include "Netlist/Pin.h"
#include "Netlist/Net.h"
//------------------------------------------------------------------------------

class ENGINE{
  private: // Massages
    PARSER Parser;

    bool error;
    void Error  (AST::BASE* Ast, const char* Message = 0);
    void Warning(AST::BASE* Ast, const char* Message = 0);
  //----------------------------------------------------------------------------

  private: // Internal structures
    std::stack<AST::BASE*> AstStack; // Used for clean-up in the destructor
  //----------------------------------------------------------------------------

  private: // Expression Evaluation
    // Creates a new node that represents the RHS expression.  Also evaluates 
    // scripting variables and expressions on the fly.
    NETLIST::EXPRESSION* Evaluate(AST::EXPRESSION* Node);

    // Populates a list of existing expressions, except when the target is an 
    // undefined attribute, in which case the attribute is created first.
    struct TARGET_LIST{
      NETLIST::BASE*        Object;
      NETLIST::EXPRESSION** Expression;
    };
    typedef std::list<TARGET_LIST> target_list;
    bool    GetLHS_Object(NETLIST::BASE* Object, target_list& List, AST::BASE* Ast);
    bool    GetLHS(AST::EXPRESSION* Node, target_list& List);

    // Simplifies the tree
    void Simplify(NETLIST::EXPRESSION* Root);
  //----------------------------------------------------------------------------

  private: // AST processing
    bool ApplyAttributes(
      NETLIST::BASE*       Object,
      std::string&         Name,
      NETLIST::EXPRESSION* Value,
      AST::BASE*           Ast
    );
    bool ApplyAttributes(NETLIST::BASE* Object, AST::ASSIGNMENT* AttributeList);
    bool ApplyParameters(NETLIST::SYNTHESISABLE* Object, AST::BASE* Parameter);

    bool Import    (AST::IMPORT    * Ast);
    bool Group     (AST::GROUP     * Ast);
    bool Alias     (AST::ALIAS     * Ast);
    bool Definition(AST::DEFINITION* Ast);
    bool Assignment(AST::ASSIGNMENT* Ast);

    bool Run(AST::BASE* Ast);
  //----------------------------------------------------------------------------

  public:
    ENGINE();
   ~ENGINE();

    bool Run(const char* Filename);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

