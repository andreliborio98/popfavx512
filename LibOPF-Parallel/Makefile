#!/usr/bin/make

all: parallel paravx parsimd parseq

parallel:
	$(MAKE) -f Makefile.generic BUILD=parallel

paravx:
	$(MAKE) -f Makefile.generic BUILD=paravx

parsimd:
	$(MAKE) -f Makefile.generic BUILD=parsimd

parseq:
	$(MAKE) -f Makefile.generic BUILD=parseq

clean: clean-parallel clean-paravx clean-parsimd clean-parseq

clean-parallel:
	$(MAKE) -f Makefile.generic BUILD=parallel clean

clean-paravx:
	$(MAKE) -f Makefile.generic BUILD=paravx clean

clean-parsimd:
	$(MAKE) -f Makefile.generic BUILD=parsimd clean

clean-parseq:
	$(MAKE) -f Makefile.generic BUILD=parseq clean
