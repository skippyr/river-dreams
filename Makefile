CC:=cc
CFLAGS:=-std=c99 -Wpedantic -Wall -Wextra -Wno-unused-result -O3
SHELL:=bash

.PHONY: all clean

all: build/bin/left-prompt build/bin/right-prompt

clean:
	rm -rf build;

build/bin/%-prompt: src/%-prompt.c
	mkdir -p build/bin;
	${CC} ${CFLAGS} -o ${@} ${<};
