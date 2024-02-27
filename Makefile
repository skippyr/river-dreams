CC:=cc
CFLAGS:=-std=c99 -pedantic -Os -Wall -Wextra
SHELL:=bash

.PHONY: all clean

all: out/left-prompt out/right-prompt

clean:
	rm -rf out;

out/%-prompt: src/%-prompt.c
	mkdir -p out;
	${CC} ${CFLAGS} -o${@} ${^};
