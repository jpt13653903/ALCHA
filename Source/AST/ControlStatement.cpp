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

#include "ControlStatement.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

ControlStatement::ControlStatement(int line, int filenameIndex):
    AST(line, filenameIndex, Type::ControlStatement)
{
}
//------------------------------------------------------------------------------

ControlStatement::~ControlStatement()
{
    if(!expression) delete expression;
    if(!body      ) delete body;
    if(!elseBody  ) delete elseBody;
}
//------------------------------------------------------------------------------

std::string ControlStatement::print(int indent) const
{
    string result;

    for(int n = 0; n < indent; n++) result += "    ";

    switch(statementType){
        case Token::Type::If:      result += "if ";     break;
        case Token::Type::For:     result += "for ";    break;
        case Token::Type::While:   result += "while ";  break;
        case Token::Type::Loop:    result += "loop ";   break;
        case Token::Type::Switch:  result += "switch "; break;
        case Token::Type::Case:    result += "case ";   break;
        case Token::Type::Default: result += "default ";   break;
        default: result += "Unknown control statement ";
    }

    if(expression){
        bool first = true;
        result += "(";
        auto expr = expression;
        while(expr){
            if(!first) result += ", ";
            first = false;
            result += expr->print();
            expr    = expr->next;
        }
        result += ") ";
    }
    if(body){
        result += "{\n";
        auto statement = body;
        while(statement){
            result += statement->print(indent+1);
            result += "\n";
            statement = statement->next;
        }
        for(int n = 0; n < indent; n++) result += "    ";
        result += "}";
    }
    if(elseBody){
        result += " else {\n";
        auto statement = elseBody;
        while(statement){
            result += statement->print(indent+1);
            result += "\n";
            statement = statement->next;
        }
        for(int n = 0; n < indent; n++) result += "    ";
        result += "}";
    }
    return result;
}
//------------------------------------------------------------------------------

