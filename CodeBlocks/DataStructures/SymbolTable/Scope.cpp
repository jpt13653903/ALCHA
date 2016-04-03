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

#include "Scope.h"
//------------------------------------------------------------------------------

SCOPE Scope;
//------------------------------------------------------------------------------

SCOPE::NODE::NODE(){
 Namespace = 0;
 Next      = 0;
}
//------------------------------------------------------------------------------

SCOPE::NODE::~NODE(){
 if(Next) delete Next;
}
//------------------------------------------------------------------------------

SCOPE::SCOPE(){
 NODE* Stack = new NODE;
 Current = &Stack->Symbols;
}
//------------------------------------------------------------------------------

SCOPE::~SCOPE(){
 delete Stack;
}
//------------------------------------------------------------------------------

void SCOPE::New(){
 NODE* Temp = new NODE;
 Temp->Next = Stack;
 Stack      = Temp;
 Current    = &Stack->Symbols;
}
//------------------------------------------------------------------------------

void SCOPE::Push(SYMBOL* Namespace){
 NODE* Temp = new NODE;
 Temp->Next = Stack;
 Stack      = Temp;

 Stack->Namespace = Namespace;
 Current          = Namespace->Members;
}
//------------------------------------------------------------------------------

void SCOPE::Pop(){
 if(!Stack || !Stack->Next) return; // Trying to pop the global scope

 NODE* Temp = Stack;
 Stack      = Temp->Next;
 Temp->Next = 0;
 delete Temp;

 if(Stack->Namespace) Current =  Namespace->Members;
 else                 Current = &Stack    ->Symbols;
}
//------------------------------------------------------------------------------
