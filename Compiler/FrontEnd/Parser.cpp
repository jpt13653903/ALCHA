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

#include "Parser.h"
//------------------------------------------------------------------------------

PARSER::ATTRIBUTE_LIST::ATTRIBUTE_LIST(){
 Next = 0;
}
//------------------------------------------------------------------------------

PARSER::ATTRIBUTE_LIST::~ATTRIBUTE_LIST(){
 if(Next) delete Next;
}
//------------------------------------------------------------------------------

PARSER::PARSER(){
 error = false;
}
//------------------------------------------------------------------------------

PARSER::~PARSER(){
}
//------------------------------------------------------------------------------

void PARSER::Error(const char* Message){
 error = true;
 printf(
  "Line %05d of %s\n  Error: %s\n",
  Token.Line,
  Token.File.String(),
  Message
 );
}
//------------------------------------------------------------------------------

void PARSER::Warning(const char* Message){
 printf(
  "Line %05d of %s\n  Warning: %s\n",
  Token.Line,
  Token.File.String(),
  Message
 );
}
//------------------------------------------------------------------------------

bool PARSER::GetToken(){
 if(!Preprocessor.GetToken(&Token)) return false;
// DisplayToken();
 return true;
}
//------------------------------------------------------------------------------

void PARSER::DisplayToken(){
 STRING s;

 printf("%05d: ", Token.Line);
 switch(Token.Type){
  case PREPROCESSOR::tIdentifier:
   printf("Identifier:  \"%s\"", Token.String.String());
   break;

  case PREPROCESSOR::tKeyword:
   Keywords.GetName(Token.Keyword, s);
   printf("Keyword %2d:  \"%s\"", Token.Keyword, s.String());
   break;

  case PREPROCESSOR::tOperator:
   Operators.GetName(Token.Operator, s);
   printf("Operator %2d: \"%s\"", Token.Operator, s.String());
   break;

  case PREPROCESSOR::tString:
   printf("String:      \"%s\"", Token.String.String());
   break;

  case PREPROCESSOR::tNumber:
   printf("Number:      ");
   Token.Number.Display() ;
   break;

  case PREPROCESSOR::tFixedPointCast:
   printf("Fixed-point: `%d.%d",
    Token.FixedPointFormat.IntegerBits,
    Token.FixedPointFormat.FractionBits
   );
   break;

  case PREPROCESSOR::tEOF:
   printf("End of file\n");
   break;

  default:
   printf("Unknown:     \"%s\"", Token.String.String());
   break;
 }
// if(Token.Comment.Length()){
//  printf(" Associated comment: %s", Token.Comment.String());
// }
 printf("\n");
}
//------------------------------------------------------------------------------

bool PARSER::PropertyValuePair(STRING& Property, STRING& Value){
 if(Token.Type != PREPROCESSOR::tIdentifier) return false;
 Property = Token.String;

 if(
  !GetToken()                                 ||
  (Token.Type     != PREPROCESSOR::tOperator) ||
  (Token.Operator != oAssign                )
 ){
  Error("= expected");
  return false;
 }
 if(
  !GetToken() ||
  (Token.Type != PREPROCESSOR::tString)
 ){
  Error("Value expected");
  return false;
 }
 Value = Token.String;
 GetToken();
 return true;
}
//------------------------------------------------------------------------------

PARSER::ATTRIBUTE_LIST* PARSER::AttributeList(){
 if(
  Token.Type     != PREPROCESSOR::tOperator ||
  Token.Operator != oBeginAttributes
 ) return 0;
 GetToken();

 if( // Check for empty list
  Token.Type     == PREPROCESSOR::tOperator &&
  Token.Operator == oEndAttributes
 ){
  GetToken();
  return 0;
 }

 ATTRIBUTE_LIST* Result = 0;
 ATTRIBUTE_LIST* Tail   = 0;

 while(!error && Token.Type != PREPROCESSOR::tEOF){
  if(Tail){
   Tail->Next = new ATTRIBUTE_LIST;
   Tail       = Tail->Next;
  }else{
   Result = new ATTRIBUTE_LIST;
   Tail   = Result;
  }

  if(!PropertyValuePair(Tail->Property, Tail->Value)){
   if(!error) Error("Property-value pair expected");
   delete Result;
   return 0;
  }
  if(Token.Type != PREPROCESSOR::tOperator){
   Error("',' or '>' expected");
   delete Result;
   return 0;
  }
  if(Token.Operator == oEndAttributes){
   GetToken();
   return Result;
  }
  if(Token.Operator != oComma){
   Error("',' expected");
   delete Result;
   return 0;
  }
  GetToken();
 }
 Error("Incomplete attribute list");
 delete Result;
 return 0;
}
//------------------------------------------------------------------------------

