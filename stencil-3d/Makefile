SRCDIR=src/
BINDIR=bin/
CC=aarch64-linux-gnu-gcc-10
CFLAGS := -O3 -static -march=armv8-a

DEPS=Makefile

all: clean makedir stencil-3d

stencil-3d: $(SRCDIR)stencil-3d.c $(DEPS)
	$(CC) $< $(CFLAGS) -o $(BINDIR)stencil-3d

.PHONY: makedir clean default

makedir:
	mkdir -p $(SRCDIR)
	mkdir -p $(BINDIR)

clean:
	rm -rf $(BINDIR)

default: all


