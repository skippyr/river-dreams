CC:=cc
CFLAGS:=-std=c99 -pedantic -Os -Wall
BINPATH:=/usr/local/bin

.PHONY: all clean install uninstall

all: riverdreams_lprompt riverdreams_rprompt

clean:
	rm -f riverdreams_{l,r}prompt;

install: all
	mkdir -p ${BINPATH};
	cp riverdreams_{l,r}prompt ${BINPATH};

uninstall:
	rm -f ${BINPATH}/riverdreams_{l,r}prompt;

riverdreams_%: riverdreams_%.c
	${CC} ${CFLAGS} -o${@} ${^};