bool PARSER::Target(){
 if(Token.Type    != PREPROCESSOR::tKeyword) return false;
 if(Token.Keyword != kTarget               ) return false;

 GetToken();

 ATTRIBUTE_LIST* List = AttributeList();
 if(error){
  if(List) delete List;
  return false;
 }

 if(Token.Type != PREPROCESSOR::tIdentifier){
  Error("Identifier expected");
  if(List) delete List;
  return false;
 }
 STRING Name;
 Name = Token.String;
 if(
  !GetToken() ||
  (Token.Type     != PREPROCESSOR::tOperator) ||
  (Token.Operator != oEndState              )
 ){
  Error("';' expected");
  if(List) delete List;
  return false;
 }

 TARGET* Target = Targets.FindName(Name.String());
 if(!Target){
  Target = new TARGET(Name.String());
  Targets.Insert(Target);
 }

 // Update attributes

 ATTRIBUTE_LIST* Temp = List;
 while(Temp){
  if     (!Temp->Property.Compare("vendor")) Target->Vendor = Temp->Value;
  else if(!Temp->Property.Compare("series")) Target->Series = Temp->Value;
  else if(!Temp->Property.Compare("device")) Target->Device = Temp->Value;
  else if(!Temp->Property.Compare("board" )) Target->Board  = Temp->Value;
  else Warning("Unknown target attribute");
  Temp = Temp->Next;
 }
 if(List) delete List;

 return true;
}
//------------------------------------------------------------------------------

// Spaces (0x20) are ignored
// Value = ["+" | "-"] {Digit} ["." {Digit}] ["e" | "E" ["+" | "-"] {Digit}]
//         ["f" | "p" | "n" | "u" | "μ" | "\xB5" | "\xC2B5" | "m" |
//          "k" | "M" | "G" | "T"] [Unit];
double PARSER::GetFloatValue(STRING& Value, STRING* Unit){
 const unsigned char* Buffer = (const unsigned char*)Value.String();

 while(*Buffer == ' ') Buffer++;

 bool Sign = false;
 if(*Buffer == '+'){
  Buffer++; while(*Buffer == ' ') Buffer++;
 }else if(*Buffer == '-'){
  Sign = true;
  Buffer++; while(*Buffer == ' ') Buffer++;
 }

 double Result = 0;
 while(*Buffer){
  if(*Buffer < '0' || *Buffer > '9') break;
  Result = 10*Result + (*Buffer - '0');
  Buffer++; while(*Buffer == ' ') Buffer++;
 }

 if(*Buffer == '.'){
  double Scale = 1.0;

  Buffer++; while(*Buffer == ' ') Buffer++;
  while(*Buffer){
   if(*Buffer < '0' || *Buffer > '9') break;
   Scale  *= 0.1;
   Result += Scale*(*Buffer - '0');
   Buffer++; while(*Buffer == ' ') Buffer++;
  }
 }

 if(*Buffer == 'e' || *Buffer == 'E'){
  bool ExponentSign = false;
  int  Exponent     = 0;

  Buffer++; while(*Buffer == ' ') Buffer++;
  if(*Buffer == '+'){
   Buffer++; while(*Buffer == ' ') Buffer++;
  }else if(*Buffer == '-'){
   ExponentSign = true;
   Buffer++; while(*Buffer == ' ') Buffer++;
  }
  while(*Buffer){
   if(*Buffer < '0' || *Buffer > '9') break;
   Exponent = 10*Exponent + (*Buffer - '0');
   Buffer++; while(*Buffer == ' ') Buffer++;
  }
  if(ExponentSign) Exponent = -Exponent;
  Result *= pow(10.0, Exponent);
 }

 switch(*Buffer){
  case 'f': Result *= 1e-15; Buffer++; break;
  case 'p': Result *= 1e-12; Buffer++; break;
  case 'n': Result *= 1e-09; Buffer++; break;
  case 'u': Result *= 1e-06; Buffer++; break;
  case 'm': Result *= 1e-03; Buffer++; break;
  case 'k': Result *= 1e003; Buffer++; break;
  case 'M': Result *= 1e006; Buffer++; break;
  case 'G': Result *= 1e009; Buffer++; break;
  case 'T': Result *= 1e012; Buffer++; break;

  // Different "micro" variations:
  case 0xB5: Result *= 1e-06; Buffer++; break;
  case 0xC2:
   if(Buffer[1] == 0xB5){ // micro sign
    Result *= 1e-06;
    Buffer++;
   }
   break;

  case 0xCE:
   if(Buffer[1] == 0xBC){ // greek mu
    Result *= 1e-06;
    Buffer++;
   }
   break;

  default: break;
 }
 if(Unit) *Unit = (char*)Buffer;

 if(Sign) Result = -Result;

 return Result;
}
//------------------------------------------------------------------------------

