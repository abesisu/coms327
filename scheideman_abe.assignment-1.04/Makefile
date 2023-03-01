all: play

play: main.o map.o path.o heap.o trainer.o
	gcc main.o map.o path.o heap.o trainer.o -o play

main.o: main.c world.h
	gcc -Wall -Werror -g main.c -c

map.o: map.c map.h
	gcc -Wall -Werror -g map.c -c

path.o: path.c path.h
	gcc -Wall -Werror -g path.c -c

heap.o: heap.c heap.h
	gcc -Wall -Werror -g heap.c -c

trainer.o: trainer.c trainer.h
	gcc -Wall -Werror -g trainer.c -c

clean:
	rm -f play main.o map.o path.o heap.o trainer.o *~ core
