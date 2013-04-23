MODULE = sokoban
ADDR = 129.241.102.197:/root

default:
	rm -rf *.o
	avr32-linux-gcc -c sokoban_move.c -o sokoban_move.o
	avr32-linux-gcc -c sokoban_level.c -o sokoban_level.o
	avr32-linux-gcc -c sokoban_graphics.c -o sokoban_graphics.o
	avr32-linux-gcc -c sokoban_core.c -o sokoban_core.o
	avr32-linux-gcc -c ../framebuffer/screen.c -o screen.o
	avr32-linux-gcc -c ../leds/leds_control.c -o leds.o
	avr32-linux-gcc -c ../buttons/buttons_control.c -o buttons.o
	avr32-linux-gcc -c ../sound/sound.c -o sound.o
	avr32-linux-gcc sokoban_move.o sokoban_graphics.o sokoban_level.o sokoban_core.o screen.o leds.o buttons.o sound.o -o sokoban

.PHONY : send
send:
	$(shell scp ${MODULE} ../framebuffer/*.bmp ../sound/*.wav root@${ADDR})

.PHONY : clean
clean:
	rm -rf *.o
