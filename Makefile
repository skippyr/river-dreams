BINPATH:=/usr/local/bin

.PHONY: all install uninstall

all: river-dreams

install: all
	mkdir -p ${BINPATH}
	mv river-dreams ${BINPATH}

uninstall:
	rm -f ${BINPATH}/river-dreams

river-dreams: river-dreams.c
	cc -std=c99 -pedantic -Os -Wall -o${@} ${^}
