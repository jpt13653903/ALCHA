CompilerPath = C:/TDM-GCC-32/bin

CC      = $(CompilerPath)/g++
Options = -std=c++11 -Wall -fexceptions -O2 -DDEBUG
#-------------------------------------------------------------------------------

Toolbox = ../Toolbox/Cpp
#-------------------------------------------------------------------------------

Includes   = -IBackEnd
Includes  +=-IDataStructures
Includes  +=-IFrontEnd
Includes  +=-IGlobal
Includes  +=-ILibraries/include
Includes  +=-ITools
Includes  +=-I$(Toolbox)

Libraries  = -lfftw3 -lgmp -lmpfr
LibInclude = -LLibraries/lib
#-------------------------------------------------------------------------------

Version  = -DMAJOR_VERSION=1 -DMINOR_VERSION=0

# Objects  = obj/BackEnd/Altera.o
# Objects += obj/BackEnd/ClockTree.o
# Objects += obj/BackEnd/HDL.o
# Objects += obj/BackEnd/MemoryTable.o
# Objects += obj/BackEnd/TimingSpec.o
# Objects += obj/DataStructures/IdentifierTree.o
Objects += obj/DataStructures/TokenTree.o
Objects += obj/DataStructures/AST/Base.o
Objects += obj/DataStructures/AST/Alias.o
Objects += obj/DataStructures/AST/Assignment.o
Objects += obj/DataStructures/AST/ClassDefinition.o
Objects += obj/DataStructures/AST/Definition.o
Objects += obj/DataStructures/AST/EnumDefinition.o
Objects += obj/DataStructures/AST/Expression.o
Objects += obj/DataStructures/AST/Fence.o
Objects += obj/DataStructures/AST/ForLoop.o
Objects += obj/DataStructures/AST/FSM.o
Objects += obj/DataStructures/AST/Group.o
Objects += obj/DataStructures/AST/HDL.o
Objects += obj/DataStructures/AST/IfStatement.o
Objects += obj/DataStructures/AST/Import.o
Objects += obj/DataStructures/AST/Jump.o
Objects += obj/DataStructures/AST/LoopLoop.o
Objects += obj/DataStructures/AST/NamespacePush.o
Objects += obj/DataStructures/AST/Parameter.o
Objects += obj/DataStructures/AST/RTL.o
Objects += obj/DataStructures/AST/Switch.o
Objects += obj/DataStructures/AST/WhileLoop.o
# Objects += obj/DataStructures/SymbolTable/Scope.o
# Objects += obj/DataStructures/SymbolTable/SymbolTree.o
# Objects += obj/DataStructures/SymbolTable/Object.o
# Objects += obj/DataStructures/SymbolTable/Object_ClassDefinition.o
# Objects += obj/DataStructures/SymbolTable/Object_ClassInstance.o
# Objects += obj/DataStructures/SymbolTable/Object_FunctionDefinition.o
# Objects += obj/DataStructures/SymbolTable/Object_FunctionInstance.o
# Objects += obj/DataStructures/SymbolTable/Object_Array.o
# Objects += obj/DataStructures/SymbolTable/Object_Synthesisable.o
# Objects += obj/DataStructures/SymbolTable/Object_Pin.o
# Objects += obj/DataStructures/SymbolTable/Object_Net.o
# Objects += obj/DataStructures/SymbolTable/Object_Number.o
# Objects += obj/DataStructures/SymbolTable/Object_Byte.o
# Objects += obj/DataStructures/SymbolTable/Object_Character.o
Objects += obj/FrontEnd/Parser.o
Objects += obj/FrontEnd/Scanner.o
Objects += obj/FrontEnd/Token.o
Objects += obj/Tools/Number.o
Objects += obj/Toolbox/General.o
Objects += obj/Toolbox/Dictionary.o
Objects += obj/Toolbox/FileWrapper.o
Objects += obj/Toolbox/UTF_Converter.o
Objects += obj/Resources/resource.res

Headers  = Tools/*.h
# Headers += BackEnd/*.h
Headers += FrontEnd/*.h
Headers += DataStructures/*.h
Headers += DataStructures/AST/*.h
# Headers += DataStructures/SymbolTable/*.h
Headers += Libraries/include/*.h
Headers += $(Toolbox)/*.h          

# Auto-generated headers
Headers += FrontEnd/CharacterNames.h
#-------------------------------------------------------------------------------

TestSources  = ../Test\ Cases/*
TestSources += ../Test\ Cases/Target/*
TestSources += ../Test\ Cases/FrontEnd/*
TestSources += ../Test\ Cases/Buttons\ to\ LEDs/*
TestSources += ../Test\ Cases/Counter/*
#-------------------------------------------------------------------------------

.PHONY: clean all
.SECONDARY:

all: bin/ALCHA.exe

clean:
	rm -rf bin
	rm -rf obj
	rm FrontEnd/CharacterNames.h

test: $(TestSources) testScanner testParser

testButtonsToLEDs: bin/testScanner.exe bin/testParser.exe $(TestSources)
	bin/testScanner.exe "../Test Cases/Buttons to LEDs/main.alc"
	bin/testParser.exe  "../Test Cases/Buttons to LEDs/main.alc"

test%: bin/test%.exe
	$<
#-------------------------------------------------------------------------------

# Binaries

bin/%.exe: %.cpp $(Objects) $(Headers)
	mkdir -p bin
	$(CC) $(Options) $(Version) $(Includes) $< $(Objects) -s $(LibInclude) $(Libraries) -o $@
#-------------------------------------------------------------------------------

# Objects

obj/%.o: %.cpp $(Headers)
	mkdir -p $(@D)
	$(CC) $(Options) $(Version) $(Defines) -c $(Includes) $< -o $@

obj/Toolbox/%.o: $(Toolbox)/%.cpp $(Headers)
	mkdir -p $(@D)
	$(CC) $(Options) $(Version) $(Defines) -c $(Includes) $< -o $@
#-------------------------------------------------------------------------------

# Autogen

FrontEnd/CharacterNames.h: FrontEnd/CharacterNames.json FrontEnd/CharacterNames.py
	cd $(@D); py CharacterNames.py
#-------------------------------------------------------------------------------

# Resources

obj/%.res: %.rc
	mkdir -p $(@D)
	$(CompilerPath)/windres.exe  -J rc -O coff -i $< -o $@
#-------------------------------------------------------------------------------

