#ifndef HDL_h
#define HDL_h
//------------------------------------------------------------------------------

#include "MyString.h"
#include "FileSystem.h"
//------------------------------------------------------------------------------

struct HDL{
 struct PARAMETER{
  STRING Name;
  STRING Value;

  PARAMETER* Next;

  PARAMETER(const char* Name, const char* Value);
 ~PARAMETER(); // Also deletes the rest of the list
 };

 struct SIGNAL{
  STRING Name;

  bool Port;
  bool Register;

  enum DIRECTION{ // Only applicable to ports
   Input,
   Output,
   Bidirectional
  } Direction;

  bool Vector;
  int  VectorStart;
  int  VectorEnd;
  int  ArrayDimensions;
  int* ArrayStart; // Array of start indices
  int* ArrayEnd;   // Array of end indices

  SIGNAL* Next;

  SIGNAL(const char* Name);
 ~SIGNAL(); // Also deletes the rest of the list

  void Write(STRING& Buffer);
 };

 struct AST{
  // No begin-end block type: this is handled by means of a
  // linked list of statements
  enum OPERATOR{
   Assign,         // "assign" keyword
   BlockAssign,    // "=" within always block
   NonBlockAssign, // "<=" within always block

   Select,

   Reduce_AND,
   Reduce_NAND,
   Reduce_OR,
   Reduce_NOR,
   Reduce_XOR,
   Reduce_XNOR,

   Logical_NOT,
   Bitwise_NOT,

   Negate,

   Concatenate, // 1 child: a linked list of statements
   Replicate,

   Multiply,
   Divide,
   Modulus,
   Add,
   Subtract,

   ShiftLeft,
   ShiftRight,

   Greater,
   Greater_Equal,
   Less,
   Less_Equal,
   Equal,
   Not_Equal,

   Bitwise_AND,
   Bitwise_XOR,
   Bitwise_XNOR,
   Bitwise_OR,

   Logical_AND,
   Logical_OR,

   Conditional, // 3 children

   // Only one child: the statement list
   Always_Comb, // Takes no sensitivity list (uses (*))
   Always_FF, // Takes only one clock as sensitivity list

   Case, // 3 children: Condition, a linked list of cases and the default case
   CaseX,
   Case_Child, // Includes the case label and body

   If, // 3 children: condition, true statements, false statements

   Identifier,
   Constant
  } Operator;

  // Some have 3 children, others have one child, where the child is a linked
  // list.  For instance, the case statement has two children:
  // 1) A linked list of cases,
  // 2) The default case
  AST* Child1;
  AST* Child2;
  AST* Child3;
  AST* Next; // Sibling in case of a linked list

  STRING Name;
  int    ConstantValue;
  int    ConstantLength;

  AST(); // Initialises to constant 32'd0, with no siblings or children
 ~AST(); // Also deletes siblings and children

  void WriteGroup(STRING& Buffer);
  void Write     (STRING& Buffer);
 };

 struct INSTANCE{ // Sub-circuit instance
  STRING Module;   // Module name
  STRING Instance; // Instance name

  // These are new instances, not linked to the module instance
  PARAMETER* Parameters; // List of parameter overrides (named)
  SIGNAL*    Ports;      // List of port assignments (names)
  AST*       PortValues; // List of port assignments (values)

  INSTANCE* Next;

  INSTANCE();
 ~INSTANCE(); // Also deletes the rest of the list
 };

 struct MODULE{
  STRING Name;

  PARAMETER* Parameters; // List of parameters
  SIGNAL*    Signals;    // List of ports, wires and registers
  INSTANCE*  Instances;  // List of sub-circuit instances
  AST*       Statements; // AST

  MODULE* Next;

  MODULE(const char* Name);
 ~MODULE(); // Also deletes the rest of the list

  void Write(STRING& Buffer);
 };

 struct FILE{
  STRING  Name; // File name with relative path
  MODULE* Modules; // Linked list of modules

  FILE(const char* Name);
 ~FILE(); // Also deletes modules

  bool Write();
 };

 FILE* Toplevel;

 HDL();
~HDL(); // Deletes everything
};
//------------------------------------------------------------------------------

extern HDL hdl;
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