bool PARSER::PinGroup(ATTRIBUTE_LIST* ParentList, STRING* ParentName){
 if(Token.Type    != PREPROCESSOR::tKeyword) return false;
 if(Token.Keyword != kPinGroup             ) return false;

 GetToken();

 ATTRIBUTE_LIST* List = AttributeList();
 if(error){
  if(List) delete List;
  return false;
 }

 STRING Name;
 if(ParentName) Name = *ParentName;
 if(Token.Type == PREPROCESSOR::tIdentifier){
  Name << Token.String;
  GetToken();
 }

 if(
  (Token.Type     != PREPROCESSOR::tOperator) ||
  (Token.Operator != oBegin                 )
 ){
  Error("'{' expected");
  if(List) delete List;
  return false;
 }

 ATTRIBUTE_LIST* ParentTail = ParentList;
 while(ParentTail && ParentTail->Next) ParentTail = ParentTail->Next;

 if(ParentList){
  ParentTail->Next = List;
  List = ParentList;
 }

 while(!error && GetToken()){
  if(
   (Token.Type     == PREPROCESSOR::tOperator) ||
   (Token.Operator == oEnd                   )
  ){
   if(ParentList){
    List = ParentTail->Next;
    ParentTail->Next = 0;
   }
   if(List) delete List;
   return true;
  }
  if(Pin     (List, &Name)) continue;
  if(PinGroup(List, &Name)) continue;
  if(PinArray(List, &Name)) continue;
  break;
 }
 if(ParentList){
  List = ParentTail->Next;
  ParentTail->Next = 0;
 }
 if(List) delete List;
 return false;
}
//------------------------------------------------------------------------------

bool PARSER::PinArray(
 ATTRIBUTE_LIST* ParentList,
 STRING        * ParentName,
 bool            Array,
 int             ParentArrayDepth,
 int*            ParentIndex
){
 if(Token.Type    != PREPROCESSOR::tKeyword) return false;
 if(Token.Keyword != kPinArray             ) return false;

 GetToken();

 int Index = 0;
 if(Array){
  if(!GetPinIndex(Index)) return false;
 }

 ATTRIBUTE_LIST* List = AttributeList();
 if(error){
  if(List) delete List;
  return false;
 }

 STRING Name;
 if(ParentName) Name << *ParentName;
 if(!Array && Token.Type == PREPROCESSOR::tIdentifier){
  Name << Token.String;
  GetToken();
 }

 if(
  (Token.Type     != PREPROCESSOR::tOperator) ||
  (Token.Operator != oBegin                 )
 ){
  Error("'{' expected");
  if(List) delete List;
  return false;
 }

 ATTRIBUTE_LIST* ParentTail = ParentList;
 while(ParentTail && ParentTail->Next) ParentTail = ParentTail->Next;

 if(ParentList){
  ParentTail->Next = List;
  List = ParentList;
 }

 int  j;
 int  ArrayDepth = 0;
 int* Indices    = 0;
 if(Array){
  ArrayDepth = ParentArrayDepth+1;
  Indices    = new int[ArrayDepth];
  for(j = 0; j < ParentArrayDepth; j++){
   Indices[j] = ParentIndex[j];
  }
  Indices[j] = Index;
 }

 while(!error && GetToken()){
  if(
   (Token.Type     == PREPROCESSOR::tOperator) ||
   (Token.Operator == oEnd                   )
  ){
   if(ParentList){
    List = ParentTail->Next;
    ParentTail->Next = 0;
   }
   if(List   ) delete   List;
   if(Indices) delete[] Indices;
   return true;
  }
  if(Pin     (List, &Name, true, ArrayDepth, Indices)) continue;
  if(PinArray(List, &Name, true, ArrayDepth, Indices)) continue;
  break;
 }
 if(ParentList){
  List = ParentTail->Next;
  ParentTail->Next = 0;
 }
 if(List   ) delete   List;
 if(Indices) delete[] Indices;
 return false;
}
//------------------------------------------------------------------------------

