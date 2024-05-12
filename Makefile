CXX:=g++
CXXFLAGS:=-std=c++14 -Wall -Wextra -Wno-unused-result -O3
SHELL:=bash

.PHONY: all clean

all: build/bin/left-prompt build/bin/right-prompt

clean:
	rm -rf build;

build/bin/%-prompt: src/%-prompt.cpp src/%-prompt.hpp
	mkdir -p build/bin;
	${CXX} ${CXXFLAGS} -o ${@} ${<};
