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

#include "Wait.h"
//------------------------------------------------------------------------------

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Wait::Wait(int line, int filenameIndex):
    AST(line, filenameIndex, Type::Wait)
{
}
//------------------------------------------------------------------------------

Wait::~Wait()
{
    switch(waitType){
        case Token::Type::WaitUntil:
            if(!sequence) delete sequence;
            break;

        case Token::Type::WaitOn:
            if(!sensitivityList) delete sensitivityList;
            break;

        case Token::Type::WaitFor:
        case Token::Type::WaitCycles:
            if(!expression) delete expression;
            break;

        default:
            break;
    }
}
//------------------------------------------------------------------------------

Wait::SensitivityItem::~SensitivityItem()
{
    if(!item) delete item;
    if(!next) delete next;
}
//------------------------------------------------------------------------------

std::string Wait::print(int indent) const
{
    string result;

    for(int n = 0; n < indent; n++) result += "    ";

    switch(waitType){
        case Token::Type::WaitUntil:
            result += "wait (";
            result += sequence->print();
            result += ")";
            break;

        case Token::Type::WaitOn:{
            result += "@(";
            bool first = true;
            auto item = sensitivityList;
            while(item){
                if(!first) result += ", ";
                first = false;
                switch(item->edge){
                    case Token::Type::PosEdge: result += "posedge "; break;
                    case Token::Type::NegEdge: result += "negedge "; break;
                    default: break;
                }
                result += item->item->print();
                item = item->next;
            }
            result += ")";
            break;
        }

        case Token::Type::WaitFor:
            result += "#" + expression->print();;
            break;

        case Token::Type::WaitCycles:
            result += "##" + expression->print();;
            break;

        default:
            break;
    }
    return result;
}
//------------------------------------------------------------------------------