void PARSER::ApplyPinAttributes(PIN* Pin, ATTRIBUTE_LIST* List){
 if(!List) return;

 ATTRIBUTE_LIST* Temp = List;
 while(Temp){
  STRING Unit;

  if     (!Temp->Property.Compare("standard")) Pin->Standard = Temp->Value;
  else if(!Temp->Property.Compare("number"  )) Pin->Number   = Temp->Value;
  else if(!Temp->Property.Compare("pair"    )) Pin->Pair     = Temp->Value;

  else  if(!Temp->Property.Compare("direction")){
   if     (!Temp->Value.Compare("in" )) Pin->Direction = PIN::Input;
   else if(!Temp->Value.Compare("out")) Pin->Direction = PIN::Output;

  }else if(!Temp->Property.Compare("voltage")){
   Pin->Voltage = GetFloatValue(Temp->Value, &Unit);
   if(Unit.Compare("V")) Warning("Wrong unit: 'V' expected");

  }else if(!Temp->Property.Compare("current")){
   Pin->Current = GetFloatValue(Temp->Value, &Unit);
   if(Unit.Compare("A")) Warning("Wrong unit: 'A' expected");

  }else if(!Temp->Property.Compare("min_delay")){
   Pin->MinDelay = GetFloatValue(Temp->Value, &Unit);
   if(Unit.Compare("s")) Warning("Wrong unit: 's' expected");

  }else if(!Temp->Property.Compare("max_delay")){
   Pin->MaxDelay = GetFloatValue(Temp->Value, &Unit);
   if(Unit.Compare("s")) Warning("Wrong unit: 's' expected");

  }else if(!Temp->Property.Compare("frequency")){
   Pin->Frequency = GetFloatValue(Temp->Value, &Unit);
   if(Unit.Compare("Hz")) Warning("Wrong unit: 'Hz' expected");

  }else if(!Temp->Property.Compare("phase")){
   Pin->Phase = GetFloatValue(Temp->Value, &Unit);

  }else if(!Temp->Property.Compare("jitter")){
   Pin->Jitter = GetFloatValue(Temp->Value, &Unit);
   if(Unit.Compare("s")) Warning("Wrong unit: 's' expected");

  }else{
   Warning("Unknown pin attribute");
  }
  Temp = Temp->Next;
 }
}
//------------------------------------------------------------------------------

bool PARSER::GetPinIndex(int& Index){
 if(
  (Token.Type     != PREPROCESSOR::tOperator) ||
  (Token.Operator != oStartSlice            )
 ){
  Error("'[' expected");
  return false;
 }
 if(
  !GetToken() ||
  (Token.Type != PREPROCESSOR::tNumber)
 ){
  Error("Index expected");
  return false;
 }
 if(!Token.Number.IsInteger32()){
  Error("Index must be an integer in the 32-bit range");
  return false;
 }
 Index = (int)Token.Number.FloatingPoint();
 if(
  !GetToken()                                 ||
  (Token.Type     != PREPROCESSOR::tOperator) ||
  (Token.Operator != oEndSlice              )
 ){
  Error("']' expected");
  return false;
 }
 GetToken();
 return true;
}
//------------------------------------------------------------------------------

