ifeq ($(OS), Windows_NT)
  CXX      = x86_64-w64-mingw32-g++.exe
  Options += -fdiagnostics-color=always
  windres  = windres
  ext      = .exe
else
  CXX = g++
endif

Options += -std=c++20
Options += -Wall
Options += -O2
Options += -DDEBUG
#-------------------------------------------------------------------------------

Source  = ../Source
Toolbox = ../Toolbox
#-------------------------------------------------------------------------------

Version  = -DMAJOR_VERSION=0 -DMINOR_VERSION=2
#-------------------------------------------------------------------------------

Libraries  = -lgmp
# Libraries += -lmpfr
# Libraries += -lfftw3

ifeq ($(OS), Windows_NT)
  Includes    = -I$(Source)/Libraries/include
  Headers    += $(Source)/Libraries/include/gmp.h
  # Headers    += $(Source)/Libraries/include/mpfr.h
  # Headers    += $(Source)/Libraries/include/fftw3.h

  LibInclude  = -L$(Source)/Libraries/lib
  Objects    += $(Source)/Libraries/lib/libgmp.a
  # Objects    += $(Source)/Libraries/lib/libmpfr.a
  # Objects    += $(Source)/Libraries/lib/libfftw3.a
else
  Includes   =
  LibInclude =
endif
#-------------------------------------------------------------------------------

Includes += -I$(Source)
Includes += -I$(Source)/AST
Includes += -I$(Source)/DataStructures
Includes += -I$(Source)/FrontEnd
Includes += -I$(Source)/Tools
Includes += -I$(Source)/Symbols
Includes += -I$(Toolbox)/Cpp
Includes += -I$(Toolbox)/Test
#-------------------------------------------------------------------------------

Headers += $(shell find $(Source)  -iname "*.h")
Headers += $(shell find $(Toolbox) -iname "*.h")
Headers += $(Source)/FrontEnd/CharacterNames.h
#-------------------------------------------------------------------------------

ifeq ($(OS), Windows_NT)
  ObjectPath = ../obj_windows
else
  ObjectPath = ../obj_linux
endif

Objects  = $(TestSource:%.cpp=$(ObjectPath)/%.o)

ifeq ($(OS), Windows_NT)
  Resources += $(ObjectPath)/Resources/resource.res
endif
#-------------------------------------------------------------------------------

testSource   = $(shell find * -iname "*.cpp")
testBinaries = $(testSource:%.cpp=bin/%$(ext))
#-------------------------------------------------------------------------------

.PHONY: clean all FORCE
.SECONDARY:

all: $(testBinaries)

test: all
	bin/testScanner
	bin/testParser

test%: bin/test%$(ext) FORCE
	$<

clean:
	- rm -rf bin/test*
	- rm -rf $(ObjectPath)
	- rm -rf $(Source)/FrontEnd/CharacterNames.h
#-------------------------------------------------------------------------------

# Binaries

testScannerObjects += $(ObjectPath)/Tests/testScanner.o
testScannerObjects += $(ObjectPath)/Toolbox/General.o
testScannerObjects += $(ObjectPath)/Toolbox/FileWrapper.o
testScannerObjects += $(ObjectPath)/Toolbox/UTF_Converter.o
testScannerObjects += $(ObjectPath)/Source/FrontEnd/Scanner.o
testScannerObjects += $(ObjectPath)/Source/FrontEnd/Token.o
testScannerObjects += $(ObjectPath)/Source/DataStructures/TokenTree.o
testScannerObjects += $(ObjectPath)/Source/Tools/Number.o
testScannerObjects += $(ObjectPath)/Source/Tools/Utilities.o

bin/testScanner$(ext): $(testScannerObjects) $(Resources) $(Headers)
	mkdir -p bin
	$(CXX) $(Options) $(testScannerObjects) $(Resources) -s $(LibInclude) $(Libraries) -o $@
#-------------------------------------------------------------------------------

testParserObjects += $(ObjectPath)/Tests/testParser.o

testParserSource  += $(shell find $(Source)/AST            -iname "*.cpp")
testParserSource  += $(shell find $(Source)/DataStructures -iname "*.cpp")
testParserSource  += $(shell find $(Source)/FrontEnd       -iname "*.cpp")
testParserSource  += $(shell find $(Source)/Symbols        -iname "*.cpp")
testParserSource  += $(shell find $(Source)/Tools          -iname "*.cpp")
testParserObjects += $(testParserSource:$(Source)/%.cpp=$(ObjectPath)/Source/%.o)

testParserObjects += $(ObjectPath)/Toolbox/General.o
testParserObjects += $(ObjectPath)/Toolbox/FileWrapper.o
testParserObjects += $(ObjectPath)/Toolbox/UTF_Converter.o

bin/testParser$(ext): $(testParserObjects) $(Resources) $(Headers)
	mkdir -p bin
	$(CXX) $(Options) $(testParserObjects) $(Resources) -s $(LibInclude) $(Libraries) -o $@
#-------------------------------------------------------------------------------

# Objects

$(ObjectPath)/Tests/%.o: %.cpp $(Headers)
	mkdir -p $(@D)
	$(CXX) $(Options) $(Version) $(Defines) -c $(Includes) $< -o $@

$(ObjectPath)/Source/%.o: $(Source)/%.cpp $(Headers)
	mkdir -p $(@D)
	$(CXX) $(Options) $(Version) $(Defines) -c $(Includes) $< -o $@

$(ObjectPath)/Toolbox/%.o: $(Toolbox)/Cpp/%.cpp $(Headers)
	mkdir -p $(@D)
	$(CXX) $(Options) $(Version) $(Defines) -c $(Includes) $< -o $@
#-------------------------------------------------------------------------------

# Autogen

$(Source)/FrontEnd/CharacterNames.h: $(Source)/FrontEnd/CharacterNames.json $(Source)/FrontEnd/CharacterNames.py
	cd $(@D); py CharacterNames.py
#-------------------------------------------------------------------------------

# Resources

$(ObjectPath)/%.res: $(Source)/%.rc
	mkdir -p $(@D)
	$(windres) -J rc -O coff -i $< -o $@
#-------------------------------------------------------------------------------

