#include "HDL.h"
//------------------------------------------------------------------------------

HDL hdl;
//------------------------------------------------------------------------------

static int Indent = 0;
//------------------------------------------------------------------------------

HDL::PARAMETER::PARAMETER(const byte* Name, const byte* Value){
 this->Name  = Name;
 this->Value = Value;

 Next = 0;
}
//------------------------------------------------------------------------------

HDL::PARAMETER::~PARAMETER(){
 if(Next) delete Next;
}
//------------------------------------------------------------------------------

HDL::SIGNAL::SIGNAL(const byte* Name){
 this->Name = Name;

 Port            = false;
 Register        = false;
 Direction       = Bidirectional;
 Vector          = false;
 VectorStart     = 0;
 VectorEnd       = 0;
 ArrayDimensions = 0;
 ArrayStart      = 0;
 ArrayEnd        = 0;
 Next            = 0;
}
//------------------------------------------------------------------------------

HDL::SIGNAL::~SIGNAL(){
 if(ArrayStart) delete[] ArrayStart;
 if(ArrayEnd  ) delete[] ArrayEnd;
 if(Next      ) delete   Next;
}
//------------------------------------------------------------------------------

void HDL::SIGNAL::Write(STRING& Buffer){
 if(Port){
  switch(Direction){
   case Input : Buffer << " input  "; break;
   case Output: Buffer << " output "; break;
   default    : Buffer << " inout  "; break;
  }
  if(Register) Buffer << "reg ";
 }else{
  if(Register) Buffer << "reg ";
  else         Buffer << "wire ";
 }
 if(Vector){
  Buffer << "[" << VectorStart << ":" << VectorEnd << "]";
 }
 Buffer << Name;
 for(int j = 0; j < ArrayDimensions; j++){
  Buffer << "[" << ArrayStart[j] << ":" << ArrayEnd[j] << "]";
 }
}
//------------------------------------------------------------------------------

HDL::AST::AST(){
 Operator       = Constant;
 ConstantValue  = 0;
 ConstantLength = 32;

 Child1 = Child2 = Child3 = Next = 0;
}
//------------------------------------------------------------------------------

HDL::AST::~AST(){
 // Nesting is generally short, so this can safely be done recursively:
 if(Child1) delete Child1;
 if(Child2) delete Child2;
 if(Child3) delete Child3;

 // The list could be quite long, so don't do it recursively:
 AST* Temp;
 while(Next){
  Temp = Next;
  Next = Next->Next;
  Temp->Next = 0;
  delete Temp;
 }
}
//------------------------------------------------------------------------------

void HDL::AST::WriteIndent(STRING& Buffer){
 int j;
 for(j = 0; j < Indent; j++) Buffer << " ";
}
//------------------------------------------------------------------------------

void HDL::AST::WriteGroup(STRING& Buffer){
 AST* Temp;

 if(this->Next){
  Buffer << "begin\n";
  Indent++;
 }
 Temp = this;
 while(Temp){
  WriteIndent(Buffer);
  Temp->Write(Buffer);
  Temp = Temp->Next;
 }
 if(this->Next){
  Buffer << "end\n";
  Indent--;
 }
}
//------------------------------------------------------------------------------

