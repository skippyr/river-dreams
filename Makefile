SOURCES_DIRECTORY = sources
BINARIES_DIRECTORY = binaries
COMPILER = g++
COMPILER_OPTIONS = -std=c++11 -O3 -Wall -Wextra -Werror

all: binaries

clean:
	rm -rf ${BINARIES_DIRECTORY}

binaries: ${BINARIES_DIRECTORY}/left-prompt                                    \
          ${BINARIES_DIRECTORY}/right-prompt

${BINARIES_DIRECTORY}/%: ${SOURCES_DIRECTORY}/%.cpp
	mkdir -p ${BINARIES_DIRECTORY}
	${COMPILER} ${COMPILER_OPTIONS} -o ${@} ${<}
