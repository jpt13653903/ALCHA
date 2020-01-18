.PHONY: clean all

all:
	$(MAKE) -C Source all

clean:
	$(MAKE) -C Source clean

test:
	$(MAKE) -C Source test
#-------------------------------------------------------------------------------

