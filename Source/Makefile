ifeq ($(OS), Windows_NT)
  CXX      = x86_64-w64-mingw32-g++.exe
  Options += -fdiagnostics-color=always
  windres  = windres
else
  CXX = g++
endif

Options += -std=c++20
Options += -Wall
Options += -O2
Options += -DDEBUG
#-------------------------------------------------------------------------------

Toolbox = ../Toolbox/Cpp
#-------------------------------------------------------------------------------

Version  = -DMAJOR_VERSION=0 -DMINOR_VERSION=2
#-------------------------------------------------------------------------------

# Libraries  = -lgmp
# Libraries += -lmpfr
# Libraries += -lfftw3

ifeq ($(OS), Windows_NT)
  Includes    = -ILibraries/include
  # Headers    += Libraries/include/gmp.h
  # Headers    += Libraries/include/mpfr.h
  # Headers    += Libraries/include/fftw3.h

  LibInclude  = -LLibraries/lib
  # Objects    += Libraries/lib/libgmp.a
  # Objects    += Libraries/lib/libmpfr.a
  # Objects    += Libraries/lib/libfftw3.a
else
  Includes   =
  LibInclude =
endif
#-------------------------------------------------------------------------------

Includes  += -ITools
Includes  += -I$(Toolbox)
Includes  += -IDataStructures
Includes  += -IFrontEnd
Includes  += -IEngine
Includes  += -IBackEnd
#-------------------------------------------------------------------------------

