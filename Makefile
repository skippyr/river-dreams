CC:=cc
CFLAGS:=-std=c99 -pedantic -Os -Wall
BINPATH:=/usr/local/bin

.PHONY: all clean

all: out/lprompt out/rprompt

clean:
	rm -rf out;

out/%prompt: src/%prompt.c
	mkdir -p out;
	${CC} ${CFLAGS} -o${@} ${^};
