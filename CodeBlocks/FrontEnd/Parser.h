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

#ifndef Parser_h
#define Parser_h
//------------------------------------------------------------------------------

#include <math.h>
//------------------------------------------------------------------------------

#include "Pin.h"
#include "Target.h"
#include "PreProcessor.h"
//------------------------------------------------------------------------------

class PARSER{
 private:
  struct ATTRIBUTE_LIST{
   STRING Property;
   STRING Value;

   ATTRIBUTE_LIST* Next;

   ATTRIBUTE_LIST();
  ~ATTRIBUTE_LIST();
  };

  KEYWORDS Keywords;

  PREPROCESSOR Preprocessor;
  PREPROCESSOR::TOKEN Token;

  inline bool GetToken();

  void DisplayToken();

  bool error;
  void Error  (const char* Message);
  void Warning(const char* Message);

  bool            PropertyValuePair(STRING& Property, STRING& Value);
  ATTRIBUTE_LIST* AttributeList();

  double GetFloatValue(STRING& Value, STRING* Unit = 0);

  bool Target();

  bool GetPinIndex(int& Index);

  bool PinGroup(ATTRIBUTE_LIST* ParentList = 0, STRING* ParentName = 0);
  bool PinArray(
   ATTRIBUTE_LIST* ParentList       = 0,
   STRING        * ParentName       = 0,
   bool            Array            = false,
   int             ParentArrayDepth = 0,
   int*            ParentIndex      = 0
  );
  bool Pin(
   ATTRIBUTE_LIST* ParentList       = 0,
   STRING        * ParentName       = 0,
   bool            Array            = false,
   int             ParentArrayDepth = 0,
   int*            ParentIndex      = 0
  );

  void ApplyPinAttributes(PIN* Pin, ATTRIBUTE_LIST* List);

 public:
FILE* File;

  PARSER();
 ~PARSER();

  bool Run(const char* Filename);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
