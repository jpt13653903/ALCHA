ifeq ($(OS), Windows_NT)
  make = mingw32-make
else
  make = make
endif
#-------------------------------------------------------------------------------

.PHONY: clean all

all:
	$(make) -C Source all

clean:
	$(make) -C Source clean

test:
	$(make) -C Source test
#-------------------------------------------------------------------------------

