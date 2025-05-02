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

void userTurn(Card **userHand, Card *deck, Card **discardPile, int *userHandSize, int *deckSize, int *discardPileSize) {
    printHand(*userHand, *userHandSize);

    char userColorChoice;
    char userValueChoice;

    printf("Select which card to draw from ([R/B/G/Y/W] + value) (ex. R 1): ");
    scanf("%c %c", &userColorChoice, &userValueChoice);

    while ((userColorChoice == 'W' && (userValueChoice != 'D' && userValueChoice != 'W')) || (userColorChoice != 'W' && userValueChoice == 'W')) {
        printHelp();
        printHand(*userHand, *userHandSize);
        printf("Invalid wild card input. Enter another card (ex. R 1): ");
        scanf("%c %c", &userColorChoice, &userValueChoice);
    }

    while (userColorChoice == '+') {
        addCardToHand(userHand, &deck, userHandSize, deckSize);
        printHand(*userHand, *userHandSize);
        printf("Drew a card. Select which card to draw from ([R/B/G/Y/W] + value) (ex. R 1): ");
        getchar();
        scanf("%c %c", &userColorChoice, &userValueChoice);
    }
    int cardIdx = findCard(*userHand, *userHandSize, userColorChoice, userValueChoice);

    while (cardIdx == -1) {
        printf("Card not found in hand.\n");
        printf("Select which card to draw from ([R/B/G/Y/W] + value shown on screen) (ex. R 1): ");
        getchar();
        scanf("%c %c", &userColorChoice, &userValueChoice);
        cardIdx = findCard(*userHand, *userHandSize, userColorChoice, userValueChoice);
    }

    removeCardFromHand(userHand, discardPile, userHandSize, discardPileSize, cardIdx);

    if ((*discardPile)[(*discardPileSize) - 1].color == 'W') {
        char pickColor;
        printf("Choose color for wild card (R/G/B/Y): ");
        getchar();
        scanf("%c", &pickColor);
        printf("User picks %c\n", pickColor);
        (*discardPile)[(*discardPileSize) - 1].color = pickColor;
        (*discardPile)[(*discardPileSize) - 1].value = 'W';
    }
}

void cpuTurn(Card **cpuHand, Card *deck, Card **discardPile, int *cpuHandSize, int *deckSize, int *discardPileSize) {
    printf("-------------------------------------------------\n");
    printf("CPU's Turn\n");

    Card *candidates = (Card *) malloc(10 * sizeof(Card));
    if (candidates == NULL) {
        printf(RED "Could not allocate memory. Program terminated.\n" RESET);
        exit(0);
    }
    int candSize = 0;
    int maxCandidates = 10;

    for (int i = 0; i < *cpuHandSize; i++) {
        Card curr = (*cpuHand)[i];
        Card top = (*discardPile)[*discardPileSize - 1];
        if (curr.color == top.color || curr.color == 'W' || curr.value == top.value) {
            if (candSize == maxCandidates) {
                maxCandidates *= 2;
                candidates = (Card *) realloc(candidates, maxCandidates * sizeof(Card));
                if (candidates == NULL) {
                    printf(RED "Could not reallocate memory. Program terminated.\n" RESET);
                    exit(0);
                }
            }
            candidates[candSize++] = curr;
        }   
    }
    

    while (candSize == 0) {
        addCardToHand(cpuHand, &deck, cpuHandSize, deckSize);
        Card drawn = (*cpuHand)[*cpuHandSize - 1];
        if ((*cpuHand)[(*cpuHandSize) - 1].color == (*discardPile)[(*discardPileSize) - 1].color || (*cpuHand)[(*cpuHandSize) - 1].color == 'W' || (*cpuHand)[(*cpuHandSize) - 1].value == (*discardPile)[(*discardPileSize) - 1].value) {
            candSize++;
            if (candSize == maxCandidates) {
                maxCandidates *= 2;
                candidates = (Card *) realloc(candidates, maxCandidates * sizeof(Card));
                if (candidates == NULL) {
                    printf(RED "Could not reallocate memory. Program terminated.\n" RESET);
                    exit(0);
                }
            }
            candidates[candSize++] = drawn;
        }
    }
    

    int candIdx = rand() % candSize;
    while (candSize > 1 && candidates[candIdx].color == 'W' && candidates[candIdx].value == 'D') {
        candIdx = rand() % candSize;
    }
    int cardIdx = findCard(*cpuHand, *cpuHandSize, candidates[candIdx].color, candidates[candIdx].value);
    
    removeCardFromHand(cpuHand, discardPile, cpuHandSize, discardPileSize, cardIdx);
    
    printf("CPU played ");
    printCard((*discardPile)[(*discardPileSize) - 1]);
    printf("\n");


    if ((*discardPile)[(*discardPileSize) - 1].color == 'W') {
        int randomColor = rand() % 4;
        char chosenColor = "RGBY"[randomColor];
        (*discardPile)[(*discardPileSize) - 1].color = chosenColor;
        printf("Color of wildcard is now %c\n", (*discardPile)[(*discardPileSize) - 1].color);
    }

    free(candidates);
    candidates = NULL;
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
    Card *deck = (Card*) malloc(numCards * sizeof(Card));
    if (deck == NULL) {
        printf(RED "Could not allocate memory. Program terminated.\n" RESET);
        exit(0);
    }

    initDeck(deck);
    printf("Deck initialized.\n");

    shuffle(deck, numCards);
    printf("Deck shuffled.\n");

    int deckSize = numCards;
    Card *userHand = NULL;
    Card *cpuHand = NULL;
    int userHandSize = 0;
    int cpuHandSize = 0;

    Card* discardPile = (Card*) malloc(sizeof(Card));
    int discardPileSize = 0;
    if (discardPile == NULL) {
        printf(RED "Could not allocate memory. Program terminated.\n" RESET);
        exit(0);
    }
    
    createHands(&userHand, &cpuHand, deck, &deckSize, &userHandSize, &cpuHandSize);
    deck = realloc(deck, deckSize * sizeof(Card));
    if (deck == NULL) {
        printf(RED "Could not reallocate memory. Program terminated.\n" RESET);
        exit(0);
    }
    printf("Hands created.\n");

    while (userHandSize != 0 && cpuHandSize != 0) {
        // User Turn
        userTurn(&userHand, deck, &discardPile, &userHandSize, &deckSize, &discardPileSize);
        if (userHandSize == 1) {
            printf("User says UNO!\n");
        }
        if (discardPile[discardPileSize - 1].color, discardPile[discardPileSize - 1].value);
        cpuTurn(&cpuHand, deck, &discardPile, &cpuHandSize, &deckSize, &discardPileSize);
        if (cpuHandSize == 1) {
            printf("CPU says UNO!\n");
        }
        getchar();
        
    }

    if (userHandSize == 0) {
        printf("User wins!");
    }

    else {
        printf("CPU wins!\n");
    }

    free(deck);
    free(userHand);
    free(cpuHand);
    free(discardPile);

    return 0;
}