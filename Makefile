CXX:=g++
CXXFLAGS:=-std=c++14 -O3 -Wall -Wextra

.PHONY: all clean

all: out/left-prompt out/right-prompt

clean:
	rm -rf out;

out/%-prompt: src/%-prompt.cpp
	mkdir -p out;
	${CXX} ${CXXFLAGS} -o${@} ${^};
