#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

//main.c only allows player to draw 1 card per move

#define SIZE 52
#define STRING 10

void setDeck(int[], int[]);
void printDeck(int[], int[]);
void printCard(int[], int[], int);
void shuffle(int[], int[]);
bool validMove(int[], int[], int, int);
void removeCard(int[], int);
int getSuit(int[], int);
int getValue(int[], int);

int main(void) {
    int i, j;

    int values[SIZE] = {0};
    int suits[SIZE] = {0};

    int player[SIZE] = {0};
    int computer[SIZE] = {0};

    int youCards, compCards, compCardsDrawn, playIndex, drawIndex, move, suitSelect;

    bool youMove = false, compMove = false, cardFound = false;

    srand(time(NULL));

    setDeck(values, suits);
    shuffle(values, suits);

    youCards = 5;
    compCards = 5;

    playIndex = 11;
    drawIndex = 12;
    for (i = 0; i < 5; i++) {
        player[i] = i;
        computer[i] = i + 5;
    }

    while (youCards > 0 && compCards > 0 && drawIndex < SIZE) {

        printf("CRAZY 8! - YOU: %d - COMPUTER: %d\n", youCards, compCards);

        printf("\nYOUR CARDS:\n");
        for (i = 0; i < youCards; i++) {
            printf("Card: %d | ", i + 1);
            printCard(values, suits, player[i]);
        }

        printf("\nPLAY CARD: ");
        printCard(values, suits, playIndex);

        printf("\n");
        do {
            printf("MAKE MOVE (1-%d, 0 to draw):", youCards);
            scanf("%d", &move);

            if (move != 0) {
                youMove = validMove(values, suits, playIndex, player[move - 1]);
            }
            else
                youMove = true;
        } while (move < 0 || move > youCards || !youMove);
        printf("\n");

        if (move == 0) {
            player[youCards] = drawIndex;
            youCards++;
            drawIndex++;
            printf("CARD DRAWN - PLAY AGAIN\n\n");
        }
        else {
            playIndex = player[move - 1];
            if (getValue(values, playIndex) == 8) {
                printf("8 CARD PLAYED\n");
                do {
                    printf("NAME SUIT (1:C, 2:D, 3:H, 4:S):");
                    scanf("%d", &suitSelect);
                } while (suitSelect < 1 || suitSelect > 4);
                suits[playIndex] = suitSelect;
                printf("\n");
            }

            //Basically used as a sentinel value. If = -1, removes card from hand. if > 0, keeps card in hand
            player[move - 1] = -1;
            youCards--;
            removeCard(player, youCards);
            printf("CARD PLAYED: ");
            printCard(values, suits, playIndex);

            compCardsDrawn = 0;
            cardFound = false;
            while(!cardFound && youCards > 0 && drawIndex < SIZE) {
                i = 0;
                compMove = false;
                while (!compMove && i < compCards) {
                    if (validMove(values, suits, playIndex, computer[i]))
                        compMove = true;
                    else {
                        i++;
                    }
                }

                if (!compMove) {
                    computer[compCards] = drawIndex;
                    compCards++;
                    compCardsDrawn++;
                    drawIndex++;
                }
                else
                    cardFound = true;
            }

            if (cardFound) {
                playIndex = computer[i];
                if (getValue(values, playIndex) == 8)
                    suits[playIndex] = rand() % 4 + 1;
                computer[i] = -1;
                compCards--;
                removeCard(computer, compCards);
                printf("\nCOMPUTER DREW %d CARDS\nCOMPUTER CARD PLAYED: ", compCardsDrawn);
                printCard(values, suits, playIndex);
                if (values[playIndex] == 8)
                    printf("COMPUTER PLAYED 8 CARD AND CHOSE SUIT\n");
                printf("\n");
            }
        }
    }

    if (youCards == 0)
        printf("\nYOU WIN - GAME OVER\n");
    else if (compCards == 0)
        printf("COMPUTER WINS - GAME OVER\n");
    else
        printf("TIE - GAME OVER\n");

    return 0;
}

void setDeck(int values[], int suits[]) {
    int i, cardCounter = 2, suitCounter = 1;

    for (i = 0; i < SIZE; i++) {
        values[i] = cardCounter;
        suits[i] = suitCounter;

        if (cardCounter % 14 == 0) {
            cardCounter = 2;
            suitCounter++;
        }
        else
            cardCounter++;
    }
}

void printDeck(int values[], int suits[]) {
    char suit[STRING];
    char value[STRING];
    int i;

    for (i = 0; i < SIZE; i++) {
        switch (suits[i]) {
            case 1:
                strcpy(suit, "Clubs");
                break;
            case 2:
                strcpy(suit, "Diamonds");
                break;
            case 3:
                strcpy(suit, "Hearts");
                break;
            case 4:
                strcpy(suit, "Spades");
                break;
        }

        switch (values[i]) {
            case 11:
                strcpy(value, "Jack");
                printf("Suit: %s | Value: %s\n", suit, value);
                break;
            case 12:
                strcpy(value, "King");
                printf("Suit: %s | Value: %s\n", suit, value);
                break;
            case 13:
                strcpy(value, "Queen");
                printf("Suit: %s | Value: %s\n", suit, value);
                break;
            case 14:
                strcpy(value, "Ace");
                printf("Suit: %s | Value: %s\n", suit, value);
                break;
            default:
                printf("Suit: %s | Value: %d\n", suit, values[i]);
                break;
        }
    }
}

void printCard(int values[], int suits[], int c) {
    char suit[STRING];
    char value[STRING];

    switch (suits[c]) {
        case 1:
            strcpy(suit, "Clubs");
            break;
        case 2:
            strcpy(suit, "Diamonds");
            break;
        case 3:
            strcpy(suit, "Hearts");
            break;
        case 4:
            strcpy(suit, "Spades");
            break;
    }

    switch (values[c]) {
        case 11:
            strcpy(value, "Jack");
            printf("Suit: %s | Value: %s\n", suit, value);
            break;
        case 12:
            strcpy(value, "King");
            printf("Suit: %s | Value: %s\n", suit, value);
            break;
        case 13:
            strcpy(value, "Queen");
            printf("Suit: %s | Value: %s\n", suit, value);
            break;
        case 14:
            strcpy(value, "Ace");
            printf("Suit: %s | Value: %s\n", suit, value);
            break;
        default:
            printf("Suit: %s | Value: %d\n", suit, values[c]);
            break;
    }
}

void shuffle(int values[], int suits[]) {
    int i, r1, r2, t1, t2;

    for (i = 0; i < 1000; i++) {
        r1 = rand() % 52;
        r2 = rand() % 52;

        if (r1 != r2) {
            t1 = values[r1];
            t2 = suits[r1];
            values[r1] = values[r2];
            suits[r1] = suits[r2];
            values[r2] = t1;
            suits[r2] = t2;
        }
    }
}

int getSuit(int suits[], int c) {
    return suits[c];
}

int getValue(int values[], int c) {
    return values[c];
}

bool validMove(int values[], int suits[], int c, int m) {
    if (getSuit(suits, c) == getSuit(suits, m) ||
            getValue(values, c) == getValue(values, m) ||
            getValue(values, m) == 8)
        return true;
    else
        return false;
}

void removeCard(int player[], int youCards) {
    int i = 0, j = 0;
    while (i < (youCards + 1) && j < youCards) {
        if (player[i] >= 0) {
            player[j] = player[i];
            j++;
        }
        i++;
    }
}