Headers += Tools/*.h
Headers += $(Toolbox)/*.h
Headers += DataStructures/*.h
Headers += DataStructures/AST/*.h
Headers += DataStructures/AST/Assignment/*.h
Headers += DataStructures/AST/Definition/*.h
Headers += DataStructures/AST/Expression/*.h
Headers += DataStructures/Netlist/*.h
Headers += DataStructures/Netlist/Namespace/*.h
Headers += DataStructures/Netlist/Synthesisable/*.h

Headers += FrontEnd/*.h

Headers += Engine/*.h

Headers += BackEnd/*.h
Headers += BackEnd/Altera/*.h

# Auto-generated headers
Headers += FrontEnd/CharacterNames.h
#-------------------------------------------------------------------------------

ifeq ($(OS), Windows_NT)
  ObjectPath = obj_windows
else
  ObjectPath = obj_linux
endif

Objects += $(ObjectPath)/Tools/Logger.o
Objects += $(ObjectPath)/Tools/Utilities.o
Objects += $(ObjectPath)/Tools/Number.o

Objects += $(ObjectPath)/Toolbox/General.o
Objects += $(ObjectPath)/Toolbox/Dictionary.o
Objects += $(ObjectPath)/Toolbox/FileWrapper.o
Objects += $(ObjectPath)/Toolbox/UTF_Converter.o

Objects += $(ObjectPath)/DataStructures/TokenTree.o

Objects += $(ObjectPath)/DataStructures/AST/Base.o
Objects += $(ObjectPath)/DataStructures/AST/Alias.o
Objects += $(ObjectPath)/DataStructures/AST/Class_Definition.o
Objects += $(ObjectPath)/DataStructures/AST/Enum_Definition.o
Objects += $(ObjectPath)/DataStructures/AST/Fence.o
Objects += $(ObjectPath)/DataStructures/AST/ForLoop.o
Objects += $(ObjectPath)/DataStructures/AST/FSM.o
Objects += $(ObjectPath)/DataStructures/AST/Group.o
Objects += $(ObjectPath)/DataStructures/AST/HDL.o
Objects += $(ObjectPath)/DataStructures/AST/IfStatement.o
Objects += $(ObjectPath)/DataStructures/AST/Import.o
Objects += $(ObjectPath)/DataStructures/AST/Jump.o
Objects += $(ObjectPath)/DataStructures/AST/LoopLoop.o
Objects += $(ObjectPath)/DataStructures/AST/NamespacePush.o
Objects += $(ObjectPath)/DataStructures/AST/RTL.o
Objects += $(ObjectPath)/DataStructures/AST/Switch.o
Objects += $(ObjectPath)/DataStructures/AST/WhileLoop.o

Objects += $(ObjectPath)/DataStructures/AST/Assignment.o
Objects += $(ObjectPath)/DataStructures/AST/Assignment/Add_Assign.o
Objects += $(ObjectPath)/DataStructures/AST/Assignment/AND_Assign.o
Objects += $(ObjectPath)/DataStructures/AST/Assignment/Append_Assign.o
Objects += $(ObjectPath)/DataStructures/AST/Assignment/Assign.o
Objects += $(ObjectPath)/DataStructures/AST/Assignment/Divide_Assign.o
Objects += $(ObjectPath)/DataStructures/AST/Assignment/Exponential_Assign.o
Objects += $(ObjectPath)/DataStructures/AST/Assignment/Modulus_Assign.o
Objects += $(ObjectPath)/DataStructures/AST/Assignment/Multiply_Assign.o
Objects += $(ObjectPath)/DataStructures/AST/Assignment/OR_Assign.o
Objects += $(ObjectPath)/DataStructures/AST/Assignment/Raw_Assign.o
Objects += $(ObjectPath)/DataStructures/AST/Assignment/Shift_Left_Assign.o
Objects += $(ObjectPath)/DataStructures/AST/Assignment/Shift_Right_Assign.o
Objects += $(ObjectPath)/DataStructures/AST/Assignment/Subtract_Assign.o
Objects += $(ObjectPath)/DataStructures/AST/Assignment/XOR_Assign.o

Objects += $(ObjectPath)/DataStructures/AST/Definition.o
Objects += $(ObjectPath)/DataStructures/AST/Definition/Auto_Definition.o
Objects += $(ObjectPath)/DataStructures/AST/Definition/Byte_Definition.o
Objects += $(ObjectPath)/DataStructures/AST/Definition/Char_Definition.o
Objects += $(ObjectPath)/DataStructures/AST/Definition/Class_Instance.o
Objects += $(ObjectPath)/DataStructures/AST/Definition/FuncPtr_Definition.o
Objects += $(ObjectPath)/DataStructures/AST/Definition/Net_Definition.o
Objects += $(ObjectPath)/DataStructures/AST/Definition/Num_Definition.o
Objects += $(ObjectPath)/DataStructures/AST/Definition/Pin_Definition.o
Objects += $(ObjectPath)/DataStructures/AST/Definition/Void_Definition.o

Objects += $(ObjectPath)/DataStructures/AST/Expression.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/AccessAttribute.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/AccessMember.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/AccessMemberSafe.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Add.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/AND_Reduce.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Array.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/ArrayConcatenate.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Bit_AND.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Bit_NAND.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Bit_NOR.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Bit_NOT.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Bit_OR.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Bit_XNOR.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Bit_XOR.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Cast.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Conditional.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Decrement.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Divide.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Equal.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Exponential.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Factorial.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/FunctionCall.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Greater.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Greater_Equal.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Identifier.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Increment.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Less.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Less_Equal.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Literal.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Logical_AND.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Logical_NOT.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Logical_OR.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Modulus.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Multiply.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/NAND_Reduce.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Negate.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/NOR_Reduce.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Not_Equal.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Object.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/OR_Reduce.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Range.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Raw.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Replicate.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Shift_Left.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Shift_Right.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Slice.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/String.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/Subtract.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/VectorConcatenate.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/XNOR_Reduce.o
Objects += $(ObjectPath)/DataStructures/AST/Expression/XOR_Reduce.o

Objects += $(ObjectPath)/DataStructures/Netlist/Base.o
Objects += $(ObjectPath)/DataStructures/Netlist/Alias.o
Objects += $(ObjectPath)/DataStructures/Netlist/Array.o
Objects += $(ObjectPath)/DataStructures/Netlist/Attribute.o
Objects += $(ObjectPath)/DataStructures/Netlist/Byte.o
Objects += $(ObjectPath)/DataStructures/Netlist/Character.o
Objects += $(ObjectPath)/DataStructures/Netlist/Namespace.o
Objects += $(ObjectPath)/DataStructures/Netlist/Namespace/Group.o
Objects += $(ObjectPath)/DataStructures/Netlist/Namespace/Module.o
Objects += $(ObjectPath)/DataStructures/Netlist/Num.o
Objects += $(ObjectPath)/DataStructures/Netlist/Synthesisable.o
Objects += $(ObjectPath)/DataStructures/Netlist/Synthesisable/Net.o
Objects += $(ObjectPath)/DataStructures/Netlist/Synthesisable/Pin.o
Objects += $(ObjectPath)/DataStructures/Netlist/Synthesisable/PinComponent.o

Objects += $(ObjectPath)/FrontEnd/Parser.o
Objects += $(ObjectPath)/FrontEnd/Scanner.o
Objects += $(ObjectPath)/FrontEnd/Token.o

Objects += $(ObjectPath)/Engine/Engine.o
Objects += $(ObjectPath)/Engine/Constants.o

Objects += $(ObjectPath)/BackEnd/BackEnd.o
Objects += $(ObjectPath)/BackEnd/SDC.o
Objects += $(ObjectPath)/BackEnd/Altera/Project.o

ifeq ($(OS), Windows_NT)
  Objects += $(ObjectPath)/Resources/resource.res
endif
#-------------------------------------------------------------------------------

TestSources  = ../TestCases/*
TestSources += ../TestCases/Target/*
TestSources += ../TestCases/FrontEnd/*
TestSources += ../TestCases/ButtonsToLEDs/*
TestSources += ../TestCases/Counter/*
TestSources += ../TestCases/MAX_10_ADC/*
TestSources += ../TestCases/Pipeline/*
TestSources += ../TestCases/Scripting/*
#-------------------------------------------------------------------------------

.PHONY: clean all
.SECONDARY:

ifeq ($(OS), Windows_NT)
  ext = .exe
endif

all: bin/ALCHA$(ext)

clean:
	- rm -rf bin/ALCHA$(ext)
	- rm -rf $(ObjectPath)
	- rm FrontEnd/CharacterNames.h

test: $(TestSources) bin/testScanner$(ext) bin/testParser$(ext)

test%: ../TestCases/%/main.alc bin/ALCHA$(ext) $(TestSources)
	- rm -rf "../Test Output"
	bin/ALCHA$(ext) "$<" "../Test Output"
#-------------------------------------------------------------------------------

# Binaries

bin/%$(ext): %.cpp $(Objects) $(Headers)
	mkdir -p bin
	$(CXX) $(Options) $(Version) $(Includes) $< $(Objects) -s $(LibInclude) $(Libraries) -o $@
#-------------------------------------------------------------------------------

# Objects

$(ObjectPath)/%.o: %.cpp $(Headers)
	mkdir -p $(@D)
	$(CXX) $(Options) $(Version) $(Defines) -c $(Includes) $< -o $@

$(ObjectPath)/Toolbox/%.o: $(Toolbox)/%.cpp $(Headers)
	mkdir -p $(@D)
	$(CXX) $(Options) $(Version) $(Defines) -c $(Includes) $< -o $@
#-------------------------------------------------------------------------------

# Autogen

FrontEnd/CharacterNames.h: FrontEnd/CharacterNames.json FrontEnd/CharacterNames.py
	cd $(@D); py CharacterNames.py
#-------------------------------------------------------------------------------

# Resources

$(ObjectPath)/%.res: %.rc
	mkdir -p $(@D)
	$(windres) -J rc -O coff -i $< -o $@
#-------------------------------------------------------------------------------

