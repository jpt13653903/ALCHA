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

#ifndef Netlist_Base_h
#define Netlist_Base_h
//------------------------------------------------------------------------------

#include <map>
#include <string>
//------------------------------------------------------------------------------

#include "Logger.h"
#include "General.h"
#include "AST/Assignment.h"
//------------------------------------------------------------------------------

namespace AST{
  class BASE;
  class ASSIGNMENT;
  class EXPRESSION;
}
//------------------------------------------------------------------------------

namespace NETLIST{
  class ATTRIBUTE;
  class NAMESPACE;
  class SYNTHESISABLE;

  class BASE{ // Base class for the symbol table
    private:
      bool Temporary;
    //--------------------------------------------------------------------------

    protected:
      void DisplayAttributes(int Indent);
    //--------------------------------------------------------------------------

    public:
      // Indentation follows the inheritance tree
      enum class TYPE{
        Attribute,
        Number,
        Byte,
        Character,

        Alias,
        Array, // An array of objects

        
        // Synthesisable
          Pin,
          Net,
            PinComponent,

        // Namespace
          Module,
          Group
      } Type;

      virtual bool IsSynthesisable();
      virtual bool IsNamespace    ();

      // The line and filename that resulted in this object, used for error reporting
      struct{
        int         Line;
        std::string Filename;
      } Source;

      std::string Name;
      NAMESPACE*  Namespace;
      std::map<std::string, ATTRIBUTE*> Attributes;

      bool IsTemporary();
    //--------------------------------------------------------------------------

    public:
      // When Name is 0, a name is auto-generated
               BASE(int Line, const std::string& Filename, const char* Name, TYPE Type);
      virtual ~BASE();

      void Error  (const char* Message = 0);
      void Warning(const char* Message = 0);

      // Called by the definition to assign the attribute list
      bool ApplyAttributes(AST::ASSIGNMENT* AttributeList);

      // Used specifically for operate-and-assign expressions.
      // If a value has been assigned, returns a copy of the expression.
      // For pins, the "Driver" component is used.
      // Returns null on error, or when no value has been assigned yet.
      virtual AST::EXPRESSION* GetExpression(int Line, const std::string& Filename) = 0;

      // Assigns the expression, taking ownership (i.e. will delete later)
      virtual bool Assign   (AST::EXPRESSION* Expression) = 0;
      virtual bool RawAssign(AST::EXPRESSION* Expression) = 0;

      // Generates a name based on the parent group(s), if it is not directly
      // part of the module.  This is the name used in the output HDL.
      std::string& HDL_Name();
      std::string& EscapedName(); // Inserts '\' in front if required

      virtual void Display        (int Indent = 0) = 0;
              void DisplayLongName();

      // Access the attribute or member object
      // Only searches this object and returns null when not found
      // It returns the original, not a copy; used to modify the original
      virtual BASE* GetAttribute(const std::string& Name);
      virtual BASE* GetMember   (const std::string& Name);

      // Access the attribute, but searches up to the root and
      // returns null when not found
      virtual AST::EXPRESSION* GetAttribValue(const std::string& Name);

      // If the named built-in attribute exists, creates a new EXPRESSION node
      // and populates in appropriately.  Returns null otherwise.
      virtual AST::EXPRESSION* GetBuiltInAttributeValue(const std::string& Name);

      // Returns the number of bits when synthesising the object.
      // Returns 0 by default, indicating an invalid question.
      virtual int     Width    ();
      virtual NUMBER& FullScale();
      virtual bool    Signed   ();

      virtual bool HasCircularReference(BASE* Object) = 0;

      // Populates the "Used" flag so that the back-end can remove unused objects.
      // SetUsed means that this node must be marked as "Used".  When calling
      // from AST::EXPRESSION, this is true; when calling from BACK_END, this
      // is false.
      virtual void PopulateUsed(bool SetUsed);

      // Runs assertions to validate the members.
      virtual void Validate();
  };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

