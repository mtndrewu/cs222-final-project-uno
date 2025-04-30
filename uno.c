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
        }
        if (userChoice == '3') {
            exit(0);
        }
        else {
            printf("Error. Invalid choice.\n");
            printf("Enter 1 to start the game, enter 2 to read the help manual, or enter 3 to exit: ");
            scanf("%c", &userChoice);
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
    int userHandCapacity = 10;
    Card *userHand = (Card *) malloc(userHandCapacity * sizeof(Card));

    if (userHand == NULL) {
        printf(RED "Could not allocate memory. Program terminated.\n" RESET);
        exit(0);
    }

    int cpuHandSize = userHandSize;
    int cpuHandCapactity = userHandCapacity;
    Card *tmp = (Card *) malloc (userHandCapacity * sizeof(Card));

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
        userTurn(userHand, deck, discardPile, &userHandSize, &userHandCapacity, &deckSize, &discardPileSize);
        if (userHandSize == 1) {
            printf("User says UNO!\n");
        }
        cpuTurn(cpuHand, deck, discardPile, &cpuHandSize, &cpuHandCapactity, &deckSize, &discardPileSize);
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