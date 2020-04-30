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

#include "Cast.h"
//------------------------------------------------------------------------------

using namespace std;
using namespace AST;
//------------------------------------------------------------------------------

CAST::CAST(int Line, const string& Filename): CAST(Line, Filename.c_str()){}
//------------------------------------------------------------------------------

CAST::CAST(int Line, const char* Filename): EXPRESSION(Line, Filename, TYPE::Cast){
}
//------------------------------------------------------------------------------

CAST::~CAST(){
}
//------------------------------------------------------------------------------

BASE* CAST::Copy(){
  CAST* Copy = new CAST(Source.Line, Source.Filename.c_str());

  if(Left ) Copy->Left  = (decltype(Left ))Left ->Copy();
  if(Right) Copy->Right = (decltype(Right))Right->Copy();

  return Copy;
}
//------------------------------------------------------------------------------

bool CAST::GetVerilog(string& Body){
  // TODO: Move to new strategy of synthesising single operations into temporaries
  error("Not yet implemented");
  // assert( Expression->Left , return false);
  // assert(!Expression->Right, return false);
  // AST::EXPRESSION* From = Expression->Left;
  // AST::EXPRESSION* To   = Expression;
  // NUMBER Factor = From->FullScale;
  // Factor.Div(To->FullScale);
  // Factor.BinScale(To->Width - From->Width);
  // assert(Factor != 0, Expression->Display(); return false);

  // // Calculate the limit of the inferred multiplier size.  Most FPGAs have 
  // // 18-bit multipliers, so make that the minimum limit, otherwise use the 
  // // target width as the limit so that no to little resolution is lost.
  // NUMBER Limit(1);
  // if(To->Width < 18) Limit.BinScale(18);
  // else               Limit.BinScale(To->Width);
  // int Shift = 0;
  // while(Factor.IsInt()){
  //   Factor.BinScale(-1);
  //   Shift--;
  // }
  // while(!Factor.IsInt() && (Factor < Limit)){
  //   Factor.BinScale(1);
  //   Shift++;
  // }
  // while(Factor >= Limit){
  //   Factor.BinScale(-1);
  //   Shift--;
  // }
  // NUMBER FullFactor(Factor);
  // Factor.Round();
  // if(Factor != FullFactor){
  //   Warning(Expression, "Rounding the scaling factor - this can be fixed "
  //                       "with an explicit scaling multiplication.");
  //   while(Factor.IsInt()){ // Make sure it's still minimised after rounding
  //     Factor.BinScale(-1);
  //     Shift--;
  //   }
  //   while(!Factor.IsInt()){
  //     Factor.BinScale(1);
  //     Shift++;
  //   }
  // }

  // int Width = 0;
  // NUMBER Num(Factor);
  // while(Num >= 1){
  //   Num.BinScale(-1);
  //   Width++;
  // }

  // string FromString;
  // if(!BuildExpression(Body, From, FromString)) return false;

  // if(Factor == 1){
  //   Body += "wire ";
  //   if(To->Width > 1){
  //     if(To->Signed) Body += "["+ to_string(To->Width  ) +":0] ";
  //     else           Body += "["+ to_string(To->Width-1) +":0] ";
  //   }
  //   Wire = GetWireName();
  //   Body += Wire +"= ";
  //   if     (Shift > 0) Body += FromString +" >> "+ to_string( Shift);
  //   else if(Shift < 0) Body += FromString +" << "+ to_string(-Shift);
  //   Body += ";\n";

  // }else{
  //   Warning(Expression, "Non power-of-two scaling factor: synthesising a multiplier");
  //   string MulWireName = GetWireName();

  //   // TODO: Signed
  //   Body += "wire ["+ to_string(From->Width + Width - 1) +":0] ";
  //   Body += MulWireName +"= "+ FromString + " * ";

  //   Body += to_string(Width) + "'h";
  //   Body += Factor.GetString(16);
  //   Body += ";\n";

  //   Body += "wire ";
  //   if(To->Width > 1){
  //     if(To->Signed) Body += "["+ to_string(To->Width  ) +":0] ";
  //     else           Body += "["+ to_string(To->Width-1) +":0] ";
  //   }
  //   Wire = GetWireName();
  //   Body += Wire +"= ";
  //   if     (Shift > 0) Body += MulWireName +" >> "+ to_string( Shift);
  //   else if(Shift < 0) Body += MulWireName +" << "+ to_string(-Shift);
  //   Body += ";\n";
  // }

  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

EXPRESSION* CAST::Evaluate(){
  error("Not yet implemented");
  return this;
//   EXPRESSION* Result = 0;
// 
//   error("Not yet implemented");
// 
//   if(!Result) return 0;
//   return Result->Simplify(false);
}
//------------------------------------------------------------------------------

int CAST::GetWidth(){
  error("Not yet implemented");
  return 0;
}
//------------------------------------------------------------------------------

EXPRESSION* CAST::FixedPointScale(int Width, NUMBER& FullScale){
  error("Not yet implemented");
  return this;
}
//------------------------------------------------------------------------------

bool CAST::HasCircularReference(NETLIST::BASE* Object){
  error("Not yet implemented");
  return false;
}
//------------------------------------------------------------------------------

// EXPRESSION* CAST::Simplify(bool GenWire){
//   // TODO: Root has the target type; Left has the original expression
//   //       Right is optional and carry the class name (when applicable)
//   assert(Left, return this);
// 
//   Left = Left->Simplify(true);
//   if(Right){
//     Right = Right->Simplify(true);
//   }
// 
//   error("Not yet implemented");
//   return this;
// }
//------------------------------------------------------------------------------

void CAST::Display(){
  DisplayStart();

  Debug.Print(" {cast} ");

  DisplayEnd();
}
//------------------------------------------------------------------------------

void CAST::ValidateMembers(){
  assert(Type == TYPE::Cast);

  assert(!Next);
  assert(!Prev);

  // TODO: assert(!Left );
  // TODO: assert(!Right);

  error("Not yet implemented");
}
//------------------------------------------------------------------------------

