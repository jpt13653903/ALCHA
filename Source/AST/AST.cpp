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

#include "AST.h"
#include "Utilities.h"
//------------------------------------------------------------------------------

using std::string;
//------------------------------------------------------------------------------

std::vector<std::string> AST::filenameBuffer;
//------------------------------------------------------------------------------

AST::AST::AST(int line, int filenameIndex, Type type)
{
    this->line          = line;
    this->filenameIndex = filenameIndex;
    this->type          = type;
}
//------------------------------------------------------------------------------

AST::AST::~AST()
{
    if(next) delete next;
}
//------------------------------------------------------------------------------

const char* AST::AST::decodeType() const
{
    switch(type){
        case Type::AccessDirectionGroup: return "AccessDirectionGroup";
        case Type::Assignment:           return "Assignment";
        case Type::Concatenate:          return "Concatenate";
        case Type::ClassDefinition:      return "ClassDefinition";
        case Type::Expression:           return "Expression";
        case Type::EnumDefinition:       return "EnumDefinition";
        case Type::FunctionCall:         return "FunctionCall";
        case Type::FunctionDef:          return "FunctionDef";
        case Type::Identifier:           return "Identifier";
        case Type::Label:                return "Label";
        case Type::Literal:              return "Literal";
        case Type::NameSpacePush:        return "NameSpacePush";
        case Type::OperatorOverload:     return "OperatorOverload";
        case Type::ParameterDef:         return "ParameterDef";
        case Type::Slice:                return "Slice";
        case Type::String:               return "String";
        case Type::Stringify:            return "Stringify";
        case Type::VariableDef:          return "VariableDef";

        default: return "Invalid Type Index";
    }
}
//------------------------------------------------------------------------------

