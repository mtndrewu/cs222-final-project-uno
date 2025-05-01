#include "Card.h"
#include "Deck.h"
#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// Text colors
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define WHITE "\033[37m"
#define RESET "\033[0m"


void printTitle() {
    printf(YELLOW "UUUUUUUU     UUUUUUUUNNNNNNNN        NNNNNNNN     OOOOOOOOO     \n");
    printf("U::::::U     U::::::UN:::::::N       N::::::N   OO:::::::::OO   \n");
    printf("U::::::U     U::::::UN::::::::N      N::::::N OO:::::::::::::OO \n");
    printf("UU:::::U     U:::::UUN:::::::::N     N::::::NO:::::::OOO:::::::O\n");
    printf(" U:::::U     U:::::U N::::::::::N    N::::::NO::::::O   O::::::O\n");
    printf(" U:::::D     D:::::U N:::::::::::N   N::::::NO:::::O     O:::::O\n");
    printf(" U:::::D     D:::::U N:::::::N::::N  N::::::NO:::::O     O:::::O\n");
    printf(" U:::::D     D:::::U N::::::N N::::N N::::::NO:::::O     O:::::O\n");
    printf(" U:::::D     D:::::U N::::::N  N::::N:::::::NO:::::O     O:::::O\n");
    printf(" U:::::D     D:::::U N::::::N   N:::::::::::NO:::::O     O:::::O\n");
    printf(" U:::::D     D:::::U N::::::N    N::::::::::NO:::::O     O:::::O\n");
    printf(" U::::::U   U::::::U N::::::N     N:::::::::NO::::::O   O::::::O\n");
    printf(" U:::::::UUU:::::::U N::::::N      N::::::::NO:::::::OOO:::::::O\n");
    printf("  UU:::::::::::::UU  N::::::N       N:::::::N OO:::::::::::::OO \n");
    printf("    UU:::::::::UU    N::::::N        N::::::N   OO:::::::::OO   \n");
    printf("      UUUUUUUUU      NNNNNNNN         NNNNNNN     OOOOOOOOO     \n" RESET);
}

void userTurn(Card *userHand, Card *deck, Card *discardPile, int *userHandSize, int *deckSize, int *discardPileSize) {
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
        addCardToHand(userHand, deck, userHandSize, deckSize);
        printHand(userHand, *userHandSize);
        printf("Select which card to draw from ([R/B/G/Y/W] + value shown on screen) (ex. R 1): ");
        getchar();
        scanf("%c %c", &userColorChoice, &userValueChoice);
    }

    int cardIdx = findCard(userHand, *userHandSize, userColorChoice, userValueChoice);
    printf("Index of card is at %d\n", cardIdx);

    while (cardIdx == *userHandSize) {
        printf("Cannot find card in your hand.\n");
        printf("Select which card to draw from ([R/B/G/Y/W] + value shown on screen) (ex. R 1): ");
        scanf("%c %c", &userColorChoice, &userValueChoice);
        int cardIdx = findCard(userHand, *userHandSize, userColorChoice, userValueChoice);
    }

    removeCardFromHand(userHand, discardPile, userHandSize, discardPileSize, cardIdx);
    printf("Top of discard pile has color %c and value %c.\n", discardPile[(*discardPileSize) - 1].color, discardPile[(*discardPileSize) - 1].value);

    if (discardPile[(*discardPileSize) - 1].color == 'W') {
        char pickColor;
        getchar();
        printf("Please pick the color you want to assign this wild card as (R/G/B/Y): ");
        scanf("%c", &pickColor);
        printf("User picks %c\n", pickColor);
        discardPile[(*discardPileSize) - 1].color = pickColor;
        discardPile[(*discardPileSize) - 1].value = 'W';
    }
    getchar(); // Removes new line character
}

