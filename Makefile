CC = gcc
CFLAGS = $(shell pkg-config --cflags gtk+-3.0 vte-2.91)
LIBS = $(shell pkg-config --libs gtk+-3.0 vte-2.91)
PREFIX = /usr/local

all: valmieritty

valmieritty: src/valmieritty.c
	$(CC) src/valmieritty.c -o valmieritty $(CFLAGS) $(LIBS)

install: valmieritty
	install -D valmieritty $(DESTDIR)$(PREFIX)/bin/valmieritty
	install -D valmieritty.desktop $(DESTDIR)$(PREFIX)/share/applications/valmieritty.desktop

clean:
	rm -f valmieritty
