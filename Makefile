CC:=cc
CFLAGS:=-std=c99 -pedantic -Os -Wall
BINPATH:=/usr/local/bin

.PHONY: all clean install uninstall

all: riverdreams

clean:
	rm -f riverdreams;

install: all
	mkdir -p ${BINPATH};
	cp riverdreams ${BINPATH};

uninstall:
	rm -f ${BINPATH}/riverdreams;

riverdreams: riverdreams.c
	${CC} ${CFLAGS} -o${@} ${^};
