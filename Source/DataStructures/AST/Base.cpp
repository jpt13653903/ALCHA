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

#include "Base.h"
//------------------------------------------------------------------------------

using namespace AST;
//------------------------------------------------------------------------------

Base::Base(int line, const char* filename, Type type)
{
    source.line     = line;
    source.filename = filename;
    this->type      = type;
    this->next      = 0;
    this->prev      = 0;
}
//------------------------------------------------------------------------------

Base::~Base()
{
    // The list might be too long for a recursive formulation.
    Base* temp;
    while(next){
        temp       = next;
        next       = temp->next;
        temp->next = 0;
        delete temp;
    }
}
//------------------------------------------------------------------------------

bool Base::isAssignment()
{
    return false;
}
//------------------------------------------------------------------------------

bool Base::isDefinition()
{
    return false;
}
//------------------------------------------------------------------------------

bool Base::isExpression()
{
    return false;
}
//------------------------------------------------------------------------------

void Base::printError(const char* message)
{
    ::printError(source.line, source.filename.c_str(), message);
}
//------------------------------------------------------------------------------

void Base::printWarning(const char* message)
{
    ::printWarning(source.line, source.filename.c_str(), message);
}
//------------------------------------------------------------------------------

Base* Base::copyList(Base* source)
{
    Base* node   = 0;
    Base* tail   = 0;
    Base* result = 0;

    while(source){
        node = source->copy();
        if(tail){
            tail->next = node;
            node->prev = tail;
        }else{
            result = node;
        }
        tail   = node;
        source = source->next;
    }

    return result;
}
//------------------------------------------------------------------------------

void Base::displayInfo()
{
    logger.print("\n" ANSI_FG_BRIGHT_BLACK "%s:", source.filename.c_str());
    logger.print(ANSI_FG_CYAN "%05d" ANSI_FG_YELLOW " -- " ANSI_RESET, source.line);
}
//------------------------------------------------------------------------------

void Base::validate()
{
    assert(prev == 0);

    Base* node = this;
    while(node){
        if(node->next) assert(node->next->prev == node,
                              info("type = %d, line = %d, File = %s",
                                   (int)node->type,
                                   node->source.line,
                                   node->source.filename.c_str()));
        if(node->prev) assert(node->prev->next == node,
                              info("type = %d, line = %d, File = %s",
                                   (int)node->type,
                                   node->source.line,
                                   node->source.filename.c_str()));
        node->validateMembers();
        node = node->next;
    }
}
//------------------------------------------------------------------------------

