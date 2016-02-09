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

#include "main.h"
//------------------------------------------------------------------------------

int main(int argc, char** argv){
 #ifdef __WIN32
  // UTF-8 Console encoding
  SetConsoleOutputCP(CP_UTF8); // UTF-8 console
  // Read the current console size
  CONSOLE_SCREEN_BUFFER_INFO Info;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
  // Make the console buffer as many lines as the system will allow
  Info.dwSize.Y = 0x7FFF;
  while(
   !SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), Info.dwSize)
  ) Info.dwSize.Y--;
 #endif

 SetCurrentDirectory(L"C:\\JPT\\Projects\\15\\1509 - ALCHA\\Test Cases\\FrontEnd");

 SCANNER Scanner;
 if(!Scanner.Open((byte*)"Scanner.alc")) return -1;

 TOKEN Token;
 while(Scanner.GetToken(&Token)){
  printf("%d: %d = %s", Token.Line, Token.Type, Token.Data.String());
  if(Token.Type == TOKEN::Literal){
   printf(" = ");
   Token.Value.Display();
  }
  printf("\n");
 }

// SetCurrentDirectory(L"C:\\JPT\\Projects\\15\\1509 - ALCHA\\Test Cases\\Output");
//
// Target.Vendor = "Altera";
// Target.Series = "MAX 10";
// Target.Device = "10M08DAF484C8GES";
// Target.Board  = "BeMicro Max 10";
//
// hdl.Files = new HDL::FILE("Testing");
//
// HDL::MODULE* Module = hdl.Files->Modules = new HDL::MODULE("Testing");
//
// HDL::SIGNAL* Port;
// Port = Module->Signals = new HDL::SIGNAL("Clk");
// Port->Port      = true;
// Port->Direction = HDL::SIGNAL::Input;
//
// Port = Port->Next = new HDL::SIGNAL("LVDS_TX_P");
// Port->Port        = true;
// Port->Direction   = HDL::SIGNAL::Output;
// Port->Vector      = true;
// Port->VectorStart = 0;
// Port->VectorEnd   = 0;
//
// Port = Port->Next = new HDL::SIGNAL("Button");
// Port->Port        = true;
// Port->Direction   = HDL::SIGNAL::Input;
// Port->Vector      = true;
// Port->VectorStart = 4;
// Port->VectorEnd   = 1;
//
// Port = Port->Next = new HDL::SIGNAL("LED");
// Port->Port        = true;
// Port->Direction   = HDL::SIGNAL::Output;
// Port->Vector      = true;
// Port->VectorStart = 8;
// Port->VectorEnd   = 1;
//
// HDL::AST* ast = Module->Statements = new HDL::AST;
// ast->Operator = HDL::AST::Assign;
// ast->Child1   = new HDL::AST;
// ast->Child1->Operator = HDL::AST::Identifier;
// ast->Child1->Name     = "LED";
// ast->Child2 = new HDL::AST;
// ast->Child2->Operator      = HDL::AST::Replicate;
// ast->Child2->ConstantValue = 2;
// ast->Child2->Child1        = new HDL::AST;
// ast->Child2->Child1->Operator = HDL::AST::Identifier;
// ast->Child2->Child1->Name     = "Button";
//
// ast = ast->Next = new HDL::AST;
// ast->Operator = HDL::AST::Assign;
// ast->Child1 = new HDL::AST;
// ast->Child1->Operator = HDL::AST::Identifier;
// ast->Child1->Name     = "LVDS_TX_P";
// ast->Child2 = new HDL::AST;
// ast->Child2->Operator = HDL::AST::Identifier;
// ast->Child2->Name     = "Clk";
//
// ast = ast->Next = new HDL::AST;
// ast->Operator = HDL::AST::Assign;
// ast->Child1 = new HDL::AST;
// ast->Child1->Operator = HDL::AST::Identifier;
// ast->Child1->Name     = "Testing";
// ast->Child2 = new HDL::AST;
// ast->Child2->Operator = HDL::AST::Constant;
// ast->Child2->ConstantLength = 16;
// ast->Child2->ConstantValue  = 0xABCD;
//
// ALTERA Altera;
// if(!Altera.WriteProject()) return -2;

 return 0;
}
//------------------------------------------------------------------------------
