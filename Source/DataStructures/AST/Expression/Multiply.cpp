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

#include "Multiply.h"
#include "Literal.h"
#include "Object.h"

#include "Netlist/Namespace/Module.h"
#include "Netlist/Synthesisable/Net.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

MULTIPLY::MULTIPLY(int Line, const string& Filename): MULTIPLY(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

MULTIPLY::MULTIPLY(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Multiply){
}
//------------------------------------------------------------------------------

MULTIPLY::~MULTIPLY(){
}
//------------------------------------------------------------------------------

BASE* MULTIPLY::Copy(){
  MULTIPLY* Copy = new MULTIPLY(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool MULTIPLY::GetVerilog(string& Body){
  assert(Left , return false);
  assert(Right, return false);

  if(!Left->GetSigned() && Right->GetSigned()){
    Body += "$signed({1'b0, (";
    Left->GetVerilog(Body);
    Body += ")})";
  }else{
    Body += "(";
    Left->GetVerilog(Body);
    Body += ")";
  }
  Body += " * ";
  if(Left->GetSigned() && !Right->GetSigned()){
    Body += "$signed({1'b0, (";
    Right->GetVerilog(Body);
    Body += ")})";
  }else{
    Body += "(";
    Right->GetVerilog(Body);
    Body += ")";
  }

  return true;
}
//------------------------------------------------------------------------------

EXPRESSION* MULTIPLY::Evaluate(){
  assert(Left , return this);
  assert(Right, return this);

  Left  = Left ->Evaluate();
  Right = Right->Evaluate();

  assert(Left , return this);
  assert(Right, return this);

  if(Left->Type == TYPE::Literal && Right->Type == TYPE::Literal){
    auto Result = new LITERAL(Source.Line, Source.Filename);
    auto left  = (LITERAL*)Left;
    auto right = (LITERAL*)Right;
    Result->Value =   left ->Value;
    Result->Value.Mul(right->Value);
    delete this;
    return Result;
  }

  // Replace a object * object with a wire
  if(Left->Type == TYPE::Object && Right->Type == TYPE::Object){
    auto left  = ((OBJECT*)Left )->ObjectRef;
    auto right = ((OBJECT*)Right)->ObjectRef;

    assert(left );
    assert(right);

    auto Object = new OBJECT      (Source.Line, Source.Filename);
    auto Net    = new NETLIST::NET(Source.Line, Source.Filename, 0);
    Object->ObjectRef = Net;

    NUMBER FullScale = left->FullScale();
    FullScale.Mul(right->FullScale());

    if(left->Signed() && right->Signed()){
      FullScale.BinScale(1); // Make space for the signed bit
      Net->SetFixedPoint(left->Width() + right->Width() + 1, FullScale, true);

    }else if(left->Signed() || right->Signed()){
      Net->SetFixedPoint(left->Width() + right->Width(), FullScale, true);

    }else{
      Net->SetFixedPoint(left->Width() + right->Width(), FullScale, false);
    }
    Net->Value = this;
    NETLIST::NamespaceStack.front()->Symbols[Net->Name] = Net;

    return Object;
  }

  // Put the literal on the right (if there is one)
  if(Left->Type == TYPE::Literal){
    auto Temp = Left;
    Left  = Right;
    Right = Temp;
  }

  if(Left->Type == TYPE::Object && Right->Type == TYPE::Literal){
    auto left  = ((OBJECT *)Left )->ObjectRef;
    auto right =  (LITERAL*)Right;

    assert(left);

    auto Object = new OBJECT      (Source.Line, Source.Filename);
    auto Net    = new NETLIST::NET(Source.Line, Source.Filename, 0);
    Object->ObjectRef = Net;

    if(right->GetSigned()){
      error("not yet implemented");
      delete Object;
      delete Net;
      return this;

    }else{ // Positive literal
      NUMBER FullScale = left->FullScale();
      FullScale.Mul(right->Value);

      Net->SetFixedPoint(left->Width(), FullScale, left->Signed());
      Net->Value = Left;
      Left = 0;
    }
    NETLIST::NamespaceStack.front()->Symbols[Net->Name] = Net;
    delete this;
    return Object;
  }

  return this;
}
//------------------------------------------------------------------------------

int MULTIPLY::GetWidth(){
  assert(Left , return 0);
  assert(Right, return 0);

  return Left->GetWidth() + Right->GetWidth();
}
//------------------------------------------------------------------------------

NUMBER& MULTIPLY::GetFullScale(){
  error("Not yet implemented");
  static NUMBER zero = 0;
  return zero;
}
//------------------------------------------------------------------------------

bool MULTIPLY::GetSigned(){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

bool MULTIPLY::HasCircularReference(NETLIST::BASE* Object){
  assert(Left , return false);
  assert(Right, return false);
  
  if(Left ->HasCircularReference(Object)) return true;
  if(Right->HasCircularReference(Object)) return true;

  return false;
}
//------------------------------------------------------------------------------

void MULTIPLY::PopulateUsed(){
  assert(Left , return);
  assert(Right, return);
  
  Left ->PopulateUsed();
  Right->PopulateUsed();
}
//------------------------------------------------------------------------------

EXPRESSION* MULTIPLY::RemoveTempNet(){
  if(Left ) Left  = Left ->RemoveTempNet();
  if(Right) Right = Right->RemoveTempNet();
  return this;
}
//------------------------------------------------------------------------------

void MULTIPLY::Display(){
  DisplayStart();

  Debug.Print(" * " );

  DisplayEnd();
}
//------------------------------------------------------------------------------

void MULTIPLY::ValidateMembers(){
  assert(Type == TYPE::Multiply);

  assert(!Next);
  assert(!Prev);

  assert(Left , return); Left ->Validate();
  assert(Right, return); Right->Validate();
}
//------------------------------------------------------------------------------

