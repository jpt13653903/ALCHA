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

#ifndef Keywords_h
#define Keywords_h
//------------------------------------------------------------------------------

#include "Tree.h"
#include "MyString.h"
//------------------------------------------------------------------------------

enum KEYWORD_CODE{
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
 kClock        ,
 kTask         ,
 kLoop         ,
 kPipe         ,
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
  KEYWORD_CODE Code;
  STRING       Identifier; // Key

  KEYWORD_NODE* Next; // Linked list used for reverse lookup

  KEYWORD_NODE();
 ~KEYWORD_NODE(){}

  int Compare(TREE_NODE* Right);
};
//------------------------------------------------------------------------------

class KEYWORDS: protected TREE{
 private:
  KEYWORD_NODE* List; // Linked list used for reverse lookup

  void Add(KEYWORD_CODE Code, const char* Identifier);

 public:
  KEYWORDS();
 ~KEYWORDS(){}

  KEYWORD_CODE GetCode(const char* Identifier); // O(log(N))

  // Reverse look-up: used to display errors; runs in O(N)
  bool GetName(KEYWORD_CODE Code, STRING& Identifier);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
