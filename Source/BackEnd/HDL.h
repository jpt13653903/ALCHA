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

    PARAMETER(const byte* Name, const byte* Value);
   ~PARAMETER(); // Also deletes the rest of the list
  };

  struct NET{
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

    NET* Next;

    NET(const byte* Name);
   ~NET(); // Also deletes the rest of the list

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

    AST* Child1;
    AST* Child2;
    AST* Child3;
    AST* Next; // Next node in the linked list

    STRING Name;
    int    ConstantValue; /// @todo Change to mpz_t
    int    ConstantLength;

    AST(); // Initialises to constant 32'd0, with no siblings or children
   ~AST(); // Also deletes siblings and children

    void WriteIndent(STRING& Buffer);
    void WriteGroup (STRING& Buffer);
    void Write      (STRING& Buffer);
  };

  struct INSTANCE{ // Sub-circuit instance
    STRING Module;   // Module name
    STRING Instance; // Instance name

    // These are new instances, not linked to the module instance
    PARAMETER* Parameters; // List of parameter overrides (named)
    NET*       Ports;      // List of port assignments (names)
    AST*       PortValues; // List of port assignments (values)

    INSTANCE* Next;

    INSTANCE();
   ~INSTANCE(); // Also deletes the rest of the list
  };

  struct MODULE{
    STRING Name;

    PARAMETER* Parameters; // List of parameters
    NET*       Nets;       // List of ports, wires and registers
    INSTANCE*  Instances;  // List of sub-circuit instances
    AST*       Statements; // AST

    MODULE* Next;

    MODULE(const byte* Name);
   ~MODULE(); // Also deletes the rest of the list

    void Write(STRING& Buffer);
  };

  struct FILE{
    STRING  Name; // File name with relative path
    MODULE* Modules; // Linked list of modules

    FILE* Next;

    FILE(const byte* Name);
   ~FILE(); // Also deletes modules and the rest of the file list

    bool Write();
  };

  FILE* Files; // Linked list of files; the first one is the top-level file

  HDL();
~HDL(); // Deletes everything

  bool Write();
};
//------------------------------------------------------------------------------

extern HDL hdl;
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
