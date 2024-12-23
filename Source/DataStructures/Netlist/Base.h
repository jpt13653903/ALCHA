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
    class Base;
    class Assignment;
    class Expression;
}
//------------------------------------------------------------------------------

namespace Netlist{
    class Attribute;
    class NameSpace;
    class Synthesisable;

    class Base{ // Base class for the symbol table
        private:
            bool temporary;
        //--------------------------------------------------------------------------

        protected:
            void displayAttributes(int indent);
        //--------------------------------------------------------------------------

        public:
            // Indentation follows the inheritance tree
            enum class Type{
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
            } type;

            virtual bool isSynthesisable();
            virtual bool isNamespace    ();

            // The line and filename that resulted in this object, used for error reporting
            struct{
                int         line;
                std::string filename;
            } source;

            std::string name;
            NameSpace*  nameSpace;
            std::map<std::string, Attribute*> attributes;

            bool isTemporary();
        //--------------------------------------------------------------------------

        public:
            // When Name is 0, a name is auto-generated
                     Base(int line, const std::string& filename, const char* name, Type type);
            virtual ~Base();

            void printError  (const char* message = 0);
            void printWarning(const char* message = 0);

            // Called by the definition to assign the attribute list
            bool applyAttributes(AST::Assignment* attributeList);

            // Used specifically for operate-and-assign expressions.
            // If a value has been assigned, returns a copy of the expression.
            // For pins, the "Driver" component is used.
            // Returns null on error, or when no value has been assigned yet.
            virtual AST::Expression* getExpression(int line, const std::string& filename) = 0;

            // Assigns the expression, taking ownership (i.e. will delete later)
            virtual bool assign   (AST::Expression* expression) = 0;
            virtual bool rawAssign(AST::Expression* expression) = 0;

            // Generates a name based on the parent group(s), if it is not directly
            // part of the module.  This is the name used in the output HDL.
            std::string& hdlName();
            std::string& escapedName(); // Inserts '\' in front if required

            virtual void display        (int indent = 0) = 0;
                    void displayLongName();

            // Access the attribute or member object
            // Only searches this object and returns null when not found
            // It returns the original, not a copy; used to modify the original
            virtual Base* getAttribute(const std::string& name);
            virtual Base* getMember   (const std::string& name);

            // Access the attribute, but searches up to the root and
            // returns null when not found
            virtual AST::Expression* getAttribValue(const std::string& name);

            // If the named built-in attribute exists, creates a new Expression node
            // and populates in appropriately.  Returns null otherwise.
            virtual AST::Expression* getBuiltInAttributeValue(const std::string& name);

            // Returns the number of bits when synthesising the object.
            // Returns 0 by default, indicating an invalid question.
            virtual int     width    ();
            virtual Number& fullScale();
            virtual bool    isSigned ();

            virtual bool hasCircularReference(Base* object) = 0;

            // Populates the "Used" flag so that the back-end can remove unused objects.
            // SetUsed means that this node must be marked as "Used".  When calling
            // from AST::Expression, this is true; when calling from BackEnd, this
            // is false.
            virtual void populateUsed(bool setUsed);

            // Runs assertions to validate the members.
            virtual void validate();
    };
}
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------

