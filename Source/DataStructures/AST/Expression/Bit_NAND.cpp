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

#include "Bit_NAND.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

BIT_NAND::BIT_NAND(int Line, const string& Filename): BIT_NAND(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

BIT_NAND::BIT_NAND(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Bit_NAND){
}
//------------------------------------------------------------------------------

BIT_NAND::~BIT_NAND(){
}
//------------------------------------------------------------------------------

BASE* BIT_NAND::Copy(bool CopyNext){
  BIT_NAND* Copy = new BIT_NAND(Line, Filename.c_str());

  Copy->Value     = Value;
  Copy->StrValue  = StrValue;

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy(CopyNext);
  if(Right) Copy->Right = (decltype(Right))Right->Copy(CopyNext);

  if(CopyNext && Next) Copy->Next = Next->Copy(CopyNext);

  return Copy;
}
//------------------------------------------------------------------------------

bool BIT_NAND::RunScripting(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* BIT_NAND::Evaluate(){
  EXPRESSION* Result = 0;

  error("Not yet implemented");

  if(!Result) return 0;
  return Result->Simplify();
}
//------------------------------------------------------------------------------

EXPRESSION* BIT_NAND::Simplify(){
  assert(Left && Right, return this);
  assert(Right->Type > TYPE::Expression, return this);

  Left = Left->Simplify();
  Right = ((EXPRESSION*)Right)->Simplify();

  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

void BIT_NAND::Display(){
  DisplayStart();

  Debug.print(" ~& ");

  DisplayEnd();
}
//------------------------------------------------------------------------------