bool PARSER::Pin(
 ATTRIBUTE_LIST* ParentList,
 STRING        * ParentName,
 bool            Array,
 int             ParentArrayDepth,
 int*            ParentIndex
){
 if(Token.Type    != PREPROCESSOR::tKeyword) return false;
 if(Token.Keyword != kPin                  ) return false;

 GetToken();

 int Index = 0;
 if(Array){
  if(!GetPinIndex(Index)) return false;
 }

 ATTRIBUTE_LIST* List = AttributeList();
 if(error){
  if(List) delete List;
  return false;
 }

 STRING Name;
 if(ParentName) Name << *ParentName;
 if(!Array && Token.Type == PREPROCESSOR::tIdentifier){
  Name << Token.String;
  GetToken();
 }

 if(
  (Token.Type     != PREPROCESSOR::tOperator) ||
  (Token.Operator != oEndState              )
 ){
  Error("';' expected");
  if(List) delete List;
  return false;
 }

 if(!Name.Length()){
  Error("Pin definition with no name");
  if(List) delete List;
  return false;
 }

 bool Existing = true;
 PIN* Pin = Pins.FindName(Name.String());
 if(!Pin){
  Existing = false;
  Pin = new PIN(Name.String());
  Pins.Insert(Pin);
 }

 ApplyPinAttributes(Pin, ParentList);
 ApplyPinAttributes(Pin, List);
 if(List) delete List;

 if(Existing && (Array || Pin->ArrayDepth || Pin->Indices)){
  Error("Redeclaration of array pin");
  return false;
 }

 int j;
 if(Array){
  Pin->ArrayDepth = ParentArrayDepth+1;
  Pin->Indices    = new int[Pin->ArrayDepth];
  for(j = 0; j < ParentArrayDepth; j++){
   Pin->Indices[j] = ParentIndex[j];
  }
  Pin->Indices[j] = Index;
 }

 fprintf(File, "Pin: %s", Pin->Name.String());
 if(Pin->ArrayDepth){
  for(j = 0; j < Pin->ArrayDepth; j++){
   fprintf(File, "[%d]", Pin->Indices[j]);
  }
 }
 fprintf(File, "\n");
 fprintf(File, "  Standard : %s\n", Pin->Standard.String());
 fprintf(File, "  Number   : %s\n", Pin->Number  .String());
 fprintf(File, "  Pair     : %s\n", Pin->Pair    .String());
 fprintf(File, "  Direction: ");
  switch(Pin->Direction){
   case PIN::Input        : fprintf(File, "Input\n"        ); break;
   case PIN::Output       : fprintf(File, "Output\n"       ); break;
   case PIN::Bidirectional: fprintf(File, "Bidirectional\n"); break;
   default                : fprintf(File, "Unknown\n"      ); break;
  }
 fprintf(File, "  Voltage  : %lg V\n"  , Pin->Voltage       );
 fprintf(File, "  Current  : %lg mA\n" , Pin->Current  /1e-3);
 fprintf(File, "  MinDelay : %lg ns\n" , Pin->MinDelay /1e-9);
 fprintf(File, "  MaxDelay : %lg ns\n" , Pin->MaxDelay /1e-9);
 fprintf(File, "  Frequency: %lg MHz\n", Pin->Frequency/1e06);
 fprintf(File, "  Phase    : %lg\n"    , Pin->Phase         );
 fprintf(File, "  Jitter   : %lg ns\n" , Pin->Jitter   /1e-9);

 return true;
}
//------------------------------------------------------------------------------

bool PARSER::Run(const char* Filename){
 File = fopen("..\\Log.log", "wb");

 error = false;

 if(!Preprocessor.Open(Filename)) return false;

 while(!error && GetToken()){
  if(Target  ()) continue; if(error) return false;
  if(Pin     ()) continue; if(error) return false;
  if(PinGroup()) continue; if(error) return false;
  if(PinArray()) continue; if(error) return false;

  Error("Unknown construct");
  return false;
 }
 fclose(File);
 return true;
}
//------------------------------------------------------------------------------
