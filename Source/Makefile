ifeq ($(OS), Windows_NT)
  # libgmp and libmpfr for Windows only works under 32-bit compilation
  CXX     = mingw32-g++
  windres = c:/TDM-GCC-32/bin/windres.exe

  # CXX     = g++
  # windres = windres
else
  CXX = g++
endif

Options  = -std=c++11
Options += -fdiagnostics-color=always
Options += -Wall
Options += -O2
Options += -DDEBUG
#-------------------------------------------------------------------------------

Toolbox = ../Toolbox/Cpp
#-------------------------------------------------------------------------------

Version  = -DMAJOR_VERSION=0 -DMINOR_VERSION=1
#-------------------------------------------------------------------------------

Libraries  = -lgmp
Libraries += -lmpfr
# Libraries += -lfftw3

ifeq ($(OS), Windows_NT)
  Includes   = -ILibraries/include
  LibInclude = -LLibraries/lib
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

Headers  = Libraries/include/*.h

Headers += Tools/*.h
Headers += $(Toolbox)/*.h          
Headers += DataStructures/*.h
Headers += DataStructures/AST/*.h
Headers += DataStructures/Netlist/*.h

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

Objects  = $(ObjectPath)/Tools/Logger.o
Objects += $(ObjectPath)/Tools/Number.o

Objects += $(ObjectPath)/Toolbox/General.o
Objects += $(ObjectPath)/Toolbox/Dictionary.o
Objects += $(ObjectPath)/Toolbox/FileWrapper.o
Objects += $(ObjectPath)/Toolbox/UTF_Converter.o

Objects += $(ObjectPath)/DataStructures/TokenTree.o

Objects += $(ObjectPath)/DataStructures/AST/Base.o
Objects += $(ObjectPath)/DataStructures/AST/Alias.o
Objects += $(ObjectPath)/DataStructures/AST/Assignment.o
Objects += $(ObjectPath)/DataStructures/AST/ClassDefinition.o
Objects += $(ObjectPath)/DataStructures/AST/Definition.o
Objects += $(ObjectPath)/DataStructures/AST/EnumDefinition.o
Objects += $(ObjectPath)/DataStructures/AST/Expression.o
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
Objects += $(ObjectPath)/DataStructures/AST/Parameter.o
Objects += $(ObjectPath)/DataStructures/AST/RTL.o
Objects += $(ObjectPath)/DataStructures/AST/Switch.o
Objects += $(ObjectPath)/DataStructures/AST/WhileLoop.o

Objects += $(ObjectPath)/DataStructures/Netlist/Base.o
Objects += $(ObjectPath)/DataStructures/Netlist/Alias.o
Objects += $(ObjectPath)/DataStructures/Netlist/Array.o
Objects += $(ObjectPath)/DataStructures/Netlist/Byte.o
Objects += $(ObjectPath)/DataStructures/Netlist/Character.o
Objects += $(ObjectPath)/DataStructures/Netlist/Expression.o
Objects += $(ObjectPath)/DataStructures/Netlist/Namespace.o
Objects += $(ObjectPath)/DataStructures/Netlist/Num.o
Objects += $(ObjectPath)/DataStructures/Netlist/Group.o
Objects += $(ObjectPath)/DataStructures/Netlist/Module.o
Objects += $(ObjectPath)/DataStructures/Netlist/Synthesisable.o
Objects += $(ObjectPath)/DataStructures/Netlist/Net.o
Objects += $(ObjectPath)/DataStructures/Netlist/Pin.o

Objects += $(ObjectPath)/FrontEnd/Parser.o
Objects += $(ObjectPath)/FrontEnd/Scanner.o
Objects += $(ObjectPath)/FrontEnd/Token.o

Objects += $(ObjectPath)/Engine/Engine.o

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

test: $(TestSources) testScanner testParser

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

