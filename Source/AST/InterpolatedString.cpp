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

#include "InterpolatedString.h"
#include "AST_String.h"
#include "Stringify.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

InterpolatedString::InterpolatedString(int line, int filenameIndex):
    AST(line, filenameIndex, Type::InterpolatedString)
{
}
//------------------------------------------------------------------------------

InterpolatedString::~InterpolatedString()
{
    if(sequence) delete sequence;
}
//------------------------------------------------------------------------------

std::string InterpolatedString::print(int indent) const
{
    string result;

    for(int n = 0; n < indent; n++) result += "    ";

    result += "$\"";
    auto part = sequence;
    while(part){
        switch(part->type){
            case Type::String:
                result += ((String*)part)->data;
                if(part->next) result += "{";
                break;

            case Type::Stringify:{
                auto injection = (Stringify*)part;
                result += injection->expression->print();
                if(injection->format){
                    result += ", ";
                    result += injection->format->print();
                }
                result += "}";
                break;
            }
            default:
                result += "(Invalid interpolated string member)";
                break;
        }
        part = part->next;
    }
    result += "\"";

    return result;
}
//------------------------------------------------------------------------------

