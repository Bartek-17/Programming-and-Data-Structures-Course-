all: Hanoi

Hanoi: Hanoi.o primlib.o
	gcc -fsanitize=undefined -g $^ -o $@  -lSDL2_gfx `sdl2-config --libs` -lm

.c.o: 
	gcc  -fsanitize=undefined -g -Wall -pedantic `sdl2-config --cflags` -c  $<

primlib.o: primlib.c primlib.h

Hanoi.o: Hanoi.c primlib.h

clean:
	-rm primlib.o Hanoi.o Hanoi
