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

BASE::BASE(int Line, const char* Filename, TYPE Type){
  Source.Line     = Line;
  Source.Filename = Filename;
  this->Type      = Type;
  this->Next      = 0;
  this->Prev      = 0;
}
//------------------------------------------------------------------------------

BASE::~BASE(){
  // The list might be too long for a recursive formulation.
  BASE* Temp;
  while(Next){
    Temp       = Next;
    Next       = Temp->Next;
    Temp->Next = 0;
    delete Temp;
  }
}
//------------------------------------------------------------------------------

bool BASE::IsAssignment(){
  return false;
}
//------------------------------------------------------------------------------

bool BASE::IsDefinition(){
  return false;
}
//------------------------------------------------------------------------------

bool BASE::IsExpression(){
  return false;
}
//------------------------------------------------------------------------------

void BASE::Error(const char* Message){
  ::Error(Source.Line, Source.Filename.c_str(), Message);
}
//------------------------------------------------------------------------------

void BASE::Warning(const char* Message){
  ::Warning(Source.Line, Source.Filename.c_str(), Message);
}
//------------------------------------------------------------------------------

BASE* BASE::CopyList(BASE* Source){
  BASE* Node   = 0;
  BASE* Tail   = 0;
  BASE* Result = 0;

  while(Source){
    Node = Source->Copy();
    if(Tail){
      Tail->Next = Node;
      Node->Prev = Tail;
    }else{
      Result = Node;
    }
    Tail   = Node;
    Source = Source->Next;
  }

  return Result;
}
//------------------------------------------------------------------------------

void BASE::DisplayInfo(){
  Debug.Print("\n" ANSI_FG_BRIGHT_BLACK "%s:", Source.Filename.c_str());
  Debug.Print(ANSI_FG_CYAN "%05d" ANSI_FG_YELLOW " -- " ANSI_RESET, Source.Line);
}
//------------------------------------------------------------------------------

void BASE::Validate(){
  assert(Prev == 0);

  BASE* Node = this;
  while(Node){
    if(Node->Next) assert(Node->Next->Prev == Node,
                          info("Type = %d, Line = %d, File = %s",
                               (int)Node->Type,
                               Node->Source.Line,
                               Node->Source.Filename.c_str()));
    if(Node->Prev) assert(Node->Prev->Next == Node,
                          info("Type = %d, Line = %d, File = %s",
                               (int)Node->Type,
                               Node->Source.Line,
                               Node->Source.Filename.c_str()));
    Node->ValidateMembers();
    Node = Node->Next;
  }
}
//------------------------------------------------------------------------------

