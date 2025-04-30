uno: Card.o Deck.o functions.o uno.o
	gcc Card.o Deck.o functions.o uno.o -o uno

Card.o: Card.c
	gcc -c Card.c

Deck.o: Deck.c
	gcc -c Deck.c

functions.o: functions.c
	gcc -c functions.c

uno.o: uno.c
	gcc -c uno.c

clean:
	rm -rf *.o uno