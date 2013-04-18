default:
	gcc -c sokoban_move.c -o sokoban_move.o
	gcc -c sokoban_level.c -o sokoban_level.o
	gcc -c sokoban_graphics.c -o sokoban_graphics.o
	gcc -c sokoban_core.c -o sokoban_core.o
	gcc sokoban_move.o sokoban_graphics.o sokoban_level.o sokoban_core.o -o sokoban

.PHONY : clean
clean:
	rm -rf *.o
