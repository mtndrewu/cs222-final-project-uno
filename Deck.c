#include "Card.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void initDeck(Card* deck) {
    char colors[5] = {'R', 'B', 'G', 'Y', 'W'};
    char values[14] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'R', 'S', 'D', 'W'};
    int deckIdx = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < sizeof(colors); j++) {
            for (int k = 0; k < sizeof(values); k++) {
                if ((colors[j] == 'W' && (values[k] != 'D' && values[k] != 'W')) || (colors[j] != 'W' && values[k] == 'W')) {
                    continue;
                }
                Card card;
                card.color = colors[j];
                card.value = values[k];
                deck[deckIdx] = card;
                deckIdx++;
            }
        }
    }
}

void shuffle(Card* deck, int deckSize) {
    srand(time(NULL));
    for (int i = 0; i < 100000; i++) {
        int idx1 = rand() % deckSize;
        int idx2 = rand() % deckSize;
        Card tmp = deck[idx1];
        deck[idx1] = deck[idx2];
        deck[idx2] = tmp;
    }
}