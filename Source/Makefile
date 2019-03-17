ifeq ($(OS), Windows_NT)
  CompilerPath = C:/TDM-GCC-32/bin
  CXX          = $(CompilerPath)/g++
else
  CompilerPath = 
  CXX          = g++
endif

Options = -std=c++11 -Wall -fexceptions -O2 -DDEBUG
#-------------------------------------------------------------------------------

Toolbox = ../Toolbox/Cpp
#-------------------------------------------------------------------------------

Version  = -DMAJOR_VERSION=0 -DMINOR_VERSION=1
#-------------------------------------------------------------------------------

Libraries  = -lfftw3 -lgmp -lmpfr

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
Headers += DataStructures/Objects/*.h

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

Objects  = $(ObjectPath)/Tools/Number.o

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

Objects += $(ObjectPath)/DataStructures/Objects/Base.o
Objects += $(ObjectPath)/DataStructures/Objects/Alias.o
Objects += $(ObjectPath)/DataStructures/Objects/Array.o
Objects += $(ObjectPath)/DataStructures/Objects/Byte.o
Objects += $(ObjectPath)/DataStructures/Objects/Character.o
Objects += $(ObjectPath)/DataStructures/Objects/Expression.o
Objects += $(ObjectPath)/DataStructures/Objects/Namespace.o
Objects += $(ObjectPath)/DataStructures/Objects/Group.o
Objects += $(ObjectPath)/DataStructures/Objects/Module.o
Objects += $(ObjectPath)/DataStructures/Objects/Synthesisable.o
Objects += $(ObjectPath)/DataStructures/Objects/Net.o
Objects += $(ObjectPath)/DataStructures/Objects/Pin.o

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

TestSources  = ../Test\ Cases/*
TestSources += ../Test\ Cases/Target/*
TestSources += ../Test\ Cases/FrontEnd/*
TestSources += ../Test\ Cases/ButtonsToLEDs/*
TestSources += ../Test\ Cases/Counter/*
TestSources += ../Test\ Cases/MAX_10_ADC/*
TestSources += ../Test\ Cases/Pipeline/*
TestSources += ../Test\ Cases/Scripting/*
#-------------------------------------------------------------------------------

.PHONY: clean all
.SECONDARY:

ifeq ($(OS), Windows_NT)
  ext = .exe
endif

all: bin/ALCHA$(ext)

clean:
	rm -rf bin
	rm -rf $(ObjectPath)
	rm FrontEnd/CharacterNames.h

test: $(TestSources) testScanner testParser

test%: ../Test\ Cases/%/main.alc bin/ALCHA$(ext) $(TestSources)
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
	$(CompilerPath)/windres.exe  -J rc -O coff -i $< -o $@
#-------------------------------------------------------------------------------

