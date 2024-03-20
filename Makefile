CXX:=g++
CXXFLAGS:=-std=c++20 -O3 -Wno-unused-result
SHELL:=bash

.PHONY: all clean

all: bin/left-prompt bin/right-prompt

clean:
	rm -rf bin;

bin/%-prompt: src/%-prompt.cpp src/%-prompt.hpp
	mkdir -p bin;
	${CXX} ${CXXFLAGS} -o${@} ${^};
