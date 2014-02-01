INCLUDES = -I../include -I../include/aquarius
WARNINGS = -Wall -Wextra
CFLAGS := $(INCLUDES) $(WARNINGS) -fno-builtin -std=c99
CC = i686-elf-gcc

GAS = i686-elf-as

AR = i686-elf-ar
RANLIB = i686-elf-ranlib
ARFLAGS = qc

LIBGLOSS_CFILES := $(shell find libgloss -name '*.c')
LIBGLOSS_COBJECTS := $(patsubst %.c,%.o,$(LIBGLOSS_CFILES))
LIBGLOSS_SFILES := $(shell find libgloss -name '*.S')
LIBGLOSS_SOBJECTS := $(patsubst %.S,%.o,$(LIBGLOSS_SFILES))
LIBGLOSS_OBJECTS := $(LIBGLOSS_COBJECTS) $(LIBGLOSS_SOBJECTS)

###

.PHONY: all clean

all: ../lib/crt0.o ../lib/libgloss.a

clean: crt0-clean libgloss-clean

crt0-clean:
	$(RM) crt0.o

libgloss-clean:
	$(RM) $(LIBGLOSS_OBJECTS)

###

../lib/crt0.o: crt0.o
	cp $< $@

crt0.o: crt0.s
	nasm -felf -o $@ $<

###

../lib/libgloss.a: libgloss/libgloss.a
	cp $< $@

libgloss/libgloss.a: $(LIBGLOSS_OBJECTS)
	$(AR) $(ARFLAGS) $@ $(LIBGLOSS_OBJECTS)
	$(RANLIB) $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.S.o:
	$(GAS) -o $@ $<
