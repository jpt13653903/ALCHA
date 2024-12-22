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

#include "Jump.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

JUMP::JUMP(int Line, std::string& Filename, JUMP_TYPE JumpType): JUMP(Line, Filename.c_str(), JumpType)
{}
//------------------------------------------------------------------------------

JUMP::JUMP(int Line, const char* Filename, JUMP_TYPE JumpType): BASE(Line, Filename, TYPE::Jump)
{
    this->JumpType = JumpType;

    Expression = 0;
}
//------------------------------------------------------------------------------

JUMP::~JUMP()
{
    if(Expression) delete Expression;
}
//------------------------------------------------------------------------------

BASE* JUMP::Copy()
{
    JUMP* Copy = new JUMP(Source.Line, Source.Filename.c_str(), JumpType);

    if(Expression) Copy->Expression = (decltype(Expression))Expression->Copy();

    return Copy;
}
//------------------------------------------------------------------------------

bool JUMP::RunAST()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool JUMP::GetVerilog(string& Body)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void JUMP::Display()
{
    DisplayInfo();
    Debug.Print("jump(");
    switch(JumpType){
        case JUMP_TYPE::Return  : Debug.Print("return) "           ); break;
        case JUMP_TYPE::Break   : Debug.Print("break) "            ); break;
        case JUMP_TYPE::Continue: Debug.Print("continue) "         ); break;
        default                 : Debug.Print("Unknown jump type) "); break;
    }
    if(Expression) Expression->Display();
    else           Debug.Print("{default}");
    Debug.Print("\n");

    if(Next) Next->Display();
}
//------------------------------------------------------------------------------

void JUMP::ValidateMembers()
{
    assert(Type == TYPE::Jump);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

