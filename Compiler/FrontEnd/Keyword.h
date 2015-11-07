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

#ifndef Keyword_h
#define Keyword_h
//------------------------------------------------------------------------------

#include "Tree.h"
#include "MyString.h"
//------------------------------------------------------------------------------

enum KEYWORD_VALUE{
 kUnknown = 0,

 // Inherited from C++
 kContinue ,
 kTrue     ,
 kGoto     ,
 kDefault  ,
 kIf       ,
 kReturn   ,
 kTypedef  ,
 kInline   ,
 kShort    ,
 kBool     ,
 kDo       ,
 kInt      ,
 kSigned   ,
 kBreak    ,
 kDouble   ,
 kLong     ,
 kSizeof   ,
 kUnion    ,
 kCase     ,
 kStatic   ,
 kUnsigned ,
 kElse     ,
 kChar     ,
 kEnum     ,
 kVirtual  ,
 kStruct   ,
 kVoid     ,
 kSwitch   ,
 kClass    ,
 kExtern   ,
 kOperator ,
 kConst    ,
 kFalse    ,
 kPrivate  ,
 kWhile    ,
 kFloat    ,
 kProtected,
 kFor      ,
 kPublic   ,

 // ALCHA-specific
 kPin          ,
 kPinGroup     ,
 kPinArray     ,
 kBus          ,
 kClock        ,
 kReset        ,
 kTask         ,
 kClockEnable  ,
 kLoop         ,
 kPipe         ,
 kString       ,
 kParam        ,
 kScript       ,
 kRtl          ,
 kIn           ,
 kOut          ,
 kExternalSetup,
 kExternalHold ,
 kExternalDelay,
 kTarget
};
//------------------------------------------------------------------------------

struct KEYWORD_NODE: public TREE_NODE{
  KEYWORD_VALUE Value;
  STRING        Identifier; // Key

  KEYWORD_NODE();
 ~KEYWORD_NODE(){}

  int Compare(TREE_NODE* Right);
};
//------------------------------------------------------------------------------

class KEYWORD: protected TREE{
 private:
  void Add(KEYWORD_VALUE Value, const char* Identifier);

 public:
  KEYWORD();
 ~KEYWORD(){}

  int GetCode(const char* Identifier);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
