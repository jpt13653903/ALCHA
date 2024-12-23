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

using std::string;
using namespace AST;
//------------------------------------------------------------------------------

Cast::Cast(int line, const string& filename): Cast(line, filename.c_str()){}
//------------------------------------------------------------------------------

Cast::Cast(int line, const char* filename): Expression(line, filename, Type::Cast){}
//------------------------------------------------------------------------------

Cast::~Cast(){}
//------------------------------------------------------------------------------

Base* Cast::copy()
{
    Cast* copy = new Cast(source.line, source.filename.c_str());

    if(left ) copy->left  = (decltype(left ))left ->copy();
    if(right) copy->right = (decltype(right))right->copy();

    return copy;
}
//------------------------------------------------------------------------------

bool Cast::getVerilog(string& body)
{
    // TODO: Move to new strategy of synthesising single operations into temporaries
    error("Not yet implemented");
    // assert( expression->left , return false);
    // assert(!expression->right, return false);
    // AST::Expression* from = expression->left;
    // AST::Expression* to   = expression;
    // Number factor = from->fullScale;
    // factor.div(to->fullScale);
    // factor.binScale(to->width - from->width);
    // assert(factor != 0, expression->display(); return false);

    // // Calculate the limit of the inferred multiplier size.  Most FPGAs have
    // // 18-bit multipliers, so make that the minimum limit, otherwise use the
    // // target width as the limit so that no to little resolution is lost.
    // Number limit(1);
    // if(to->width < 18) limit.binScale(18);
    // else               limit.binScale(to->width);
    // int shift = 0;
    // while(factor.isInt()){
    //   factor.binScale(-1);
    //   shift--;
    // }
    // while(!factor.isInt() && (factor < limit)){
    //   factor.binScale(1);
    //   shift++;
    // }
    // while(factor >= limit){
    //   factor.binScale(-1);
    //   shift--;
    // }
    // Number fullFactor(factor);
    // factor.round();
    // if(factor != fullFactor){
    //   printWarning(expression, "Rounding the scaling factor - this can be fixed "
    //                       "with an explicit scaling multiplication.");
    //   while(factor.isInt()){ // Make sure it's still minimised after rounding
    //     factor.binScale(-1);
    //     shift--;
    //   }
    //   while(!factor.isInt()){
    //     factor.binScale(1);
    //     shift++;
    //   }
    // }

    // int width = 0;
    // Number num(factor);
    // while(num >= 1){
    //   num.binScale(-1);
    //   width++;
    // }

    // string fromString;
    // if(!buildExpression(body, from, fromString)) return false;

    // if(factor == 1){
    //   body += "wire ";
    //   if(to->width > 1){
    //     if(to->signed) body += "["+ to_string(to->width  ) +":0] ";
    //     else           body += "["+ to_string(to->width-1) +":0] ";
    //   }
    //   wire = getWireName();
    //   body += wire +"= ";
    //   if     (shift > 0) body += fromString +" >> "+ to_string( shift);
    //   else if(shift < 0) body += fromString +" << "+ to_string(-shift);
    //   body += ";\n";

    // }else{
    //   printWarning(expression, "Non power-of-two scaling factor: synthesising a multiplier");
    //   string mulWireName = getWireName();

    //   // TODO: signed
    //   body += "wire ["+ to_string(from->width + width - 1) +":0] ";
    //   body += mulWireName +"= "+ fromString + " * ";

    //   body += to_string(width) + "'h";
    //   body += factor.getString(16);
    //   body += ";\n";

    //   body += "wire ";
    //   if(to->width > 1){
    //     if(to->signed) body += "["+ to_string(to->width  ) +":0] ";
    //     else           body += "["+ to_string(to->width-1) +":0] ";
    //   }
    //   wire = getWireName();
    //   body += wire +"= ";
    //   if     (shift > 0) body += mulWireName +" >> "+ to_string( shift);
    //   else if(shift < 0) body += mulWireName +" << "+ to_string(-shift);
    //   body += ";\n";
    // }

    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

Expression* Cast::evaluate()
{
    error("Not yet implemented");
    return this;
//   Expression* result = 0;
//
//   error("Not yet implemented");
//
//   if(!result) return 0;
//   return result->simplify(false);
}
//------------------------------------------------------------------------------

int Cast::getWidth()
{
    error("Not yet implemented");
    return 0;
}
//------------------------------------------------------------------------------

Number& Cast::getFullScale()
{
    error("Not yet implemented");
    static Number zero = 0;
    return zero;
}
//------------------------------------------------------------------------------

bool Cast::getSigned()
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

bool Cast::hasCircularReference(Netlist::Base* object)
{
    error("Not yet implemented");
    return false;
}
//------------------------------------------------------------------------------

void Cast::populateUsed()
{
    error("Not yet implemented");
}
//------------------------------------------------------------------------------

Expression* Cast::removeTempNet(int width, bool isSigned)
{
    error("Not yet implemented");
    return this;
}
//------------------------------------------------------------------------------

void Cast::display()
{
    displayStart();

    logger.print(" {cast} ");

    displayEnd();
}
//------------------------------------------------------------------------------

void Cast::validateMembers()
{
    assert(type == Type::Cast);

    assert(!next);
    assert(!prev);

    // TODO: assert(!left );
    // TODO: assert(!right);

    error("Not yet implemented");
}
//------------------------------------------------------------------------------

