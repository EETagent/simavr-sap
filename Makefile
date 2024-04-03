# Makefile

# Normal build will link against the shared library for simavr
# in the current build tree, so you don't have to 'install' to
# run simavr or the examples.
#
# For package building, you will need to pass RELEASE=1 to make
RELEASE	?= 0

DESTDIR = /usr/local
PREFIX = ${DESTDIR}

.PHONY: doc

all:	build-simavr build-sap

build-simavr:
	$(MAKE) -C simavr RELEASE=$(RELEASE)


build-sap: build-simavr
	$(MAKE) -C sap RELEASE=$(RELEASE)


install: install-simavr install-parts

install-simavr:
	$(MAKE) -C simavr install RELEASE=$(RELEASE) DESTDIR=$(DESTDIR) PREFIX=$(PREFIX)

doc:
	$(MAKE) -C doc RELEASE=$(RELEASE)

clean:
	$(MAKE) -C simavr clean
	$(MAKE) -C sap clean
	$(MAKE) -C doc clean

