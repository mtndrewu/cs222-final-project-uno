#ifndef DECK_H
#define DECK_H

#include "Card.h"

void initDeck(Card* deck);

void shuffle(Card* deck, int deckSize);

#endif