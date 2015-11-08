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

#include "PreProcessor.h"
//------------------------------------------------------------------------------

PREPROCESSOR::FIXED_POINT::FIXED_POINT(){
 IntegerBits  = 0;
 FractionBits = 0;
 Bits         = 0;
}
//------------------------------------------------------------------------------

PREPROCESSOR::FIXED_POINT::~FIXED_POINT(){
 if(Bits) delete[] Bits;
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::FIXED_POINT::Init(
 unsigned IntegerBits,
 unsigned FractionBits
){
 this->IntegerBits  = IntegerBits;
 this->FractionBits = FractionBits;

 unsigned TotalBits = IntegerBits + FractionBits;
 if(TotalBits & 0x1F) TotalBits = (TotalBits >> 5) + 1;
 else                 TotalBits = (TotalBits >> 5);

 Bits = new unsigned[TotalBits];

 return Bits;
}
//------------------------------------------------------------------------------

PREPROCESSOR::PREPROCESSOR(){
 Scanner = new SCANNER;
}
//------------------------------------------------------------------------------

PREPROCESSOR::~PREPROCESSOR(){
 delete Scanner;
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::Open(const char* Filename){
 if(!Scanner->Open(Filename));

 Scanner->GetToken(&ppToken);
 return true;
}
//------------------------------------------------------------------------------

bool PREPROCESSOR::GetToken(TOKEN* Token){
 Token->Comment.Clear();

 while(ppToken.Type != SCANNER::tEOF){
  Token->Line    =  ppToken.Line;
  Token->Comment << ppToken.Comment;

  if(ppToken.Type == SCANNER::tNewline){
   Scanner->GetToken(&ppToken);
   continue;
  }

  if(ppToken.Type == SCANNER::tDirective){
   while(Scanner->GetToken(&ppToken)){ // Just ignore them for now...
    if(ppToken.Type == SCANNER::tNewline){
     Scanner->GetToken(&ppToken);
     break;
    }
   }
   continue;
  }

  if(ppToken.Type == SCANNER::tIdentifier){
   Token->Keyword = Keywords.GetCode(ppToken.Token.String());
   if(Token->Keyword){
    Token->Type = tKeyword;
   }else{
    Token->Type   = tIdentifier;
    Token->String = ppToken.Token;
   }
   Scanner->GetToken(&ppToken);
   return true;
  }
  if(ppToken.Type == SCANNER::tNumber){
   /// @todo Scan numbers properly
   Token->Type  = tFloat;
   Token->Float = atof(ppToken.Token.String());
   Scanner->GetToken(&ppToken);
   return true;
  }
  if(ppToken.Type == SCANNER::tCharacter){
   Token->Type = tFixedPoint;
   Scanner->GetToken(&ppToken);
   return true;
  }
  if(ppToken.Type == SCANNER::tString){
   /// @todo Concatenate strings.  Remember that there could be directives and newlines between strings...
   ///       Maybe add another level of processing?  A function to get the next ppToken (like in SCANNER)?
   Token->Type   = tString;
   Token->String = ppToken.Token;
   Scanner->GetToken(&ppToken);
   return true;
  }
  if(ppToken.Type == SCANNER::tOperator){
   Token->Operator = Operators.GetCode(ppToken.Token.String());
   if(Token->Operator){
    Token->Type = tOperator;
    Scanner->GetToken(&ppToken);
    return true;
   }
   printf("Error: unknown operator\n"); /// @todo Use better error handling (copy from SCANNER)
   break;
  }
  if(ppToken.Type == SCANNER::tOther){
   printf("Error: invalid token\n"); /// @todo Use better error handling (copy from SCANNER)
   break;
  }
 }
 Token->Type = tEOF;
 return false;
}
//------------------------------------------------------------------------------
