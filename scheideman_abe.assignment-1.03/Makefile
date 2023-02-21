all: play

play: main.o map.o path.o heap.o
	gcc main.o map.o path.o heap.o -o play

main.o: main.c world.h
	gcc -Wall -Werror -g main.c -c

map.o: map.c map.h
	gcc -Wall -Werror -g map.c -c

path.o: path.c path.h
	gcc -Wall -Werror -g path.c -c

heap.o: heap.c heap.h
	gcc -Wall -Werror -g heap.c -c

clean:
	rm -f play main.o map.o path.o heap.o *~ core
