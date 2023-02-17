SRCDIR=src/
BINDIR=bin/
ASMDIR=asm/
CC=aarch64-none-linux-gnu-gcc
#CC=gcc
CFLAGS := -O3 -static -march=armv8.2-a
CFLAGSV := -static -march=armv8.2-a+sve
#CFLAGS := -O3 -static

DEPS=Makefile

all: clean makedir stencil-3d psize autov manuv optis

stencil-3d: $(SRCDIR)stencil-3d.c $(DEPS)
	$(CC) $< $(CFLAGS) -o $(BINDIR)$@
	$(CC) $< $(CFLAGS) -S -o $(ASMDIR)$@

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

# auto-vectorization
autov: stencil-3daO1 stencil-3daO2 stencil-3daO3 stencil-3daOf

stencil-3daO1: $(SRCDIR)stencil-3d.c $(DEPS)
	$(CC) $< $(CFLAGSV) -O1 -o $(ASMDIR)$@ -S
	$(CC) $< $(CFLAGSV) -O1 -o $(BINDIR)$@

stencil-3daO2: $(SRCDIR)stencil-3d.c $(DEPS)
	$(CC) $< $(CFLAGSV) -O2 -o $(ASMDIR)$@ -S
	$(CC) $< $(CFLAGSV) -O2 -o $(BINDIR)$@

stencil-3daO3: $(SRCDIR)stencil-3d.c $(DEPS)
	$(CC) $< $(CFLAGSV) -O3 -o $(ASMDIR)$@ -S
	$(CC) $< $(CFLAGSV) -O3 -o $(BINDIR)$@

stencil-3daOf: $(SRCDIR)stencil-3d.c $(DEPS)
	$(CC) $< $(CFLAGSV) -Ofast -o $(ASMDIR)$@ -S
	$(CC) $< $(CFLAGSV) -Ofast -o $(BINDIR)$@

# manual-vectorization
manuv: stencil-3d-sve

stencil-3d-sve: $(SRCDIR)stencil-3d-sve.c $(DEPS)
	$(CC) $< $(CFLAGSV) -O3 -o $(ASMDIR)$@ -S
	$(CC) $< $(CFLAGSV) -O3 -o $(BINDIR)$@

# other optimizations
optis: stencil-3dflat

stencil-3dflat: $(SRCDIR)stencil-3dflat.c $(DEPS)
	$(CC) $< $(CFLAGSV) -O3 -o $(ASMDIR)$@ -S
	$(CC) $< $(CFLAGSV) -O3 -o $(BINDIR)$@

.PHONY: makedir clean default psize autov manuv optis

makedir:
	mkdir -p $(SRCDIR)
	mkdir -p $(BINDIR)
	mkdir -p $(ASMDIR)

clean:
	rm -rf $(BINDIR)
	rm -rf $(ASMDIR)

default: all


