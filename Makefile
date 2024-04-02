CC:=cc
CFLAGS:=-pedantic -std=c99 -O3 -Wall -Wextra -Wno-unused-result
SHELL:=bash

.PHONY: all clean

all: build/bin/left-prompt build/bin/right-prompt

clean:
	rm -rf build;

build/bin/%-prompt: src/%-prompt.c src/%-prompt.h
	mkdir -p build/bin;
	${CC} ${CFLAGS} -o${@} ${<};
