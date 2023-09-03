SOURCES_DIRECTORY         = sources
BINARIES_DIRECTORY        = binaries
BINARY_OBJECTS_DIRECTORY  = ${BINARIES_DIRECTORY}/objects
LIBRARIES_DIRECTORY       = libraries
LIBRARY_HEADERS_DIRECTORY = ${LIBRARIES_DIRECTORY}/headers
LIBRARY_SOURCES_DIRECTORY = ${LIBRARIES_DIRECTORY}/sources
COMPILER                  = cc
COMPILER_OPTIONS          = -Oz -Wall -I ${LIBRARY_HEADERS_DIRECTORY}

all: binaries

libraries: ${BINARY_OBJECTS_DIRECTORY}/errors.o                                \
           ${BINARY_OBJECTS_DIRECTORY}/heap-memory.o

binaries: ${BINARIES_DIRECTORY}/left-prompt                                    \
          ${BINARIES_DIRECTORY}/right-prompt

clean:
	rm -rf ${BINARIES_DIRECTORY}

${BINARY_OBJECTS_DIRECTORY}/%.o: ${LIBRARY_SOURCES_DIRECTORY}/%.c              \
                                 ${LIBRARY_HEADERS_DIRECTORY}/%.h
	mkdir -p ${BINARY_OBJECTS_DIRECTORY}
	${COMPILER} ${COMPILER_OPTIONS} -c -o ${@} ${<}

${BINARIES_DIRECTORY}/%: ${SOURCES_DIRECTORY}/%.c                              \
                         libraries
	${COMPILER} ${COMPILER_OPTIONS} -o ${@} ${<}                           \
	                                        ${BINARY_OBJECTS_DIRECTORY}/*
