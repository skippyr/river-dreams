SOURCES_DIRECTORY         = sources
LIBRARIES_DIRECTORY       = ${SOURCES_DIRECTORY}/libraries
LIBRARY_HEADERS_DIRECTORY = ${LIBRARIES_DIRECTORY}/headers
LIBRARY_SOURCES_DIRECTORY = ${LIBRARIES_DIRECTORY}/sources
OUT_DIRECTORY             = out
BUILD_DIRECTORY           = ${OUT_DIRECTORY}/build
RELEASES_DIRECTORY        = ${OUT_DIRECTORY}/releases
COMPILER                  = g++
COMPILER_OPTIONS          = -std=c++11 -O3 -Wall -Wextra -Werror -I${LIBRARY_HEADERS_DIRECTORY}

all: releases

clean:
	rm -rf ${OUT_DIRECTORY}

objects: ${BUILD_DIRECTORY}/Terminal.o                                                                                 \
         ${BUILD_DIRECTORY}/Network.o                                                                                  \
         ${BUILD_DIRECTORY}/SystemTime.o                                                                               \
         ${BUILD_DIRECTORY}/User.o                                                                                     \
         ${BUILD_DIRECTORY}/StorageDevice.o                                                                            \
         ${BUILD_DIRECTORY}/DirectoryEntriesStatus.o                                                                   \
         ${BUILD_DIRECTORY}/GitRepository.o                                                                            \
         ${BUILD_DIRECTORY}/Path.o                                                                                     \
         ${BUILD_DIRECTORY}/Shell.o                                                                                    \
         ${BUILD_DIRECTORY}/EnvironmentVariables.o

releases: ${RELEASES_DIRECTORY}/LeftPrompt                                                                             \
          ${RELEASES_DIRECTORY}/RightPrompt

${BUILD_DIRECTORY}/%.o: ${LIBRARY_SOURCES_DIRECTORY}/%.cpp                                                             \
                          ${LIBRARY_HEADERS_DIRECTORY}/%.hpp
	mkdir -p ${BUILD_DIRECTORY}
	${COMPILER} ${COMPILER_OPTIONS} -c -o ${@} ${<}

${RELEASES_DIRECTORY}/%: ${SOURCES_DIRECTORY}/%.cpp                                                                    \
                         ${LIBRARY_SOURCES_DIRECTORY}/*                                                                \
                         objects
	mkdir -p ${RELEASES_DIRECTORY}
	${COMPILER} ${COMPILER_OPTIONS} -o ${@} ${<} ${BUILD_DIRECTORY}/*
