#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Card.h"
#include "Deck.h"

void printHelp();
void userTurn(Card *userHand, Card *deck, Card *discardPile, int *userHandSize, int *deckSize, int *discardPileSize);
void cpuTurn(Card *cpuHand, Card *deck, Card *discardPile, int *cpuHandSize, int *deckSize, int *discardPileSize);
void createHands(Card *userHand, Card *cpuHand, Card *deck, int *deckSize);
void addCardToHand(Card *hand, Card *deck, int* handSize, int* deckSize);
void removeCardFromHand(Card *hand, Card *discardPile, int* handSize, int* discardPileSize, int idx);
int findCard(Card *hand, int handSize, char colorChoice, char valueChoice);
void printCard(Card card);
void printHand(Card *userHand, int userHandSize);

#endif