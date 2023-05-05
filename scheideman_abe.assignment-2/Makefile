all: solitare

CC = g++
CFLAGS = -Wall -Werror -g 

solitare: card.o deck.o setup.o main.o
	$(CC) $(CFLAGS) card.o deck.o setup.o main.o -lncurses -o solitare

card.o: card.cpp card.h
	$(CC) $(CFLAGS) -c card.cpp

deck.o: deck.cpp deck.h
	$(CC) $(CFLAGS) -c deck.cpp

setup.o: setup.cpp setup.h
	$(CC) $(CFLAGS) -c setup.cpp

main.o: main.cpp setup.h
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm -f solitare *.o *~ core