void cpuTurn(Card *cpuHand, Card *deck, Card *discardPile, int *cpuHandSize, int *deckSize, int *discardPileSize) {
    printf("-------------------------------------------------\n");
    printf("CPU's Turn\n");
    printHand(cpuHand, *cpuHandSize);
    Card *candidates;
    int candSize = 0;
    int max_candidates = 10;
    candidates = (Card *) malloc(max_candidates * sizeof(Card));
    for (int i = 0; i < *cpuHandSize; i++) {
        if (cpuHand[i].color == discardPile[(*discardPileSize) - 1].color || cpuHand[i].color == 'W' || cpuHand[i].value == discardPile[(*discardPileSize) - 1].value) {
            candSize++;
            if (candSize == max_candidates)
                candidates = (Card *) realloc(candidates, 2 * candSize * sizeof(Card));
            candidates[candSize - 1] = cpuHand[i];
            printf("Candidate of color %c and value %c added.\n", candidates[candSize - 1].color, candidates[candSize - 1].value);
        }   
    }

    while (candSize == 0) {
        addCardToHand(cpuHand, deck, cpuHandSize, deckSize);

        printf("Top of discard pile has color %c and value %c.\n", discardPile[(*discardPileSize) - 1].color, discardPile[(*discardPileSize) - 1].value);
        printf("Card of color %c and value %c added to hand.\n", cpuHand[(*cpuHandSize) - 1].color, cpuHand[(*cpuHandSize) - 1].value);

        printHand(cpuHand, *cpuHandSize);
        //printf("Last card in hand is %c %c\n", cpuHand[(*cpuHandSize) - 1].color, cpuHand[(*cpuHandSize) - 1].value);
        if (cpuHand[(*cpuHandSize) - 1].color == discardPile[(*discardPileSize) - 1].color || cpuHand[(*cpuHandSize) - 1].color == 'W' || cpuHand[(*cpuHandSize) - 1].value == discardPile[(*discardPileSize) - 1].value) {
            candSize++;
            if (candSize == max_candidates) {
                max_candidates *= 2;
                candidates = (Card *) realloc(candidates, max_candidates * sizeof(Card));
            }
            candidates[candSize - 1] = cpuHand[(*cpuHandSize) - 1];
            printf("Candidate of color %c and value %c added.\n", candidates[candSize - 1].color, candidates[candSize - 1].value);
        }
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

int main(void) {
    char userChoice;
    printTitle();
    printf("By Drew Urenko\n");
    printf("Enter 1 to start the game, enter 2 to read the help manual, or enter 3 to exit: ");
    scanf("%c", &userChoice);
    getchar();
    while (userChoice != '1') {
        if (userChoice == '2') {
            printHelp();
            printf("Enter 1 to start the game, enter 2 to read the help manual, or enter 3 to exit: ");
            scanf("%c", &userChoice);
            getchar();
            printf("%c\n", userChoice);
        }
        if (userChoice == '3') {
            exit(0);
        }
        if (userChoice != '1' && userChoice != '2' && userChoice != '3') {
            printf("Error. Invalid choice.\n");
            printf("Enter 1 to start the game, enter 2 to read the help manual, or enter 3 to exit: ");
            scanf("%c", &userChoice);
            getchar();
        }
    }
    
    const int numCards = 108;
    Card* deck;
    int deckSize = numCards;
    Card* discardPile;
    int discardPileSize = 0;

    deck = (Card*) malloc(numCards * sizeof(Card));
    if (deck == NULL) {
        printf(RED "Could not allocate memory. Program terminated.\n" RESET);
        exit(0);
    }

    initDeck(deck);
    shuffle(deck, deckSize);

    int userHandSize = 7;
    Card *userHand = (Card *) malloc(numCards / 2 * sizeof(Card));

    if (userHand == NULL) {
        printf(RED "Could not allocate memory. Program terminated.\n" RESET);
        exit(0);
    }

    int cpuHandSize = userHandSize;
    Card *tmp = (Card *) malloc (numCards/2 * sizeof(Card));
    if (tmp == NULL) {
        printf(RED "Could not allocate memory. Program terminated.\n" RESET);
        exit(0);
    }
    Card *cpuHand = tmp;

    discardPile = (Card*) malloc(numCards * sizeof(Card));
    

    createHands(userHand, cpuHand, deck, &deckSize);
    deck = realloc(deck, deckSize * sizeof(Card));
    if (deck == NULL) {
        printf(RED "Could not reallocate memory. Program terminated.\n" RESET);
        exit(0);
    }

    while (userHandSize != 0 && cpuHandSize != 0) {
        // User Turn
        userTurn(userHand, deck, discardPile, &userHandSize, &deckSize, &discardPileSize);
        if (userHandSize == 1) {
            printf("User says UNO!\n");
        }
        cpuTurn(cpuHand, deck, discardPile, &cpuHandSize, &deckSize, &discardPileSize);
        if (cpuHandSize == 1) {
            printf("CPU says UNO!\n");
        }
        
    }

    if (userHandSize == 0) {
        printf("User wins!");
    }

    else {
        printf("CPU wins!\n");
    }

    return 0;
}