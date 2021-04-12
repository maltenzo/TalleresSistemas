CC=gcc

ARCH := $(shell uname -m)

BINS=anillo_alu

OBJS=$(BINS:=.o)

all: $(BINS)

# Dependencias
anillo_alu.o = anillo_alu.c

clean:
	rm -rf $(OBJS) $(BINS)