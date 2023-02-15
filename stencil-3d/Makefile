SRCDIR=src/
BINDIR=bin/
CC=aarch64-linux-gnu-gcc-10
#CC=gcc
CFLAGS := -O3 -static -march=armv8-a
#CFLAGS := -O3 -static

DEPS=Makefile

all: clean makedir stencil-3d psize

stencil-3d: $(SRCDIR)stencil-3d.c $(DEPS)
	$(CC) $< $(CFLAGS) -o $(BINDIR)stencil-3d

# different problem sizes for point2
psize: stencil5 stencil10 stencil20 stencil40

stencil5: $(SRCDIR)stencil-3d.c $(DEPS)
	$(CC) $< $(CFLAGS) -DPSIZE=5 -o $(addprefix $(BINDIR), $@)

stencil10: $(SRCDIR)stencil-3d.c $(DEPS)
	$(CC) $< $(CFLAGS) -DPSIZE=10 -o $(addprefix $(BINDIR), $@)

stencil20: $(SRCDIR)stencil-3d.c $(DEPS)
	$(CC) $< $(CFLAGS) -DPSIZE=20 -o $(addprefix $(BINDIR), $@)

stencil40: $(SRCDIR)stencil-3d.c $(DEPS)
	$(CC) $< $(CFLAGS) -DPSIZE=40 -o $(addprefix $(BINDIR), $@)

.PHONY: makedir clean default psize

makedir:
	mkdir -p $(SRCDIR)
	mkdir -p $(BINDIR)

clean:
	rm -rf $(BINDIR)

default: all