void HDL::AST::Write(STRING& Buffer){
 AST* Temp;

 switch(Operator){
  case Assign:
   Buffer << "assign ";

  case BlockAssign:
   Child1->Write(Buffer);
   Buffer << " = ";
   Child2->Write(Buffer);
   Buffer << ";\n";
   break;

  case NonBlockAssign:
   Child1->Write(Buffer);
   Buffer << " <= ";
   Child2->Write(Buffer);
   Buffer << ";\n";
   break;

  case Select:
   Child1->Write(Buffer); // The variable to slice
   Buffer << "[";
   Child2->Write(Buffer);
   if(Child3){
    Buffer << ":";
    Child3->Write(Buffer);
   }
   Buffer << "]";
   break;

  case Reduce_AND:
   Buffer << "(&";
   Child1->Write(Buffer);
   Buffer << ")";
   break;

  case Reduce_NAND:
   Buffer << "(~&";
   Child1->Write(Buffer);
   Buffer << ")";
   break;

  case Reduce_OR:
   Buffer << "(|";
   Child1->Write(Buffer);
   Buffer << ")";
   break;

  case Reduce_NOR:
   Buffer << "(~|";
   Child1->Write(Buffer);
   Buffer << ")";
   break;

  case Reduce_XOR:
   Buffer << "(^";
   Child1->Write(Buffer);
   Buffer << ")";
   break;

  case Reduce_XNOR:
   Buffer << "(~^";
   Child1->Write(Buffer);
   Buffer << ")";
   break;

  case Logical_NOT:
   Buffer << "(!";
   Child1->Write(Buffer);
   Buffer << ")";
   break;

  case Bitwise_NOT:
   Buffer << "(~";
   Child1->Write(Buffer);
   Buffer << ")";
   break;

  case Negate:
   Buffer << "(-";
   Child1->Write(Buffer);
   Buffer << ")";
   break;

  case Concatenate:
   Buffer << "{";
   Temp = Child1;
   while(Temp){
    Temp->Write(Buffer);
    Temp = Temp->Next;
    if(Temp) Buffer << ", ";
   }
   Buffer << "}";
   break;

  case Replicate:
   Buffer << "{" << ConstantValue << "{";
   Child1->Write(Buffer);
   Buffer << "}}";
   break;

  case Multiply:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " * ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case Divide:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " / ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case Modulus:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " % ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case Add:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " + ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case Subtract:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " - ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case ShiftLeft:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " << ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case ShiftRight:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " >> ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case Greater:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " > ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case Greater_Equal:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " >= ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case Less:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " < ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case Less_Equal:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " <= ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case Equal:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " == ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case Not_Equal:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " != ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case Bitwise_AND:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " & ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case Bitwise_XOR:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " ^ ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case Bitwise_XNOR:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " ~^ ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case Bitwise_OR:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " | ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case Logical_AND:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " && ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case Logical_OR:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " || ";
   Child2->Write(Buffer);
   Buffer << ")";
   break;

  case Conditional:
   Buffer << "(";
   Child1->Write(Buffer);
   Buffer << " ? ";
   Child2->Write(Buffer);
   Buffer << " : ";
   Child3->Write(Buffer);
   Buffer << ")";
   break;

  case Always_Comb:
   Buffer << "always @(*) ";
   Child1->WriteGroup(Buffer);
   break;

  case Always_FF: break;
   Buffer << "always @(posedge " << Name << ") ";
   Child1->WriteGroup(Buffer);
   break;

  case Case:
   Buffer << "case(";
   Child1->Write(Buffer);
   Buffer << ")\n";
   Indent++;
   Temp = Child2;
   while(Temp){
    Temp->Write(Buffer);
    Temp = Temp->Next;
    if(Temp) Buffer << "\n";
   }
   if(Child3){
    WriteIndent(Buffer);
    Buffer << "default: ";
    Child3->WriteGroup(Buffer);
   }
   Buffer << "endcase\n";
   Indent--;
   break;

  case CaseX:
   Buffer << "casex(";
   Child1->Write(Buffer);
   Buffer << ")\n";
   Indent++;
   Temp = Child2;
   while(Temp){
    Temp->Write(Buffer);
    Temp = Temp->Next;
    if(Temp) Buffer << "\n";
   }
   if(Child3){
    WriteIndent(Buffer);
    Buffer << "default: ";
    Child3->WriteGroup(Buffer);
   }
   Buffer << "endcase\n";
   Indent--;
   break;

  case Case_Child:
   WriteIndent(Buffer);
   Child1->Write(Buffer);
   Buffer << ": ";
   Child2->WriteGroup(Buffer);
   break;

  case If:
   Buffer << "if(";
   Child1->Write(Buffer);
   Buffer << ") ";
   Child2->WriteGroup(Buffer);
   if(Child3){
    Buffer << "else ";
    Child3->WriteGroup(Buffer);
   }
   break;

  case Identifier:
   Buffer << Name;
   break;

  case Constant:
   Buffer << ConstantLength << "'h";
   Buffer.Hex(ConstantValue);
   break;

  default:
   break;
 }
}
//------------------------------------------------------------------------------

HDL::INSTANCE::INSTANCE(){
 Parameters = 0;
 Ports      = 0;
 PortValues = 0;
 Next       = 0;
}
//------------------------------------------------------------------------------

