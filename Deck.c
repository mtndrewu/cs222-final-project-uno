#include "Card.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void initDeck(Card* deck) {
    char colors[] = {'R', 'B', 'G', 'Y'};
    char values[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'R', 'S', 'D'};
    int deckIdx = 0;


    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < sizeof(colors); j++) {
            for (int k = 0; k < sizeof(values); k++) {
                if (i == 1 && values[k] == '0') {
                    continue;
                }
                deck[deckIdx++] = (Card){colors[j], values[k]};
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        deck[deckIdx++] = (Card){'W', 'W'};
        deck[deckIdx++] = (Card){'W', 'W'};
    }
}

void shuffle(Card* deck, int deckSize) {
    srand(time(NULL));
    for (int i = deckSize - 1; i > 0; i--) {
        int j = rand() % (i+1);
        Card tmp = deck[i];
        deck[i] = deck[j];
        deck[j] = tmp;
    }
}