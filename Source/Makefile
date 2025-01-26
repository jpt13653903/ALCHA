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

Toolbox = ../Toolbox/Cpp
#-------------------------------------------------------------------------------

Version  = -DMAJOR_VERSION=0 -DMINOR_VERSION=2
#-------------------------------------------------------------------------------

Libraries  = -lgmp
# Libraries += -lmpfr
# Libraries += -lfftw3

ifeq ($(OS), Windows_NT)
  Includes    = -ILibraries/include
  Headers    += Libraries/include/gmp.h
  # Headers    += Libraries/include/mpfr.h
  # Headers    += Libraries/include/fftw3.h

  LibInclude  = -LLibraries/lib
  Objects    += Libraries/lib/libgmp.a
  # Objects    += Libraries/lib/libmpfr.a
  # Objects    += Libraries/lib/libfftw3.a
else
  Includes   =
  LibInclude =
endif
#-------------------------------------------------------------------------------

Includes += -I.
Includes += -IAST
Includes += -IFrontEnd
Includes += -ITools
Includes += -IObjects
Includes += -I$(Toolbox)
#-------------------------------------------------------------------------------

# Auto-generated headers
Headers += FrontEnd/CharacterNames.h
#-------------------------------------------------------------------------------

Source  += $(shell find . -iname "*.cpp")
Headers += $(shell find . -iname "*.h")

ToolboxSource = $(shell find $(Toolbox) -iname "*.cpp")
Headers      += $(shell find $(Toolbox) -iname "*.h")
#-------------------------------------------------------------------------------

ifeq ($(OS), Windows_NT)
  ObjectPath = ../obj_windows
else
  ObjectPath = ../obj_linux
endif

Objects  = $(Source:%.cpp=$(ObjectPath)/Source/%.o)
Objects += $(ToolboxSource:$(Toolbox)/%.cpp=$(ObjectPath)/Toolbox/%.o)

ifeq ($(OS), Windows_NT)
  Objects += $(ObjectPath)/Resources/resource.res
endif
#-------------------------------------------------------------------------------

.PHONY: clean all
.SECONDARY:

all: ../bin/alcha$(ext)

clean:
	- rm -rf ../bin/alcha$(ext)
	- rm -rf $(ObjectPath)
	- rm FrontEnd/CharacterNames.h
#-------------------------------------------------------------------------------

# Binaries

../bin/%$(ext): %.cpp $(Objects) $(Headers)
	mkdir -p ../bin
	$(CXX) $(Options) $(Objects) -s $(LibInclude) $(Libraries) -o $@
#-------------------------------------------------------------------------------

# Objects

$(ObjectPath)/Source/%.o: %.cpp $(Headers)
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

