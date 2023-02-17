all: play

play: play.o map.o path.o heap.o
	gcc play.o map.o path.o heap.o -o play

play.o: play.c world.h
	gcc -Wall -Werror -g play.c -c

map.o: map.c map.h
	gcc -Wall -Werror -g map.c -c

path.o: path.c path.h
	gcc -Wall -Werror -g path.c -c

heap.o: heap.c heap.h
	gcc -Wall -Werror -g heap.c -c

clean:
	rm -f play play.o map.o path.o heap.o *~ core
