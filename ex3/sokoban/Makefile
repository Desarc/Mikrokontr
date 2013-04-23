MODULE = sokoban
ADDR = 129.241.102.197:/root

default:
	rm -rf *.o
	gcc -c sokoban_move.c -o sokoban_move.o
	gcc -c sokoban_level.c -o sokoban_level.o
	gcc -c sokoban_graphics.c -o sokoban_graphics.o
	gcc -c sokoban_core.c -o sokoban_core.o
	gcc -c ../framebuffer/screen.c -o screen.o
	gcc -c ../leds/leds_control.c -o leds.o
	gcc -c ../buttons/buttons_control.c -o buttons.o
	gcc -c ../sound/sound.c -o sound.o
	gcc sokoban_move.o sokoban_graphics.o sokoban_level.o sokoban_core.o screen.o leds.o buttons.o sound.o -o sokoban

.PHONY : send
send:
	$(shell scp ${MODULE} ../framebuffer/*.bmp ../sound/*.wav root@${ADDR})

.PHONY : clean
clean:
	rm -rf *.o
