include config.mk

all: river-dreams

clean:
	rm -f river-dreams

install: all
	mkdir -p $(BINPATH)
	mv river-dreams $(BINPATH)

river-dreams: river-dreams.c
	$(CC) $(CFLAGS) -o$(@) $(^)

uninstall:
	rm -f $(BINPATH)/river-dreams

.PHONY: all clean install uninstall
