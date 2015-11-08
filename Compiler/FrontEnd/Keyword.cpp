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

#include "Keyword.h"
//------------------------------------------------------------------------------

KEYWORD_NODE::KEYWORD_NODE(){
 Code = kUnknown;
}
//------------------------------------------------------------------------------

int KEYWORD_NODE::Compare(TREE_NODE* Right){
 return Identifier.Compare(((KEYWORD_NODE*)Right)->Identifier);
}
//------------------------------------------------------------------------------

KEYWORD::KEYWORD(){
 List = 0;

 Add(kContinue , "continue" );
 Add(kTrue     , "true"     );
 Add(kGoto     , "goto"     );
 Add(kDefault  , "default"  );
 Add(kIf       , "if"       );
 Add(kReturn   , "return"   );
 Add(kTypedef  , "typedef"  );
 Add(kInline   , "inline"   );
 Add(kShort    , "short"    );
 Add(kBool     , "bool"     );
 Add(kDo       , "do"       );
 Add(kInt      , "int"      );
 Add(kSigned   , "signed"   );
 Add(kBreak    , "break"    );
 Add(kDouble   , "double"   );
 Add(kLong     , "long"     );
 Add(kSizeof   , "sizeof"   );
 Add(kUnion    , "union"    );
 Add(kCase     , "case"     );
 Add(kStatic   , "static"   );
 Add(kUnsigned , "unsigned" );
 Add(kElse     , "else"     );
 Add(kChar     , "char"     );
 Add(kEnum     , "enum"     );
 Add(kVirtual  , "virtual"  );
 Add(kStruct   , "struct"   );
 Add(kVoid     , "void"     );
 Add(kSwitch   , "switch"   );
 Add(kClass    , "class"    );
 Add(kExtern   , "extern"   );
 Add(kOperator , "operator" );
 Add(kConst    , "const"    );
 Add(kFalse    , "false"    );
 Add(kPrivate  , "private"  );
 Add(kWhile    , "while"    );
 Add(kFloat    , "float"    );
 Add(kProtected, "protected");
 Add(kFor      , "for"      );
 Add(kPublic   , "public"   );

 Add(kPin          , "pin"           );
 Add(kPinGroup     , "pin_group"     );
 Add(kPinArray     , "pin_array"     );
 Add(kBus          , "bus"           );
 Add(kClock        , "clock"         );
 Add(kReset        , "reset"         );
 Add(kTask         , "task"          );
 Add(kClockEnable  , "clock_enable"  );
 Add(kLoop         , "loop"          );
 Add(kPipe         , "pipe"          );
 Add(kString       , "string"        );
 Add(kParam        , "param"         );
 Add(kScript       , "script"        );
 Add(kRtl          , "rtl"           );
 Add(kIn           , "in"            );
 Add(kOut          , "out"           );
 Add(kExternalSetup, "external_setup");
 Add(kExternalHold , "external_hold" );
 Add(kExternalDelay, "external_delay");
 Add(kTarget       , "target"        );
}
//------------------------------------------------------------------------------

void KEYWORD::Add(KEYWORD_CODE Code, const char* Identifier){
 KEYWORD_NODE* N = new KEYWORD_NODE;
 N->Code       = Code;
 N->Identifier = Identifier;
 Insert(N);

 // Build reverse lookup list
 N->Next = List;
 List    = N;
}
//------------------------------------------------------------------------------

KEYWORD_CODE KEYWORD::GetCode(const char* Identifier){
 KEYWORD_NODE Key;
 Key.Identifier = Identifier;

 KEYWORD_NODE* N = (KEYWORD_NODE*)Find(&Key);

 if(N) return N->Code;
 else  return kUnknown;
}
//------------------------------------------------------------------------------

bool KEYWORD::GetName(KEYWORD_CODE Code, STRING& Identifier){
 KEYWORD_NODE* N = List;
 while(N){
  if(N->Code == Code){
   Identifier = N->Identifier;
   return true;
  }
  N = N->Next;
 }
 return false;
}
//------------------------------------------------------------------------------
