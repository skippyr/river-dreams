CC:=cc
CFLAGS:=-std=c99 -pedantic -Os -Wall -Wextra

.PHONY: all clean

all: bin/left-prompt bin/right-prompt

clean:
	rm -rf bin;

bin/%-prompt: src/%-prompt.c
	mkdir -p bin;
	${CC} ${CFLAGS} -o${@} ${^};
