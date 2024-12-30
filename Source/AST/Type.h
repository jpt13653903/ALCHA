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

#ifndef AST_Type_h
#define AST_Type_h
//------------------------------------------------------------------------------

namespace AST{
    enum class Type{
        AccessDirectionGroup,
        Alias,
        Array,
        Assert,
        Assignment,
        ClassDefinition,
        ClockedConstruct,
        Concatenate,
        ControlStatement,
        EnumDefinition,
        Expression,
        Fence,
        ForkJoin,
        FunctionCall,
        FunctionDef,
        GroupDefinition,
        HdlConstruct,
        Identifier,
        Import,
        InterpolatedString,
        Jump,
        Label,
        Literal,
        NameSpacePush,
        OperatorOverload,
        ParameterDef,
        Slice,
        StimulusOrEmulate,
        String,
        Stringify,
        VariableDef,
        Wait
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
