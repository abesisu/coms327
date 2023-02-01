all: map

map: map.c
	gcc -Wall -Werror -g map.c -o map

clean:
	rm -f map *~ core
