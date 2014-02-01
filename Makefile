# A makefile to build the project
# Author: Alex Orlenko

### Setting up the environment ###

INCLUDES := $(shell kernel/incdirs.pl)
WARNINGS = -Wall -Wextra

CC = i686-elf-gcc
CFLAGS := -fno-builtin -std=c99 -g $(INCLUDES) $(WARNINGS)

AS = nasm
ASFLAGS = -felf

LD = i686-elf-ld
LDFLAGS = -Tkernel/link.ld

OBJCOPY = i686-elf-objcopy
STRIP = i686-elf-strip

CP = cp

CFILES := $(shell find kernel -name '*.c')
ASMFILES := $(shell find kernel -name '*.s')
COBJECTS := $(patsubst %.c,%.o,$(CFILES))
ASMOBJECTS := $(patsubst %.s,%.o,$(ASMFILES))
OBJECTS := kernel/x86/boot.o $(patsubst kernel/x86/boot.o,,$(COBJECTS) $(ASMOBJECTS))
DEPENDENCIES := $(patsubst %.c,%.d,$(CFILES))

##

INITRD_FILES := $(shell find initrd/files)

### .PHONY targets ###

.PHONY : all run debug clean clean-all clean-initrd clean-kernel rebuild

all: alpha.iso

run: all
	qemu -cdrom alpha.iso
	
debug: all
	qemu -cdrom alpha.iso -s -S &
	gdb

clean: clean-initrd clean-kernel

clean-all: clean
	-$(RM) isofiles/kernel isofiles/initrd
	-$(RM) alpha.iso

clean-initrd:
	-$(RM) initrd/initrd initrd/initrd.asm

clean-kernel:
	-$(RM) $(OBJECTS) $(DEPENDENCIES) kernel/kernel kernel/kernel.sym

rebuild: clean all

### ISO image ###

alpha.iso: isofiles/initrd isofiles/kernel
	@xorrisofs -R -J -r -quiet -iso-level 3 -hide boot.catalog \
		-b boot/grub/stage2_eltorito -no-emul-boot \
		-boot-load-size 4 -boot-info-table \
		-A "Aquarius Alpha by Alex Orlenko" \
		-V "Aquaruis Alpha by Alex Orlenko" \
		-o alpha.iso isofiles
	@echo OK.

isofiles/initrd: initrd/initrd
	@$(CP) $< $@
	
isofiles/kernel: kernel/kernel
	@$(CP) $< $@

### Kernel ###

-include $(DEPENDENCIES)

kernel/kernel: $(OBJECTS)
	@$(LD) $(LDFLAGS) $(OBJECTS) -o $@
	@$(OBJCOPY) --only-keep-debug $@ $@.sym
	@$(STRIP) $@
	@echo 'The kernel has been built.'

.c.o:
	@$(CC) $(CFLAGS) -MD -MP -o $@ -c $<

.s.o:
	@$(AS) $(ASFLAGS) $<

### Initial ramdisk ###

initrd/initrd: $(INITRD_FILES)
	@cd initrd && ./mkrd
	@echo 'Initrd is done.'

### The end. ###
