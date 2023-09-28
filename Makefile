SOURCES_DIRECTORY   = Sources
PROMPTS_DIRECTORY   = ${SOURCES_DIRECTORY}/Prompts
LIBRARIES_DIRECTORY = ${SOURCES_DIRECTORY}/Libraries
BUILD_DIRECTORY     = Build
OBJECTS_DIRECTORY   = ${BUILD_DIRECTORY}/Objects
BINARIES_DIRECTORY  = ${BUILD_DIRECTORY}/Binaries
COMPILER            = g++
COMPILER_OPTIONS    = -std=c++11 -O3 -Wall -Wextra -Werror -I${LIBRARIES_DIRECTORY}

all: ${BINARIES_DIRECTORY}/LeftPrompt                                                              \
     ${BINARIES_DIRECTORY}/RightPrompt

clean:
	rm -rf ${BUILD_DIRECTORY}

${OBJECTS_DIRECTORY}/%.o: ${LIBRARIES_DIRECTORY}/%.cpp                                             \
                          ${LIBRARIES_DIRECTORY}/%.hpp
	mkdir -p ${OBJECTS_DIRECTORY}
	${COMPILER} ${COMPILER_OPTIONS} -c -o ${@} ${<}

${BINARIES_DIRECTORY}/%: ${PROMPTS_DIRECTORY}/%.cpp                                                \
                         ${OBJECTS_DIRECTORY}/Console.o                                            \
                         ${OBJECTS_DIRECTORY}/Directory.o                                          \
                         ${OBJECTS_DIRECTORY}/EnvironmentVariables.o                               \
                         ${OBJECTS_DIRECTORY}/File.o                                               \
                         ${OBJECTS_DIRECTORY}/GitRepository.o                                      \
                         ${OBJECTS_DIRECTORY}/Network.o                                            \
                         ${OBJECTS_DIRECTORY}/Path.o                                               \
                         ${OBJECTS_DIRECTORY}/Shell.o                                              \
                         ${OBJECTS_DIRECTORY}/StorageDevice.o                                      \
                         ${OBJECTS_DIRECTORY}/SystemTime.o                                         \
                         ${OBJECTS_DIRECTORY}/User.o
	mkdir -p ${BINARIES_DIRECTORY}
	${COMPILER} ${COMPILER_OPTIONS} -o ${@} ${<} ${OBJECTS_DIRECTORY}/*
