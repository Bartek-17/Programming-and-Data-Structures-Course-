all: Tetris

Tetris: Tetris.o primlib.o
	gcc -fsanitize=undefined -g $^ -o $@  -lSDL2_gfx `sdl2-config --libs` -lm

.c.o: 
	gcc  -fsanitize=undefined -g -Wall -pedantic `sdl2-config --cflags` -c  $<

primlib.o: primlib.c primlib.h

Tetris.o: Tetris.c primlib.h pieces.h

clean:
	-rm primlib.o Tetris.o Tetris
