.PHONY: clean all

all:
	$(MAKE) -C Source all

clean:
	$(MAKE) -C Source clean
	$(MAKE) -C Tests  clean

test:
	$(MAKE) -C Tests test
#-------------------------------------------------------------------------------

