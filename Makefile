CC:=cc
CFLAGS:=-std=c99 -pedantic -O3 -Wall -Wextra -Wno-unused-result
SHELL:=bash

.PHONY: all clean

all: out/left-prompt out/right-prompt

clean:
	rm -rf out;

out/%-prompt: src/%-prompt.c
	mkdir -p out;
	${CC} ${CFLAGS} -o${@} ${^};
