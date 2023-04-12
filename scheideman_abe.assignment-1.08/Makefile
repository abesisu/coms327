all: play

play: heap.o map.o path.o action.o data.o pokemon.o main.o
	g++ heap.o map.o path.o action.o data.o pokemon.o main.o -lncurses -o play

heap.o: heap.c heap.h
	gcc -Wall -Werror -g -c heap.c -o heap.o

map.o: map.cpp map.h
	g++ -Wall -Werror -g -c map.cpp -o map.o

path.o: path.cpp path.h
	g++ -Wall -Werror -g -c path.cpp -o path.o

action.o: action.cpp action.h
	g++ -Wall -Werror -g -c action.cpp -o action.o

data.o: data.cpp data.h
	g++ -Wall -Werror -g -c data.cpp -o data.o

pokemon.o: pokemon.cpp pokemon.h
	g++ -Wall -Werror -g -c pokemon.cpp -o pokemon.o

main.o: main.cpp world.h data.h
	g++ -Wall -Werror -g -c main.cpp -o main.o

clean:
	rm -f play heap.o map.o path.o action.o data.o pokemon.o main.o *~ core