HDL::INSTANCE::~INSTANCE(){
 if(Parameters) delete Parameters;
 if(Ports     ) delete Ports;
 if(PortValues) delete PortValues;
 if(Next      ) delete Next;
}
//------------------------------------------------------------------------------

HDL::MODULE::MODULE(const byte* Name){
 this->Name = Name;

 Parameters = 0;
 Signals    = 0;
 Instances  = 0;
 Statements = 0;
 Next       = 0;
}
//------------------------------------------------------------------------------

HDL::MODULE::~MODULE(){
 if(Parameters) delete Parameters;
 if(Signals   ) delete Signals;
 if(Instances ) delete Instances;
 if(Statements) delete Statements;
 if(Next      ) delete Next;
}
//------------------------------------------------------------------------------

void HDL::MODULE::Write(STRING& Buffer){
 Buffer << "module " << Name;

 if(Parameters){
  Buffer << " #(\n ";
  PARAMETER* Parameter = Parameters;
  while(Parameter){
   Buffer << Parameter->Name << " = " << Parameter->Value;
   Parameter = Parameter->Next;
   if(Parameter) Buffer << ",\n ";
   else          Buffer << "\n";
  }
  Buffer << ")";
 }

 bool Ports     = false;
 bool Wires     = false;
 bool Registers = false;

 SIGNAL* Signal = Signals;
 while(Signal){
       if(Signal->Port    ) Ports     = true;
  else if(Signal->Register) Registers = true;
  else                      Wires     = true;
  if(Ports && Wires && Registers) break;
  Signal = Signal->Next;
 }

 if(Ports){
  Buffer << "(\n";
  Signal = Signals;
  while(Signal){
   if(Signal->Port){
    Signal->Write(Buffer);
    Signal = Signal->Next;
    if(Signal) Buffer << ",\n";
    else       Buffer << "\n";
   }
  }
  Buffer << ");\n";
  Buffer << "//--------------------------------------"
            "----------------------------------------\n\n";
 }else{
  Buffer << ";\n";
 }

 if(Wires){
  Signal = Signals;
  while(Signal){
   if(!Signal->Port && !Signal->Register){
    Signal->Write(Buffer);
    Buffer << ";\n";
    Signal = Signal->Next;
   }
  }
  Buffer << "//--------------------------------------"
            "----------------------------------------\n\n";
 }

 if(Registers){
  Signal = Signals;
  while(Signal){
   if(!Signal->Port && Signal->Register){
    Signal->Write(Buffer);
    Buffer << ";\n";
    Signal = Signal->Next;
   }
  }
  Buffer << "//--------------------------------------"
            "----------------------------------------\n\n";
 }

 /// @todo Instances

 if(Statements){
  AST* Temp = Statements;
  while(Temp){
   Temp->Write(Buffer);
   Temp = Temp->Next;
  }
  Buffer << "//--------------------------------------"
            "----------------------------------------\n\n";
 }

 Buffer << "endmodule\n";
 Buffer << "//--------------------------------------"
           "----------------------------------------\n\n";
}
//------------------------------------------------------------------------------

HDL::FILE::FILE(const byte* Name){
 this->Name = Name;
 Modules = 0;
 Next    = 0;
}
//------------------------------------------------------------------------------

HDL::FILE::~FILE(){
 if(Modules) delete Modules;
 if(Next   ) delete Next;
}
//------------------------------------------------------------------------------

bool HDL::FILE::Write(){
 STRING Buffer;

 Indent = 0;

 MODULE* Module = Modules;
 while(Module){
  Module->Write(Buffer);
  Module = Module->Next;
 }

 STRING Filename;
 Filename << Name << ".v";
 FILE_SYSTEM fs;
 return fs.Write(Filename.String(), Buffer.String(), Buffer.Length());
}
//------------------------------------------------------------------------------

HDL::HDL(){
 Files = 0;
}
//------------------------------------------------------------------------------

HDL::~HDL(){
 if(Files) delete Files;
}
//------------------------------------------------------------------------------

bool HDL::Write(){
 FILE* File = Files;
 while(File){
  if(!File->Write()) return false;
  File = File->Next;
 }
 return true;
}
//------------------------------------------------------------------------------
