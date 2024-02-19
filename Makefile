CXX:=g++
CXXFLAGS:=-std=c++14 -O3 -Wall -Wextra

.PHONY: all clean

all: bin/left-prompt bin/right-prompt

clean:
	rm -rf bin;

bin/%-prompt: src/%-prompt.cpp
	mkdir -p bin;
	${CXX} ${CXXFLAGS} -o${@} ${^};
