.PHONY: all clean run

all: ../initrd/files/system ../initrd/files/hello ../initrd/files/autostart ../initrd/files/kbd ../initrd/files/ls ../initrd/files/game ../initrd/files/dock
	echo 'Programs are up to date.'
	(cd .. && make)

clean:
	rm -f *.o system hello

run: all
	(cd .. && make run)

../initrd/files/hello: hello
	cp $< $@

../initrd/files/autostart: autostart
	cp $< $@

../initrd/files/ls: ls
	cp $< $@

../initrd/files/game: game
	cp $< $@

../initrd/files/dock: dock
	cp $< $@

LS_O = ls.o aquarius.o str.o
ls: $(LS_O)
	i686-elf-ld -Tsdk/link.ld -o $@ $(LS_O)

AUTOSTART_OBJ = autostart.o aquarius.o kbd.o str.o
autostart: $(AUTOSTART_OBJ)
	i686-elf-ld -Tsdk/link.ld -o $@ $(AUTOSTART_OBJ)

GAME_OBJ = game.o aquarius.o kbd.o os.o
game: $(GAME_OBJ)
	i686-elf-ld -Tsdk/link.ld -o $@ $(GAME_OBJ)

hello: hello.o
	i686-elf-ld -Tsdk/link.ld -o $@ hello.o sdk/lib/ccaprice.a

dock: dock.o
	i686-elf-ld -Tsdk/link.ld -o $@ dock.o sdk/lib/ccaprice.a

.c.o:
	i686-elf-gcc -Isdk/include -Isdk/include/aquarius -c -o $@ $<

../initrd/files/system: system
	cp $< $@

system: system.asm
	nasm -fbin $<
