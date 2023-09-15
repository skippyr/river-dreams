SOURCES_DIRECTORY = sources
BUILD_DIRECTORY   = build
COMPILER          = g++
COMPILER_OPTIONS  = -std=c++11 -O3 -Wall -Wextra -Werror

all: ${BUILD_DIRECTORY}/left-prompt                                            \
     ${BUILD_DIRECTORY}/right-prompt

clean:
	rm -rf ${BUILD_DIRECTORY}

${BUILD_DIRECTORY}/%: ${SOURCES_DIRECTORY}/%.cpp
	mkdir -p ${BUILD_DIRECTORY}
	${COMPILER} ${COMPILER_OPTIONS} -o ${@} ${<}
