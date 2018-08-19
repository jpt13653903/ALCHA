CC      = g++
Options = -Wall -fexceptions -O2 -DDEBUG
#-------------------------------------------------------------------------------

Toolbox = ../Toolbox/Cpp
#-------------------------------------------------------------------------------

Includes   = -IBackEnd                    \
             -IDataStructures             \
             -IDataStructures/AST         \
             -IDataStructures/SymbolTable \
             -IFrontEnd                   \
             -IGlobal                     \
             -ILibraries/include          \
             -ITools                      \
             -I$(Toolbox)

Libraries  = -lfftw3 -lgmp -lmpfr
LibInclude = -LLibraries/lib
#-------------------------------------------------------------------------------

Version = -DMAJOR_VERSION=1 -DMINOR_VERSION=0

Objects = obj/BackEnd/Altera.o                          \
          obj/BackEnd/ClockTree.o                       \
          obj/BackEnd/HDL.o                             \
          obj/BackEnd/MemoryTable.o                     \
          obj/BackEnd/TimingSpec.o                      \
          obj/DataStructures/IdentifierTree.o           \
          obj/DataStructures/TokenTree.o                \
          obj/DataStructures/AST/AST.o                  \
          obj/DataStructures/AST/AST_Alias.o            \
          obj/DataStructures/AST/AST_Assignment.o       \
          obj/DataStructures/AST/AST_ClassDefinition.o  \
          obj/DataStructures/AST/AST_Definition.o       \
          obj/DataStructures/AST/AST_EnumDefinition.o   \
          obj/DataStructures/AST/AST_Expression.o       \
          obj/DataStructures/AST/AST_Fence.o            \
          obj/DataStructures/AST/AST_ForLoop.o          \
          obj/DataStructures/AST/AST_FSM.o              \
          obj/DataStructures/AST/AST_Group.o            \
          obj/DataStructures/AST/AST_HDL.o              \
          obj/DataStructures/AST/AST_IfStatement.o      \
          obj/DataStructures/AST/AST_Import.o           \
          obj/DataStructures/AST/AST_Jump.o             \
          obj/DataStructures/AST/AST_LoopLoop.o         \
          obj/DataStructures/AST/AST_NamespacePush.o    \
          obj/DataStructures/AST/AST_Parameter.o        \
          obj/DataStructures/AST/AST_RTL.o              \
          obj/DataStructures/AST/AST_Switch.o           \
          obj/DataStructures/AST/AST_TargetDefinition.o \
          obj/DataStructures/AST/AST_WhileLoop.o        \
          obj/DataStructures/SymbolTable/Clock.o        \
          obj/DataStructures/SymbolTable/Net.o          \
          obj/DataStructures/SymbolTable/Object.o       \
          obj/DataStructures/SymbolTable/Pin.o          \
          obj/DataStructures/SymbolTable/Reference.o    \
          obj/DataStructures/SymbolTable/Scope.o        \
          obj/DataStructures/SymbolTable/SymbolTree.o   \
          obj/DataStructures/SymbolTable/Target.o       \
          obj/FrontEnd/Parser.o                         \
          obj/FrontEnd/Scanner.o                        \
          obj/FrontEnd/Token.o                          \
          obj/Tools/FileSystem.o                        \
          obj/Tools/MyString.o                          \
          obj/Tools/Number.o                            \
          obj/Toolbox/General.o                         \
          obj/Toolbox/Dictionary.o                      \
          obj/Resources/resource.res

Headers = Tools/*.h                      \
          BackEnd/*.h                    \
          FrontEnd/*.h                   \
          DataStructures/*.h             \
          DataStructures/AST/*.h         \
          DataStructures/SymbolTable/*.h \
          Libraries/include/*.h          \
          $(Toolbox)/*.h          

# Auto-generated headers
Headers += FrontEnd/CharacterNames.h
#-------------------------------------------------------------------------------

.PHONY: clean all
.SECONDARY:

all: bin/ALCHA.exe

clean:
	rm -rf bin
	rm -rf obj
	rm FrontEnd/CharacterNames.h

test: testScanner \
      testParser

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
	windres.exe  -J rc -O coff -i $< -o $@
#-------------------------------------------------------------------------------

