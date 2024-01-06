CC:=cc
CFLAGS:=-std=c99 -pedantic -Os -Wall
BINPATH:=/usr/local/bin

.PHONY: all clean install uninstall

all: river-dreams

clean:
	rm -f river-dreams;

install: all
	mkdir -p ${BINPATH};
	cp river-dreams ${BINPATH};

uninstall:
	rm -f ${BINPATH}/river-dreams;

river-dreams: river-dreams.c
	${CC} ${CFLAGS} -o${@} ${^};
