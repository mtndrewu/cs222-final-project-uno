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

void userTurn(Card *userHand, Card *deck, Card *discardPile, int *userHandSize, int *userHandCapacity, int *deckSize, int *discardPileSize) {
    printHand(userHand, *userHandSize);

    char userColorChoice;
    char userValueChoice;
    printf("Select which card to draw from ([R/B/G/Y/W] + value shown on screen) (ex. R 1): ");
    scanf("%c %c", &userColorChoice, &userValueChoice);

    while ((userColorChoice == 'W' && (userValueChoice != 'D' && userValueChoice != 'W')) || (userColorChoice != 'W' && userValueChoice == 'W')) {
        printHelp();
        printHand(userHand, *userHandSize);
        printf("Select which card to draw from ([R/B/G/Y/W] + value shown on screen) (ex. R 1): ");
        scanf("%c %c", &userColorChoice, &userValueChoice);
    }

    while (userColorChoice == '+') {
        addCardToHand(userHand, deck, userHandSize, userHandCapacity, deckSize);
        printHand(userHand, *userHandSize);
        printf("Select which card to draw from ([R/B/G/Y/W] + value shown on screen) (ex. R 1): ");
        getchar();
        scanf("%c %c", &userColorChoice, &userValueChoice);
    }

    int cardIdx = findCard(userHand, *userHandSize, userColorChoice, userValueChoice);
    while (cardIdx == *userHandSize) {
        printf("Cannot find card in your hand.\n");
        printf("Select which card to draw from ([R/B/G/Y/W] + value shown on screen) (ex. R 1): ");
        scanf("%c %c", &userColorChoice, &userValueChoice);
        int cardIdx = findCard(userHand, *userHandSize, userColorChoice, userValueChoice);
    }

    
    printf("Color is %c, value is %c\n", userColorChoice, userValueChoice);
    removeCardFromHand(userHand, discardPile, userHandSize, discardPileSize, cardIdx);

    if (discardPile[(*discardPileSize) - 1].color == 'W') {
        char pickColor;
        printf("Please pick the color you want to assign this wild card as (R/G/B/Y): ");
        scanf("%c", &pickColor);
        discardPile[(*discardPileSize) - 1].color = pickColor;
        getchar();
    }
    getchar(); // Removes new line character
}

void cpuTurn(Card *cpuHand, Card *deck, Card *discardPile, int *cpuHandSize, int *cpuHandCapacity, int *deckSize, int *discardPileSize) {
    printHand(cpuHand, *cpuHandSize);
    printCard(discardPile[(*discardPileSize) - 1]);
    Card *candidates;
    int candSize = 0;
    candidates = (Card *) malloc(candSize * sizeof(Card));
    for (int i = 0; i < *cpuHandSize; i++) {
        if (cpuHand[i].color == discardPile[(*discardPileSize) - 1].color || cpuHand[i].color == 'W' || cpuHand[i].value == discardPile[(*discardPileSize) - 1].value) {
            candSize++;
            candidates = (Card *) realloc(candidates, candSize * sizeof(Card));
            candidates[candSize - 1] = cpuHand[i];
        }   
    }

    while (candSize == 0) {
        addCardToHand(cpuHand, deck, cpuHandSize, cpuHandCapacity, deckSize);
        printHand(cpuHand, *cpuHandSize);
        printf("Last card in hand is %c %c\n", cpuHand[(*cpuHandSize) - 1].color, cpuHand[(*cpuHandSize) - 1].value);
        if (cpuHand[(*cpuHandSize) - 1].color == discardPile[(*discardPileSize) - 1].color || cpuHand[(*cpuHandSize) - 1].color == 'W' || cpuHand[(*cpuHandSize) - 1].value == discardPile[(*discardPileSize) - 1].value) {
            candSize++;
            printf("New candSize: %d\n", candSize);
            candidates = (Card *) realloc(candidates, candSize * sizeof(Card));
            printf("Candidates reallocated.\n");
            candidates[candSize - 1] = cpuHand[(*cpuHandSize) - 1];
        }
    }

    for (int i = 0; i < candSize; i++) {
        printf("Color: %c Size: %c\n", candidates[i].color, candidates[i].value);
    }

    int candIdx = rand() % candSize;
    int cardIdx = findCard(cpuHand, *cpuHandSize, candidates[candIdx].color, candidates[candIdx].value);
    removeCardFromHand(cpuHand, discardPile, cpuHandSize, discardPileSize, cardIdx);
    
    printf("CPU played ");
    printCard(discardPile[(*discardPileSize) - 1]);
    printf("\n");
    if (discardPile[(*discardPileSize) - 1].color == 'W') {
        int randomColor = rand() % 4;
        switch (randomColor) {
            case 0:
                discardPile[(*discardPileSize) - 1].color = 'R';
                break;
            case 1:
                discardPile[(*discardPileSize) - 1].color = 'B';
                break;
            case 2:
                discardPile[(*discardPileSize) - 1].color = 'G';
                break;
            case 3:
                discardPile[(*discardPileSize) - 1].color = 'Y';
                break;
        }
        printf("Color of wildcard is now %c\n", discardPile[(*discardPileSize) - 1].color);
        printCard(discardPile[(*discardPileSize) - 1]);
    }
    printf("\n\n");
}

void createHands(Card *userHand, Card *cpuHand, Card *deck, int* deckSize) {
    for (int i = 0; i < 14; i++) {
        if (i % 2 == 0) {
            userHand[i/2] = deck[*deckSize - 1];
        }
        else {
            cpuHand[i/2] = deck[*deckSize - 1];
        }
        (*deckSize) -= 1;
    }
}

void addCardToHand(Card *hand, Card *deck, int* handSize, int *handCapacity, int* deckSize) {
    Card newCard = deck[(*deckSize) - 1];

    (*deckSize)--;
    Card *tmp = realloc(deck, *deckSize * sizeof(Card));
    deck = tmp;

    if (*handSize == *handCapacity) {
        *handCapacity *= 2;
        Card *tmp = realloc(hand, *handCapacity * sizeof(Card));
        hand = tmp;
    }

    printf("%c %c\n", newCard.color, newCard.value);
    hand[(*handSize)++] = newCard;
    printf("%c %c\n", hand[*handSize].color, hand[*handSize].value);
}

int findCard(Card *hand, int handSize, char colorChoice, char valueChoice) {
    int i = 0;
    for (i = 0; i < handSize; i++) {
        if (hand[i].color == colorChoice && hand[i].value == valueChoice) {
            return i;
        }
    }
    i++;
    return i;
}

void removeCardFromHand(Card *hand, Card *discardPile, int* handSize, int* discardPileSize, int idx) {
    (*discardPileSize)++;
    
    if (discardPile == NULL) {
        discardPile = (Card *) malloc(*discardPileSize * sizeof(Card));
    }

    else {
        discardPile = (Card *) realloc(discardPile, *discardPileSize * sizeof(Card));
    }

    if (discardPile == NULL) {
        printf(RED "Could not allocate memory. Program terminated.\n" RESET);
        exit(0);
    }

    discardPile[(*discardPileSize) - 1] = hand[idx];
    for (int i = idx + 1; i < *handSize; i++) {
        hand[i - 1] = hand[i];
    }
    (*handSize)--;
    hand = (Card *) realloc(hand, *handSize * sizeof(Card));
    
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