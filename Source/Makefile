CC      = g++
Options = -Wall -fexceptions -O2
#-------------------------------------------------------------------------------

Includes   = -I../Toolbox/Cpp            \
             -ILibraries/include         \
             -ITools                     \
             -IGlobal                    \
             -IBackEnd                   \
             -IFrontEnd                  \
             -IDataStructures            \
             -IDataStructures/AST        \
             -IDataStructures/SymbolTable

Libraries  = -lfftw3 -lgmp -lmpfr
LibInclude = -LLibraries/lib
#-------------------------------------------------------------------------------

Version = -DMAJOR_VERSION=1 -DMINOR_VERSION=0

Objects = obj/Altera.o               \
          obj/AST.o                  \
          obj/AST_Alias.o            \
          obj/AST_Assignment.o       \
          obj/AST_ClassDefinition.o  \
          obj/AST_Definition.o       \
          obj/AST_EnumDefinition.o   \
          obj/AST_Expression.o       \
          obj/AST_Fence.o            \
          obj/AST_ForLoop.o          \
          obj/AST_FSM.o              \
          obj/AST_Group.o            \
          obj/AST_HDL.o              \
          obj/AST_IfStatement.o      \
          obj/AST_Import.o           \
          obj/AST_Jump.o             \
          obj/AST_LoopLoop.o         \
          obj/AST_NamespacePush.o    \
          obj/AST_Parameter.o        \
          obj/AST_RTL.o              \
          obj/AST_Switch.o           \
          obj/AST_TargetDefinition.o \
          obj/AST_WhileLoop.o        \
          obj/Clock.o                \
          obj/ClockTree.o            \
          obj/Dictionary.o           \
          obj/FileSystem.o           \
          obj/HDL.o                  \
          obj/IdentifierTree.o       \
          obj/MemoryTable.o          \
          obj/MyString.o             \
          obj/Net.o                  \
          obj/Number.o               \
          obj/Object.o               \
          obj/Parser.o               \
          obj/Pin.o                  \
          obj/Reference.o            \
          obj/resource.res           \
          obj/Scanner.o              \
          obj/Scope.o                \
          obj/SymbolTree.o           \
          obj/Target.o               \
          obj/TimingSpec.o           \
          obj/Token.o                \
          obj/TokenTree.o

Headers = ../Toolbox/Cpp/*.h     \
          Libraries/include/*.h  \
          Tools/*.h              \
          Global/*.h             \
          BackEnd/*.h            \
          FrontEnd/*.h           \
          DataStructures/*.h     \
          DataStructures/AST/*.h \
          DataStructures/SymbolTable/*.h
#-------------------------------------------------------------------------------

.PHONY:    clean all
.PRECIOUS: $(Objects)

all: bin/ALCHA.exe

clean:
	rm -rf bin obj
#-------------------------------------------------------------------------------

# Binaries

bin/ALCHA.exe: main.cpp main.h $(Objects) $(Headers)
	mkdir -p bin
	$(CC) $(Options) $(Version) $(Includes) $< $(Objects) -s $(LibInclude) $(Libraries) -o $@
#-------------------------------------------------------------------------------

# Objects

obj/%.o: ../Toolbox/Cpp/%.cpp $(Headers)
	mkdir -p obj
	$(CC) $(Options) $(Version) $(Defines) -c $(Includes) $< -o $@

obj/%.o: Libraries/include/%.cpp $(Headers)
	mkdir -p obj
	$(CC) $(Options) $(Version) $(Defines) -c $(Includes) $< -o $@

obj/%.o: Tools/%.cpp $(Headers)
	mkdir -p obj
	$(CC) $(Options) $(Version) $(Defines) -c $(Includes) $< -o $@

obj/%.o: Global/%.cpp $(Headers)
	mkdir -p obj
	$(CC) $(Options) $(Version) $(Defines) -c $(Includes) $< -o $@

obj/%.o: BackEnd/%.cpp $(Headers)
	mkdir -p obj
	$(CC) $(Options) $(Version) $(Defines) -c $(Includes) $< -o $@

obj/%.o: FrontEnd/%.cpp $(Headers)
	mkdir -p obj
	$(CC) $(Options) $(Version) $(Defines) -c $(Includes) $< -o $@

obj/%.o: DataStructures/%.cpp $(Headers)
	mkdir -p obj
	$(CC) $(Options) $(Version) $(Defines) -c $(Includes) $< -o $@

obj/%.o: DataStructures/AST/%.cpp $(Headers)
	mkdir -p obj
	$(CC) $(Options) $(Version) $(Defines) -c $(Includes) $< -o $@

obj/%.o: DataStructures/SymbolTable/%.cpp $(Headers)
	mkdir -p obj
	$(CC) $(Options) $(Version) $(Defines) -c $(Includes) $< -o $@
#-------------------------------------------------------------------------------

# Resources

obj/%.res: Resources/%.rc
	windres.exe  -J rc -O coff -i $< -o $@
#-------------------------------------------------------------------------------

