include config.mk

.PHONY: all clean install uninstall

all: build/river-dreams

clean:
	rm -rf build

install: all
	mkdir -p ${INSTALLATION_BIN_PATH}
	cp build/river-dreams ${INSTALLATION_BIN_PATH}

uninstall:
	rm -f ${INSTALLATION_BIN_PATH}/river-dreams

build/river-dreams: src/river-dreams.c
	mkdir -p build
	${COMPILER} ${COMPILER_FLAGS} -o${@} ${^}
