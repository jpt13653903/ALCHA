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

#include "Subtract.h"
#include "Literal.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

SUBTRACT::SUBTRACT(int Line, const string& Filename): SUBTRACT(Line, Filename.c_str())
{}
//------------------------------------------------------------------------------

SUBTRACT::SUBTRACT(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Subtract)
{
}
//------------------------------------------------------------------------------

SUBTRACT::~SUBTRACT()
{
}
//------------------------------------------------------------------------------

BASE* SUBTRACT::Copy()
{
    SUBTRACT* Copy = new SUBTRACT(Source.Line, Source.Filename.c_str());

    if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
    if(Right) Copy->Right = (decltype(Right))Right->Copy();

    return Copy;
}
//------------------------------------------------------------------------------

bool SUBTRACT::GetVerilog(string& Body)
{
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
    Body += " - ";
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

EXPRESSION* SUBTRACT::SubtractLiteral(EXPRESSION* Object, EXPRESSION* Literal)
{
    ResultWidth     = Object->GetWidth();
    ResultFullScale = Object->GetFullScale();

    NUMBER Scale = 1;
    Scale.BinScale(ResultWidth);
    Scale.Div(ResultFullScale);
    ((LITERAL*)Literal)->Value.Mul(Scale);

    int LiteralWidth = Literal->GetWidth();

    if(LiteralWidth > ResultWidth){
        ResultFullScale.BinScale(LiteralWidth - ResultWidth);
        ResultWidth = LiteralWidth;
    }
    // Make space for the overflow
    ResultFullScale.BinScale(1);
    ResultWidth++;

    return MakeObject();
}
//------------------------------------------------------------------------------

EXPRESSION* SUBTRACT::Evaluate()
{
    assert(Left , return this);
    assert(Right, return this);

    Left  = Left ->Evaluate();
    Right = Right->Evaluate();

    assert(Left , return this);
    assert(Right, return this);

    ResultSigned = (Left->GetSigned() || Right->GetSigned());

    if(Left->Type == TYPE::Literal && Right->Type == TYPE::Literal){
        auto Result = new LITERAL(Source.Line, Source.Filename);
        auto left  = (LITERAL*)Left;
        auto right = (LITERAL*)Right;
        Result->Value =   left ->Value;
        Result->Value.Sub(right->Value);
        delete this;
        return Result;
    }

    if(Left->Type == TYPE::Literal && Right->Type == TYPE::Object){
        return SubtractLiteral(Right, Left);

    }else if(Left->Type == TYPE::Object && Right->Type == TYPE::Literal){
        return SubtractLiteral(Left, Right);

    }else if(Left->Type == TYPE::Object && Right->Type == TYPE::Object){
        int LeftWidth  = Left ->GetWidth();
        int RightWidth = Right->GetWidth();

        NUMBER LeftFullScale  = Left ->GetFullScale();
        NUMBER RightFullScale = Right->GetFullScale();

        // The numerator if choosing to scale the left
        NUMBER Scale1 = LeftFullScale;
        Scale1.BinScale(RightWidth);

        // The numerator if choosing to scale the right
        NUMBER Scale2 = RightFullScale;
        Scale2.BinScale(LeftWidth);

        if(Scale1 > Scale2){ // Scale the left
            Scale1.Div(Scale2);
            Left = Left->ScaleWith(Scale1, RightWidth, RightFullScale);
            LeftWidth = Left->GetWidth();

            ResultWidth     = RightWidth;
            ResultFullScale = RightFullScale;

            if(LeftWidth > ResultWidth){
                ResultFullScale.BinScale(LeftWidth - ResultWidth);
                ResultWidth = LeftWidth;
            }

        }else{ // Scale the right
            Scale2.Div(Scale1);
            Right = Right->ScaleWith(Scale2, LeftWidth, LeftFullScale);
            RightWidth = Right->GetWidth();

            ResultWidth     = LeftWidth;
            ResultFullScale = LeftFullScale;

            if(RightWidth > ResultWidth){
                ResultFullScale.BinScale(RightWidth - ResultWidth);
                ResultWidth = RightWidth;
            }
        }

        // Make space for the overflow
        ResultFullScale.BinScale(1);
        ResultWidth++;

        return MakeObject();
    }

    return this;
}
//------------------------------------------------------------------------------

int SUBTRACT::GetWidth()
{
    return ResultWidth;
}
//------------------------------------------------------------------------------

NUMBER& SUBTRACT::GetFullScale()
{
    return ResultFullScale;
}
//------------------------------------------------------------------------------

bool SUBTRACT::GetSigned()
{
    return ResultSigned;
}
//------------------------------------------------------------------------------

bool SUBTRACT::HasCircularReference(NETLIST::BASE* Object)
{
    assert(Left , return false);
    assert(Right, return false);

    if(Left ->HasCircularReference(Object)) return true;
    if(Right->HasCircularReference(Object)) return true;

    return false;
}
//------------------------------------------------------------------------------

void SUBTRACT::PopulateUsed()
{
    assert(Left , return);
    assert(Right, return);

    Left ->PopulateUsed();
    Right->PopulateUsed();
}
//------------------------------------------------------------------------------

EXPRESSION* SUBTRACT::RemoveTempNet(int Width, bool Signed)
{
    if(Left ) Left  = Left ->RemoveTempNet(0, false);
    if(Right) Right = Right->RemoveTempNet(0, false);
    return this;
}
//------------------------------------------------------------------------------

void SUBTRACT::Display()
{
    DisplayStart();

    Debug.Print(" - " );

    DisplayEnd();
}
//------------------------------------------------------------------------------

void SUBTRACT::ValidateMembers()
{
    assert(Type == TYPE::Subtract);

    assert(!Next);
    assert(!Prev);

    assert(Left , return); Left ->Validate();
    assert(Right, return); Right->Validate();
}
//------------------------------------------------------------------------------

