#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "functions.h"
#include "Deck.h"
#include "Card.h"

// Text colors
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define WHITE "\033[37m"
#define RESET "\033[0m"

void printHelp() {
    printf("A card's color is indicated by the color of the text (wildcard cards have white text), which corresponds to the value of the card.\n");
    printf("List of possible colors and commands to draw card of that color:\n");
    printf("Red -> R\nBlue -> B\nGreen -> G\nYellow -> Y\nWildcard -> W\n\n");
    printf("List of possible values and commands to draw card containing that value:\n");
    printf("0-9 -> 0-9\nReverse -> R\nSkip -> S\nDraw 2 (4 when combined with a white text color)-> D\nWildcard -> W\n\n");
    printf("The correct format for a string input would be 'color value', for example Y 1 to play a yellow card with a value of 1.\n\n");
    printf("In the event you need to draw another card: enter '+ 1'.\n\n");
}

void createHands(Card **userHand, Card **cpuHand, Card *deck, int* deckSize, int* userHandSize, int* cpuHandSize) {
    *userHandSize = *cpuHandSize = 7;
    *userHand = (Card *) malloc(7 * sizeof(Card));
    *cpuHand = (Card *) malloc(7 * sizeof(Card));
    for (int i = 0; i < 7; i++) {
        (*userHand)[i] = deck[--(*deckSize)];
        (*cpuHand)[i] = deck[--(*deckSize)];
    }
}

void addCardToHand(Card **hand, Card **deck, int* handSize, int* deckSize) {
    if (*deckSize == 0) {
        printf(RED "Deck is empty. Cannot draw more cards.\n" RESET);
        return;
    }
    
    Card newCard = (*deck)[(*deckSize) - 1];
    (*deckSize)--;


    Card *tmpDeck = realloc(*deck, ((*deckSize) - 1) * sizeof(Card));
    if (tmpDeck == NULL) {
        printf(RED "Cannot reallocate memory. Program terminated.\n" RESET);
        exit(0);
    }
    *deck = tmpDeck;
    (*deckSize)--;

    
    Card *tmpHand = realloc(*hand, ((*handSize) + 1) * sizeof(Card));
    if (tmpHand == NULL) {
        printf(RED "Cannot reallocate memory. Program terminated.\n" RESET);
        exit(0);
    }
    *hand = tmpHand;

    (*hand)[*handSize] = newCard;
    (*handSize)++;
}

int findCard(Card *hand, int handSize, char colorChoice, char valueChoice) {
    //getchar();
    printf("Desired Color: %c, Desired Value: %c\n", colorChoice, valueChoice); 
    for (int i = 0; i < handSize; i++) {
        printf("Color: %c, Value: %c\n", hand[i].color, hand[i].value); 
        if (hand[i].color == colorChoice && hand[i].value == valueChoice) {
            return i;
        }
    }
    return -1;
}

void removeCardFromHand(Card **hand, Card **discardPile, int* handSize, int* discardPileSize, int idx) {
    (*discardPileSize)++;
    
    if (*discardPile == NULL) {
        *discardPile = (Card *) malloc((*discardPileSize) * sizeof(Card));
    }

    else {
        Card *tmpDiscardPile = (Card *) realloc(*discardPile, (*discardPileSize) * sizeof(Card));
        if (tmpDiscardPile == NULL) {
            printf(RED "Cannot reallocate memory. Program terminated.\n" RESET);
            exit(0);
        }
        *discardPile = tmpDiscardPile;
    }

    (*discardPile)[(*discardPileSize) - 1] = (*hand)[idx];

    for (int i = idx + 1; i < *handSize; i++) {
        (*hand)[i - 1] = (*hand)[i];
    }

    (*handSize)--;

    if (*handSize > 0) {
        Card *tmpHand = realloc(*hand, (*handSize) * sizeof(Card));
        if (tmpHand == NULL) {
            printf(RED "Cannot reallocate memory. Program terminated.\n" RESET);
            exit(0);
        }
        *hand = tmpHand;
    }

    else {
        free(*hand);
        *hand = NULL;
    }
    
}


void printCard(Card card) {
    switch (card.color) {
        case 'R':
            printf(RED "%c " RESET, card.value);
            break;
        case 'B':
            printf(BLUE "%c " RESET, card.value);
            break;
        case 'G':
            printf(GREEN "%c " RESET, card.value);
            break;
        case 'Y':
            printf(YELLOW "%c " RESET, card.value);
            break;
        default:
            printf("%c ", card.value);
            break;
    }
}

void printHand(Card *userHand, int userHandSize) {
    for (int i = 0; i < userHandSize; i++) {
        switch (userHand[i].color) {
            case 'R':
                printf(RED "%c " RESET, userHand[i].value);
                break;
            case 'B':
                printf(BLUE "%c " RESET, userHand[i].value);
                break;
            case 'G':
                printf(GREEN "%c " RESET, userHand[i].value);
                break;
            case 'Y':
                printf(YELLOW "%c " RESET, userHand[i].value);
                break;
            default:
                printf("%c ", userHand[i].value);
                break;
        }
    }
    printf("\n\n");
}