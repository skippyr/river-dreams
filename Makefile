include config.mk

all: build/river-dreams

clean:
	rm -rf build

install: all
	mkdir -p ${BINPATH}
	cp build/river-dreams ${BINPATH}

build/river-dreams: src/river-dreams.c
	mkdir -p build
	${CC} ${CFLAGS} -o${@} ${^}

uninstall:
	rm -f ${BINPATH}/river-dreams

.PHONY: all clean install uninstall
