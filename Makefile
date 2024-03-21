CC:=cc
CFLAGS:=-pedantic -std=c99 -O3 -Wall -Wextra -Wno-unused-result
SHELL:=bash

.PHONY: all clean

all: bin/left-prompt bin/right-prompt

clean:
	rm -rf bin;

bin/%-prompt: src/%-prompt.c src/%-prompt.h
	mkdir -p bin;
	${CC} ${CFLAGS} -o${@} ${^};
