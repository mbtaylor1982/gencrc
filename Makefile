#
# Makefile to build gencrc
#
PROGS   := gencrc

VER ?= 0.1+

CC      := gcc
CFLAGS  := -O2

ifeq (, $(shell which $(CC) 2>/dev/null ))
$(error "No $(CC) in PATH: maybe do PATH=$$PATH:/opt/amiga/bin to set up")
endif

all: $(PROGS)

gencrc: gencrc.c crc32.c

$(PROGS): Makefile

gencrc:
	$(CC) $(CFLAGS) $(filter %.c,$^) -o $@

clean:
	rm $(PROGS)
