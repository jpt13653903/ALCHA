.PHONY: clean all

all:
	make -C Source all

clean:
	make -C Source clean

test: all
	cd Test\ Cases/FrontEnd; ../../Source/bin/ALCHA Scanner.alc
#-------------------------------------------------------------------------------

