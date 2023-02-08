all: play

play: play.o map.o
	gcc play.o map.o -o play

play.o: play.c map.h
	gcc -Wall -Werror -g play.c -c

map.o: map.c map.h
	gcc -Wall -Werror -g map.c -c

clean:
	rm -f play.o map.o play *~ core
