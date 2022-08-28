#!/usr/bin/make

all: list listavx listsimd

listavx:
	$(MAKE) -f Makefile.generic BUILD=listavx

listsimd:
	$(MAKE) -f Makefile.generic BUILD=listsimd

list:
	$(MAKE) -f Makefile.generic BUILD=list

clean: clean-list clean-listavx clean-listsimd

clean-listavx:
	$(MAKE) -f Makefile.generic BUILD=listavx clean

clean-listsimd:
	$(MAKE) -f Makefile.generic BUILD=listsimd clean

clean-list:
	$(MAKE) -f Makefile.generic BUILD=list clean
